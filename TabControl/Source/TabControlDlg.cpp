
// TabControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TabControl.h"
#include "TabControlDlg.h"
#include "afxdialogex.h"

#include "Child1.h"
#include "Child2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTabControlDlg ダイアログ




CTabControlDlg::CTabControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTabControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
	DDX_Control(pDX, IDC_TAB2, m_tab2);
}

BEGIN_MESSAGE_MAP(CTabControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabControlDlg::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CTabControlDlg::OnTcnSelchangeTab2)
END_MESSAGE_MAP()


// CTabControlDlg メッセージ ハンドラー

BOOL CTabControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	m_tab1.InsertItem(0, "Page1");
	m_tab1.InsertItem(1, "Page2");
	m_tab1.InsertItem(3, "Page3");

	m_tab2.InsertItem(0, "PageA");
	m_tab2.InsertItem(1, "PageB");

    // サイズを計算する
    CRect r;
    m_tab2.GetClientRect(&r);
    r.left +=2;
    r.right -=4;
    r.top += 20;
    r.bottom -= 4;

    // ダイアログを、タブの子ウィンドウとして作る
//    m_Child1 = new CChild1(&m_tab2);  
    m_Child1.Create(CChild1::IDD, &m_tab2);
    m_Child1.MoveWindow(&r);

//    m_Child2 = new CChild2(&m_tab2);  
    m_Child2.Create(CChild2::IDD, &m_tab2);
    m_Child2.MoveWindow(&r);

    // タブ0をカレントにし、ダイアログ１を表示する
    m_tab2.SetCurSel(0);
    m_Child1.ShowWindow(SW_SHOW);
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CTabControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CTabControlDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CTabControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTabControlDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	switch (m_tab1.GetCurSel())
	{
	case 0:
		SetDlgItemText(IDC_STATIC1, "Page1 selected");
		break;

	case 1:
		SetDlgItemText(IDC_STATIC1, "Page2 selected");
		break;

	case 2:
		SetDlgItemText(IDC_STATIC1, "Page3 selected");
		break;
	}

	*pResult = 0;
}


void CTabControlDlg::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	switch(m_tab2.GetCurSel())
	{
	case 0:
	    m_Child2.ShowWindow(SW_HIDE);
	    m_Child1.ShowWindow(SW_SHOW);
		break;

	case 1:
	    m_Child1.ShowWindow(SW_HIDE);
	    m_Child2.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}
