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

CFileCompareDlg::CFileCompareDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
	, m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
	, m_running(false)
	, m_files(kMaxFileCount)
	, m_fileCount(0)
	, m_nextGroup(1)
{
}

void CFileCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDPR_COMPARE, m_progress);
}

BEGIN_MESSAGE_MAP(CFileCompareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_EXECUTE, &CFileCompareDlg::OnBnClickedExecute)
END_MESSAGE_MAP()

BOOL CFileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetDlgItemText(IDBT_EXECUTE, kStartLabel);
	SetDlgItemText(IDET_PATH, kDefaultPath);
	CEdit* pPathEdit = static_cast<CEdit*>(GetDlgItem(IDET_PATH));
	pPathEdit->SetFocus();
	pPathEdit->SetSel(0, -1);
	return FALSE;	// フォーカスを自分で設定したので FALSE
}

// 最小化時のアイコン描画（ダイアログアプリでは自前で描く必要がある）
void CFileCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CFileCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 比較中なら停止、そうでなければ比較を開始する
void CFileCompareDlg::OnBnClickedExecute()
{
	if (m_running)
	{
		StopCompare();
		return;
	}
	StartCompare();
}

void CFileCompareDlg::StartCompare()
{
	m_fileCount = 0;
	m_nextGroup = 1;
	m_running = true;
	SetDlgItemText(IDBT_EXECUTE, kStopLabel);
	SetDlgItemText(IDET_RESULT, _T(""));

	CollectFiles(GetFolders());

	// 総当たりの進捗。表示上の位置は「行 * ファイル数 + 列」で数える
	m_progress.SetRange32(0, GetProgressEnd());
	UpdateProgress(0, 0);
	AfxBeginThread(CompareThread, this);
}

void CFileCompareDlg::StopCompare()
{
	SetDlgItemText(IDBT_EXECUTE, kStartLabel);
	m_running = false;
}

std::vector<CString> CFileCompareDlg::GetFolders() const
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
void CFileCompareDlg::CollectFiles(const std::vector<CString>& folders)
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
			if (m_fileCount >= kMaxFileCount)
			{
				break;
			}
			m_files[m_fileCount].path = finder.GetFilePath();
			m_files[m_fileCount].group = kNoGroup;
			m_fileCount++;
		}
	}
}

UINT CFileCompareDlg::CompareThread(LPVOID pParam)
{
	static_cast<CFileCompareDlg*>(pParam)->CompareFiles();
	return 0;
}

// 総当たりで比較し、同じ内容のファイルに同じ組番号を付ける
void CFileCompareDlg::CompareFiles()
{
	FileComparer comparer;
	const int end = GetProgressEnd();

	for (int i = 0; i < m_fileCount && m_running; i++)
	{
		UpdateProgress(i * m_fileCount, end);
		if (m_files[i].group != kNoGroup)
		{
			continue;	// すでにどこかの組に入っている
		}

		for (int j = i + 1; j < m_fileCount && m_running; j++)
		{
			UpdateProgress(i * m_fileCount + j, end);
			if (m_files[j].group != kNoGroup)
			{
				continue;
			}
			if (comparer.CompareBinary(m_files[i].path, m_files[j].path))
			{
				m_files[i].group = m_nextGroup;
				m_files[j].group = m_nextGroup;
			}
		}

		if (m_files[i].group != kNoGroup)
		{
			m_nextGroup++;
		}
	}

	StopCompare();
	ShowResult();
}

int CFileCompareDlg::GetProgressEnd() const
{
	return m_fileCount * (m_fileCount - 1);
}

void CFileCompareDlg::UpdateProgress(int pos, int end)
{
	m_progress.SetPos(pos);

	CString text;
	text.Format(_T("(%d / %d)"), pos, end);
	SetDlgItemText(IDST_COMPARE, text);
}

void CFileCompareDlg::ShowResult()
{
	CString result;
	for (int group = 1; group < m_nextGroup; group++)
	{
		CString header;
		header.Format(_T("[Group%d]%s"), group, kNewLine);
		result += header;
		for (int i = 0; i < m_fileCount; i++)
		{
			if (m_files[i].group == group)
			{
				result += m_files[i].path;
				result += kNewLine;
			}
		}
		result += kNewLine;
	}
	SetDlgItemText(IDET_RESULT, result);
}
