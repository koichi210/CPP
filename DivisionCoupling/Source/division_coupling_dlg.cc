// division_coupling_dlg.cc : メインダイアログ（ファイルの分割と結合）

#include "stdafx.h"
#include "division_coupling.h"
#include "division_coupling_dlg.h"

#include <climits>
#include <memory>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int kKilobyte = 1024;
	constexpr UINT kMergeBufferSize = 1024 * kKilobyte;	// 結合で1回に読み書きする大きさ

	// malloc で確保した作業バッファを自動で解放する（確保できなければ空）
	struct FreeDeleter
	{
		void operator()(char* p) const	{ free(p); }
	};
	using WorkBuffer = std::unique_ptr<char, FreeDeleter>;

	WorkBuffer AllocBuffer(size_t size)
	{
		return WorkBuffer(static_cast<char*>(malloc(size)));
	}

	// 分割ファイル名（例: fname.jpg → fname.jpg.div001）
	CString MakePartPath(const CString& path, int index)
	{
		CString part_path;
		part_path.Format(_T("%s.div%03d"), static_cast<LPCTSTR>(path), index);
		return part_path;
	}
}

DivisionCouplingDlg::DivisionCouplingDlg(CWnd* parent)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void DivisionCouplingDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Control(dx, IDPR_SPLITMERGE, progress_);
}

BEGIN_MESSAGE_MAP(DivisionCouplingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_SPLIT, &DivisionCouplingDlg::OnBnClickedSplit)
	ON_BN_CLICKED(IDBT_MERGE, &DivisionCouplingDlg::OnBnClickedMerge)
	ON_BN_CLICKED(IDBT_SPLIT_BROWSE, &DivisionCouplingDlg::OnBnClickedSplitBrowse)
	ON_BN_CLICKED(IDBT_MERGE_BROWSE, &DivisionCouplingDlg::OnBnClickedMergeBrowse)
	ON_BN_CLICKED(IDBT_STOP, &DivisionCouplingDlg::OnBnClickedStop)
	ON_WM_ENDSESSION()
	ON_MESSAGE(kWmAppProcessFinished, &DivisionCouplingDlg::OnProcessFinished)
END_MESSAGE_MAP()

BOOL DivisionCouplingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

void DivisionCouplingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// アイコンをクライアント領域の中央に描く
		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		dc.DrawIcon((rect.Width() - icon_width + 1) / 2, (rect.Height() - icon_height + 1) / 2, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR DivisionCouplingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void DivisionCouplingDlg::OnBnClickedSplit()
{
	GetDlgItemText(IDET_SPLIT_FNAME, src_path_);

	// KB → バイト。int に収まらない値は分割サイズエラーにする
	const int div_size_kb = static_cast<int>(GetDlgItemInt(IDET_SIZE));
	div_size_ = (0 < div_size_kb && div_size_kb <= INT_MAX / kKilobyte) ? div_size_kb * kKilobyte : 0;

	StartProcess(&DivisionCouplingDlg::SplitThreadProc);
}

void DivisionCouplingDlg::OnBnClickedMerge()
{
	GetDlgItemText(IDET_MERGE_FNAME, src_path_);

	// 選んだ分割ファイルから最後の拡張子を外したものが結合後のファイル名
	// 例: fname.jpg.div001 → fname.jpg
	const int dot = src_path_.ReverseFind(_T('.'));
	dest_path_ = (dot >= 0) ? src_path_.Left(dot) : src_path_;

	// 確認はワーカーを動かす前に UI スレッドで行う
	if (PathFileExists(dest_path_))
	{
		if (MessageBox(_T("すでにファイルが存在します。上書きしますか？\n") + dest_path_, _T("Warning"), MB_YESNO) == IDNO)
		{
			MessageBox(_T("処理を中断しました。"));
			return;
		}
	}

	StartProcess(&DivisionCouplingDlg::MergeThreadProc);
}

void DivisionCouplingDlg::OnBnClickedSplitBrowse()
{
	BrowseFile(IDET_SPLIT_FNAME);
}

void DivisionCouplingDlg::OnBnClickedMergeBrowse()
{
	BrowseFile(IDET_MERGE_FNAME);
}

// 開始ボタンはワーカーが止まりきってから OnProcessFinished で戻す（前の処理と同時に動かさないため）
void DivisionCouplingDlg::OnBnClickedStop()
{
	running_ = false;
	GetDlgItem(IDBT_STOP)->EnableWindow(FALSE);
}

void DivisionCouplingDlg::OnOK()
{
	if (StopWorkersForClose())
	{
		CDialogEx::OnOK();
	}
}

void DivisionCouplingDlg::OnCancel()
{
	if (StopWorkersForClose())
	{
		CDialogEx::OnCancel();
	}
}

bool DivisionCouplingDlg::StopWorkersForClose()
{
	if (workers_.IsRunning())
	{
		const int answer = MessageBox(_T("処理中です。中止して閉じますか？\n\n作成途中のファイルは削除します。"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
		if (answer != IDYES)
		{
			return false;
		}
	}

	StopWorkers();
	return true;
}

// シャットダウン・ログオフでは、この後すぐプロセスごと終了させられるので、確認せずに止めて後片付けさせる
void DivisionCouplingDlg::OnEndSession(BOOL ending)
{
	if (ending)
	{
		StopWorkers();
	}
	CDialogEx::OnEndSession(ending);
}

// ワーカーはダイアログを触るので、閉じる（ダイアログが破棄される）前に止めて終了を待つ
void DivisionCouplingDlg::StopWorkers()
{
	closing_ = true;
	running_ = false;
	EnableWindow(FALSE);	// 待っている間に操作させない
	workers_.WaitAll();
	EnableWindow(TRUE);
}

// ワーカーは画面に何も出さず、終わったことだけ UI スレッドに伝えてすぐ終わる
// （ワーカーがメッセージボックスを出すと、閉じるまでスレッドが終われないため）
UINT DivisionCouplingDlg::SplitThreadProc(LPVOID param)
{
	auto dlg = static_cast<DivisionCouplingDlg*>(param);
	dlg->Split();
	dlg->PostMessage(kWmAppProcessFinished);
	return TRUE;
}

UINT DivisionCouplingDlg::MergeThreadProc(LPVOID param)
{
	auto dlg = static_cast<DivisionCouplingDlg*>(param);
	dlg->Merge();
	dlg->PostMessage(kWmAppProcessFinished);
	return TRUE;
}

void DivisionCouplingDlg::StartProcess(AFX_THREADPROC thread_proc)
{
	error_ = Error::kNone;
	running_ = true;
	EnableControls(true);
	workers_.Start(thread_proc, this);
}

LRESULT DivisionCouplingDlg::OnProcessFinished(WPARAM /*w_param*/, LPARAM /*l_param*/)
{
	running_ = false;
	EnableControls(false);

	// 閉じる途中はダイアログの終了を妨げないよう、何も表示しない
	if (closing_)
	{
		return 0;
	}

	CString msg;
	switch (error_)
	{
	case Error::kNone:
		return 0;
	case Error::kAborted:
		MessageBox(_T("処理を中止しました。\n作成途中のファイルは削除しました。"), _T("info"), MB_OK);
		return 0;
	case Error::kOpenSource:
		msg = _T("元ファイルオープンエラー");
		break;
	case Error::kOpenDest:
		msg.Format(_T("ファイルが作成できません\n\n%s"), static_cast<LPCTSTR>(dest_path_));
		break;
	case Error::kAlloc:
		msg = _T("Workメモリ確保エラー");
		break;
	case Error::kDivSize:
		msg = _T("分割サイズエラー");
		break;
	}
	MessageBox(msg, _T("error"), MB_OK);
	return 0;
}

void DivisionCouplingDlg::Split()
{
	if (div_size_ <= 0)
	{
		error_ = Error::kDivSize;
		return;
	}

	CFile src_file;
	if (!src_file.Open(src_path_, CFile::modeRead))
	{
		error_ = Error::kOpenSource;
		return;
	}

	// 読み込みバッファは分割サイズで1回だけ確保する
	WorkBuffer buffer = AllocBuffer(div_size_);
	if (!buffer)
	{
		error_ = Error::kAlloc;
		return;
	}

	ULONGLONG rest_size = src_file.GetLength();
	const int part_count = static_cast<int>((rest_size + div_size_ - 1) / div_size_);	// 切り上げ
	progress_.SetRange32(0, part_count);

	// 途中で終わったときに消せるよう、作った分割ファイルを覚えておく
	std::vector<CString> created_parts;

	for (int index = 1; rest_size > 0 && running_; index++)
	{
		dest_path_ = MakePartPath(src_path_, index);
		CFile dest_file;
		if (!dest_file.Open(dest_path_, CFile::modeCreate | CFile::modeWrite))
		{
			error_ = Error::kOpenDest;
			break;
		}
		created_parts.push_back(dest_path_);

		const UINT size = (rest_size > static_cast<ULONGLONG>(div_size_))
			? static_cast<UINT>(div_size_) : static_cast<UINT>(rest_size);
		rest_size -= size;

		src_file.Read(buffer.get(), size);
		dest_file.Write(buffer.get(), size);
		dest_file.Close();

		progress_.SetPos(index);
	}

	src_file.Close();

	// 分割ファイルが欠けたまま残ると、結合したときに欠けたファイルが黙ってできてしまう
	if (rest_size > 0)
	{
		if (error_ == Error::kNone)
		{
			error_ = Error::kAborted;
		}
		for (const CString& part : created_parts)
		{
			::DeleteFile(part);
		}
	}
}

void DivisionCouplingDlg::Merge()
{
	// 分割ファイルの大きさによらず、決まった大きさのバッファで少しずつ写す
	WorkBuffer buffer = AllocBuffer(kMergeBufferSize);
	if (!buffer)
	{
		error_ = Error::kAlloc;
		return;
	}

	// 結合先 dest_path_ は OnBnClickedMerge で決めて、上書きの確認も済んでいる
	CFile dest_file;
	if (!dest_file.Open(dest_path_, CFile::modeCreate | CFile::modeWrite))
	{
		error_ = Error::kOpenDest;
		return;
	}

	// .div001 から番号順に、ファイルが無くなるまで連結する
	for (int index = 1; ; index++)
	{
		if (!running_)
		{
			error_ = Error::kAborted;
			break;
		}

		CFile part_file;
		if (!part_file.Open(MakePartPath(dest_path_, index), CFile::modeRead))
		{
			break;
		}

		UINT read_size;
		while ((read_size = part_file.Read(buffer.get(), kMergeBufferSize)) > 0)
		{
			dest_file.Write(buffer.get(), read_size);
		}
		part_file.Close();
	}

	dest_file.Close();

	// 途中で終わった結合結果は欠けているので残さない
	if (error_ != Error::kNone)
	{
		::DeleteFile(dest_path_);
	}
}

void DivisionCouplingDlg::BrowseFile(UINT edit_id)
{
	CFileDialog dlg(TRUE, _T("*.*"), nullptr, OFN_CREATEPROMPT, _T("*.*|*.*|全て(*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(edit_id, dlg.GetPathName());
	}
}

void DivisionCouplingDlg::EnableControls(bool running)
{
	GetDlgItem(IDBT_SPLIT)->EnableWindow(!running);
	GetDlgItem(IDBT_MERGE)->EnableWindow(!running);
	GetDlgItem(IDBT_STOP)->EnableWindow(running);
}
