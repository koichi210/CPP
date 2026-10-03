// SplitPathOwnDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "split_path_own.h"
#include "split_path_own_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// システムメニューの「バージョン情報」から開くダイアログ
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CSplitPathOwnDlg::CSplitPathOwnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSplitPathOwnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplitPathOwnDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSplitPathOwnDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CSplitPathOwnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	// （IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		UNREFERENCED_PARAMETER(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

void CSplitPathOwnDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CSplitPathOwnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CSplitPathOwnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSplitPathOwnDlg::OnBnClickedButton1()
{
	char FileFullPath[256] = "C:/Windows/System32/explorer.exe";
	// strncpy は終端の '\0' を付けないので、出力先は0で埋めておく
	char drive[256] = "";
	char dir[256] = "";
	char fname[256] = "";
	SplitPath(FileFullPath, drive, dir, fname);

	CString Result;
	Result.Format("org=%s\n\n drv=%s\n dir=%s\n fname=%s",
			FileFullPath,
			drive,
			dir,
			fname
			);
	MessageBox(Result);
}

void CSplitPathOwnDlg::SplitPath(const char* pFileFullPath, char* pDrive, char* pDir, char* pFile)
{
	const char* pt;

	/* ドライブ名取得 */
	{
		pt = pFileFullPath;	/* ポインタを先頭に移動*/
		while(*pt != '/')
		{
			pt++;
		}
		strncpy(pDrive, pFileFullPath, strlen(pFileFullPath) - strlen(pt) + 1);	// 「+1」は終端の「/」を追加
	}

	/* ファイル名取得 */
	{
		pt = pFileFullPath + strlen(pFileFullPath);	/* ポインタを終端に移動 */
		while(*pt != '/')
		{
			pt--;
		}
		pt++;	// 先頭の「/」を削除
		strncpy(pFile, pt, strlen(pt));
	}

	/* ディレクトリ名取得 */
	{
		pt = &pFileFullPath[strlen(pDrive)];	/* ポインタをドライブレターの後ろに移動 */
		strncpy(pDir, pt, strlen(pt) - strlen(pFile));
	}
}
