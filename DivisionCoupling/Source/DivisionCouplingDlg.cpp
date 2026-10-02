// DivisionCouplingDlg.cpp : メインダイアログ（ファイルの分割と結合）

#include "stdafx.h"
#include "DivisionCoupling.h"
#include "DivisionCouplingDlg.h"

#include <climits>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int KILOBYTE = 1024;

	// calloc で確保した作業バッファを自動で解放する
	struct FreeDeleter
	{
		void operator()(char* p) const	{ free(p); }
	};
	using CallocBuffer = std::unique_ptr<char, FreeDeleter>;

	CallocBuffer AllocBuffer(size_t size)
	{
		return CallocBuffer(static_cast<char*>(calloc(size, sizeof(char))));
	}

	// 分割ファイル名（例: fname.jpg → fname.jpg.div001）
	CString MakePartPath(const CString& path, int index)
	{
		CString partPath;
		partPath.Format(_T("%s.div%03d"), static_cast<LPCTSTR>(path), index);
		return partPath;
	}
}

CDivisionCouplingDlg::CDivisionCouplingDlg(CWnd* pParent)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDivisionCouplingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDPR_SPLITMERGE, m_progress);
}

BEGIN_MESSAGE_MAP(CDivisionCouplingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_SPLIT, &CDivisionCouplingDlg::OnBnClickedSplit)
	ON_BN_CLICKED(IDBT_MERGE, &CDivisionCouplingDlg::OnBnClickedMerge)
	ON_BN_CLICKED(IDBT_SPLIT_BROWSE, &CDivisionCouplingDlg::OnBnClickedSplitBrowse)
	ON_BN_CLICKED(IDBT_MERGE_BROWSE, &CDivisionCouplingDlg::OnBnClickedMergeBrowse)
	ON_BN_CLICKED(IDBT_STOP, &CDivisionCouplingDlg::OnBnClickedStop)
END_MESSAGE_MAP()

BOOL CDivisionCouplingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

void CDivisionCouplingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// アイコンをクライアント領域の中央に描く
		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		dc.DrawIcon((rect.Width() - cxIcon + 1) / 2, (rect.Height() - cyIcon + 1) / 2, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CDivisionCouplingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDivisionCouplingDlg::OnBnClickedSplit()
{
	GetDlgItemText(IDET_SPLIT_FNAME, m_srcPath);

	// KB → バイト。int に収まらない値は分割サイズエラーにする
	const int divSizeKB = static_cast<int>(GetDlgItemInt(IDET_SIZE));
	m_divSize = (0 < divSizeKB && divSizeKB <= INT_MAX / KILOBYTE) ? divSizeKB * KILOBYTE : 0;

	StartProcess(&CDivisionCouplingDlg::SplitThreadProc);
}

void CDivisionCouplingDlg::OnBnClickedMerge()
{
	GetDlgItemText(IDET_MERGE_FNAME, m_srcPath);
	StartProcess(&CDivisionCouplingDlg::MergeThreadProc);
}

void CDivisionCouplingDlg::OnBnClickedSplitBrowse()
{
	BrowseFile(IDET_SPLIT_FNAME);
}

void CDivisionCouplingDlg::OnBnClickedMergeBrowse()
{
	BrowseFile(IDET_MERGE_FNAME);
}

void CDivisionCouplingDlg::OnBnClickedStop()
{
	m_running = false;
	EnableControls(false);
}

UINT CDivisionCouplingDlg::SplitThreadProc(LPVOID pParam)
{
	auto pDlg = static_cast<CDivisionCouplingDlg*>(pParam);
	pDlg->Split();
	pDlg->FinishProcess();
	return TRUE;
}

UINT CDivisionCouplingDlg::MergeThreadProc(LPVOID pParam)
{
	auto pDlg = static_cast<CDivisionCouplingDlg*>(pParam);
	pDlg->Merge();
	pDlg->FinishProcess();
	return TRUE;
}

void CDivisionCouplingDlg::StartProcess(AFX_THREADPROC pfnThreadProc)
{
	m_error = Error::None;
	m_running = true;
	EnableControls(true);
	AfxBeginThread(pfnThreadProc, this);
}

void CDivisionCouplingDlg::FinishProcess()
{
	m_running = false;
	EnableControls(false);

	CString msg;
	switch (m_error)
	{
	case Error::None:
		return;
	case Error::OpenSource:
		msg = _T("元ファイルオープンエラー");
		break;
	case Error::OpenDest:
		msg.Format(_T("ファイルが作成できません\n\n%s"), static_cast<LPCTSTR>(m_destPath));
		break;
	case Error::Alloc:
		msg = _T("Workメモリ確保エラー");
		break;
	case Error::DivSize:
		msg = _T("分割サイズエラー");
		break;
	}
	MessageBox(msg, _T("error"), MB_OK);
}

void CDivisionCouplingDlg::Split()
{
	if (m_divSize <= 0)
	{
		m_error = Error::DivSize;
		return;
	}

	CFile srcFile;
	if (!srcFile.Open(m_srcPath, CFile::modeRead))
	{
		m_error = Error::OpenSource;
		return;
	}

	// 読み込みバッファは分割サイズで1回だけ確保する
	CallocBuffer buffer = AllocBuffer(m_divSize);
	if (!buffer)
	{
		m_error = Error::Alloc;
		return;
	}

	ULONGLONG restSize = srcFile.GetLength();
	int partCount = static_cast<int>(restSize / m_divSize);
	if (restSize % m_divSize)
	{
		partCount++;
	}
	m_progress.SetRange32(0, partCount);

	for (int index = 1; restSize > 0 && m_running; index++)
	{
		m_destPath = MakePartPath(m_srcPath, index);
		CFile destFile;
		if (!destFile.Open(m_destPath, CFile::modeCreate | CFile::modeWrite))
		{
			m_error = Error::OpenDest;
			break;
		}

		const UINT size = (restSize > static_cast<ULONGLONG>(m_divSize))
			? static_cast<UINT>(m_divSize) : static_cast<UINT>(restSize);
		restSize -= size;

		srcFile.Read(buffer.get(), size);
		destFile.Write(buffer.get(), size);
		destFile.Close();

		m_progress.SetPos(index);
	}

	srcFile.Close();
}

void CDivisionCouplingDlg::Merge()
{
	// 選んだ分割ファイルから最後の拡張子を外したものが結合後のファイル名
	// 例: fname.jpg.div001 → fname.jpg
	const int dot = m_srcPath.ReverseFind(_T('.'));
	m_destPath = (dot >= 0) ? m_srcPath.Left(dot) : m_srcPath;

	if (PathFileExists(m_destPath))
	{
		if (MessageBox(_T("すでにファイルが存在します。上書きしますか？\n") + m_destPath, _T("Warning"), MB_YESNO) == IDNO)
		{
			MessageBox(_T("処理を中断しました。"));
			return;
		}
	}

	CFile destFile;
	if (!destFile.Open(m_destPath, CFile::modeCreate | CFile::modeWrite))
	{
		m_error = Error::OpenDest;
		return;
	}

	// .div001 から番号順に、ファイルが無くなるまで連結する
	for (int index = 1; m_running; index++)
	{
		CFile partFile;
		if (!partFile.Open(MakePartPath(m_destPath, index), CFile::modeRead))
		{
			break;
		}

		const UINT size = static_cast<UINT>(partFile.GetLength());
		CallocBuffer buffer = AllocBuffer(size);
		if (!buffer)
		{
			m_error = Error::Alloc;
			break;
		}

		partFile.Read(buffer.get(), size);
		destFile.Write(buffer.get(), size);
		partFile.Close();
	}

	destFile.Close();
}

void CDivisionCouplingDlg::BrowseFile(UINT editId)
{
	CFileDialog dlg(TRUE, _T("*.*"), nullptr, OFN_CREATEPROMPT, _T("*.*|*.*|全て(*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(editId, dlg.GetPathName());
	}
}

void CDivisionCouplingDlg::EnableControls(bool running)
{
	GetDlgItem(IDBT_SPLIT)->EnableWindow(!running);
	GetDlgItem(IDBT_MERGE)->EnableWindow(!running);
	GetDlgItem(IDBT_STOP)->EnableWindow(running);
}
