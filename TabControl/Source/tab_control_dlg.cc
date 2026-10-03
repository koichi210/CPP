// TabControlDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "tab_control.h"
#include "tab_control_dlg.h"
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


CTabControlDlg::CTabControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
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

BOOL CTabControlDlg::OnInitDialog()
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

	m_tab1.InsertItem(0, "Page1");
	m_tab1.InsertItem(1, "Page2");
	m_tab1.InsertItem(2, "Page3");

	m_tab2.InsertItem(0, "PageA");
	m_tab2.InsertItem(1, "PageB");

	// 子ダイアログをタブの子ウィンドウとして作り、タブ見出しを避けた領域に置く
	CRect r;
	m_tab2.GetClientRect(&r);
	r.left += 2;
	r.right -= 4;
	r.top += 20;
	r.bottom -= 4;

	m_child1.Create(CChild1::IDD, &m_tab2);
	m_child1.MoveWindow(&r);

	m_child2.Create(CChild2::IDD, &m_tab2);
	m_child2.MoveWindow(&r);

	m_tab2.SetCurSel(0);
	m_child1.ShowWindow(SW_SHOW);

	return TRUE;
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

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CTabControlDlg::OnPaint()
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

HCURSOR CTabControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTabControlDlg::OnTcnSelchangeTab1(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	int sel = m_tab1.GetCurSel();
	if (0 <= sel && sel <= 2)
	{
		CString text;
		text.Format("Page%d selected", sel + 1);
		SetDlgItemText(IDC_STATIC1, text);
	}

	*pResult = 0;
}

void CTabControlDlg::OnTcnSelchangeTab2(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	switch (m_tab2.GetCurSel())
	{
	case 0:
		m_child2.ShowWindow(SW_HIDE);
		m_child1.ShowWindow(SW_SHOW);
		break;

	case 1:
		m_child1.ShowWindow(SW_HIDE);
		m_child2.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}
