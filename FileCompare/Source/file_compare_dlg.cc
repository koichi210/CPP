// file_compare_dlg.cc : メインダイアログ（指定フォルダ内の重複ファイルを探す）

#include "stdafx.h"
#include "file_compare.h"
#include "file_compare_dlg.h"
#include "afxdialogex.h"
#include "file_comparer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// 配列はダイアログ生成時に確保し、比較中に再確保しない（比較スレッドが参照し続けるため）
	constexpr int		kMaxFileCount	= 100000;
	constexpr size_t	kMaxFolderCount	= 20;
	constexpr int		kNoGroup		= 0;
	constexpr LPCTSTR	kDefaultPath	= _T("C:\\windows;D:\\download;E:\\tmp");
	constexpr LPCTSTR	kStartLabel		= _T("比較");
	constexpr LPCTSTR	kStopLabel		= _T("停止");
	constexpr LPCTSTR	kNewLine		= _T("\r\n");
}

FileCompareDlg::FileCompareDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
	, icon_(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
	, running_(false)
	, files_(kMaxFileCount)
	, file_count_(0)
	, next_group_(1)
{
}

void FileCompareDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Control(dx, IDPR_COMPARE, progress_);
}

BEGIN_MESSAGE_MAP(FileCompareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_EXECUTE, &FileCompareDlg::OnBnClickedExecute)
END_MESSAGE_MAP()

BOOL FileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	SetDlgItemText(IDBT_EXECUTE, kStartLabel);
	SetDlgItemText(IDET_PATH, kDefaultPath);
	CEdit* path_edit = static_cast<CEdit*>(GetDlgItem(IDET_PATH));
	path_edit->SetFocus();
	path_edit->SetSel(0, -1);
	return FALSE;	// フォーカスを自分で設定したので FALSE
}

// 最小化時のアイコン描画（ダイアログアプリでは自前で描く必要がある）
void FileCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - icon_width + 1) / 2;
		const int y = (rect.Height() - icon_height + 1) / 2;
		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR FileCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// 比較中なら停止、そうでなければ比較を開始する
void FileCompareDlg::OnBnClickedExecute()
{
	if (running_)
	{
		StopCompare();
		return;
	}
	StartCompare();
}

void FileCompareDlg::StartCompare()
{
	file_count_ = 0;
	next_group_ = 1;
	running_ = true;
	SetDlgItemText(IDBT_EXECUTE, kStopLabel);
	SetDlgItemText(IDET_RESULT, _T(""));

	CollectFiles(GetFolders());

	// 総当たりの進捗。表示上の位置は「行 * ファイル数 + 列」で数える
	progress_.SetRange32(0, GetProgressEnd());
	UpdateProgress(0, 0);
	AfxBeginThread(CompareThread, this);
}

void FileCompareDlg::StopCompare()
{
	SetDlgItemText(IDBT_EXECUTE, kStartLabel);
	running_ = false;
}

std::vector<CString> FileCompareDlg::GetFolders() const
{
	std::vector<CString> folders;
	CString rest;
	GetDlgItemText(IDET_PATH, rest);
	if (rest.IsEmpty())
	{
		return folders;
	}

	// 上限を超えた分は捨てる
	while (folders.size() < kMaxFolderCount)
	{
		const int sep = rest.Find(_T(';'));
		if (sep < 0)
		{
			folders.push_back(rest);
			break;
		}
		folders.push_back(rest.Left(sep));
		rest.Delete(0, sep + 1);
	}
	return folders;
}

// 各フォルダ直下のファイルを集める（サブフォルダはたどらない）
void FileCompareDlg::CollectFiles(const std::vector<CString>& folders)
{
	CFileFind finder;
	for (const CString& folder : folders)
	{
		if (folder.IsEmpty())
		{
			continue;
		}

		BOOL found = finder.FindFile(folder + _T("\\*"));
		while (found)
		{
			found = finder.FindNextFile();
			if (finder.IsDots() || finder.IsDirectory())
			{
				continue;
			}
			if (file_count_ >= kMaxFileCount)
			{
				break;
			}
			files_[file_count_].path = finder.GetFilePath();
			files_[file_count_].group = kNoGroup;
			file_count_++;
		}
	}
}

UINT FileCompareDlg::CompareThread(LPVOID param)
{
	static_cast<FileCompareDlg*>(param)->CompareFiles();
	return 0;
}

// 総当たりで比較し、同じ内容のファイルに同じ組番号を付ける
void FileCompareDlg::CompareFiles()
{
	FileComparer comparer;
	const int end = GetProgressEnd();

	for (int i = 0; i < file_count_ && running_; i++)
	{
		UpdateProgress(i * file_count_, end);
		if (files_[i].group != kNoGroup)
		{
			continue;	// すでにどこかの組に入っている
		}

		for (int j = i + 1; j < file_count_ && running_; j++)
		{
			UpdateProgress(i * file_count_ + j, end);
			if (files_[j].group != kNoGroup)
			{
				continue;
			}
			if (comparer.CompareBinary(files_[i].path, files_[j].path))
			{
				files_[i].group = next_group_;
				files_[j].group = next_group_;
			}
		}

		if (files_[i].group != kNoGroup)
		{
			next_group_++;
		}
	}

	StopCompare();
	ShowResult();
}

int FileCompareDlg::GetProgressEnd() const
{
	return file_count_ * (file_count_ - 1);
}

void FileCompareDlg::UpdateProgress(int pos, int end)
{
	progress_.SetPos(pos);

	CString text;
	text.Format(_T("(%d / %d)"), pos, end);
	SetDlgItemText(IDST_COMPARE, text);
}

void FileCompareDlg::ShowResult()
{
	CString result;
	for (int group = 1; group < next_group_; group++)
	{
		CString header;
		header.Format(_T("[Group%d]%s"), group, kNewLine);
		result += header;
		for (int i = 0; i < file_count_; i++)
		{
			if (files_[i].group == group)
			{
				result += files_[i].path;
				result += kNewLine;
			}
		}
		result += kNewLine;
	}
	SetDlgItemText(IDET_RESULT, result);
}
