// JointMovieDlg.cpp : 実装ファイル
//

#include <afxtempl.h>
#include "stdafx.h"
#include "JointMovie.h"
#include "JointMovieDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CJointMovieDlg ダイアログ


CJointMovieDlg::CJointMovieDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJointMovieDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJointMovieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJointMovieDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OUT_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN1_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN2_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN3_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN4_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN5_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN6_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN7_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN8_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_EXECUTE, OnExecute)
END_MESSAGE_MAP()


// CJointMovieDlg メッセージ ハンドラ

BOOL CJointMovieDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CJointMovieDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CJointMovieDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CJointMovieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CJointMovieDlg::OnBrowse()
{
	const MSG *pMsg = GetCurrentMessage();

	if ( pMsg )
	{
		CString	strFileType;
		char	szFileNames[MAX_PATH]="\0";
		int		idCtrl = LOWORD(pMsg->wParam);
		int		idxCtrl = idCtrl - IDC_IN1_BROWSE;	// オフセット取得

		idxCtrl += IDC_INPUTFILE1;	// 設定するエディットコントロールIDを算出

		strFileType.Format("動画（*.mpg; *.mpeg;）|*.mpg; *.mpeg;|すべてのﾌｧｲﾙ （*.*）|*.*||");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFileType, this);
		dlg.GetOFN().lpstrFile = szFileNames;
		dlg.GetOFN().nMaxFile = sizeof(szFileNames) / sizeof(char);
		if ( dlg.DoModal() == IDOK )
		{
			SetDlgItemText(idxCtrl, szFileNames);
		}
	}
}


void CJointMovieDlg::OnExecute()
{
	CString org_fname;
	CString new_fname;
	CString tmp;
	BOOL rt = FALSE;

	GetDlgItem(IDC_OUTPUTFILE)->GetWindowText(new_fname);
	for(int i=0;i < GROUP_MAX;i++)
	{
		GetDlgItem(IDC_INPUTFILE1 + i)->GetWindowText(tmp);
		if ( tmp != "" )
		{
			if ( org_fname != "" )
			{
				org_fname += "+";
			}
			org_fname += tmp;
		}
	}

	if ( new_fname == "" )
	{
		MessageBox("結合先ファイル名が不正");
	}
	else if ( org_fname == "" )
	{
		MessageBox("元ファイル名が不正");
	}
	else
	{
		char command[MAX_PATH];

		sprintf_s(command, sizeof(command),"copy /B /-Y %s %s",org_fname,new_fname);
		rt = system(command);
	}
}
