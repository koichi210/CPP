// tab_control_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "tab_control.h"
#include "tab_control_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// システムメニューの「バージョン情報」から開くダイアログ
class AboutDlg : public CDialogEx
{
public:
	AboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;

	DECLARE_MESSAGE_MAP()
};

AboutDlg::AboutDlg() : CDialogEx(IDD)
{
}

void AboutDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialogEx)
END_MESSAGE_MAP()


TabControlDlg::TabControlDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TabControlDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Control(dx, IDC_TAB1, tab1_);
	DDX_Control(dx, IDC_TAB2, tab2_);
}

BEGIN_MESSAGE_MAP(TabControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &TabControlDlg::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &TabControlDlg::OnTcnSelchangeTab2)
END_MESSAGE_MAP()

BOOL TabControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	// （IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* sys_menu = GetSystemMenu(FALSE);
	if (sys_menu != nullptr)
	{
		CString about_menu;
		BOOL name_valid = about_menu.LoadString(IDS_ABOUTBOX);
		ASSERT(name_valid);
		UNREFERENCED_PARAMETER(name_valid);
		if (!about_menu.IsEmpty())
		{
			sys_menu->AppendMenu(MF_SEPARATOR);
			sys_menu->AppendMenu(MF_STRING, IDM_ABOUTBOX, about_menu);
		}
	}

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	tab1_.InsertItem(0, "Page1");
	tab1_.InsertItem(1, "Page2");
	tab1_.InsertItem(2, "Page3");

	tab2_.InsertItem(0, "PageA");
	tab2_.InsertItem(1, "PageB");

	// 子ダイアログをタブの子ウィンドウとして作り、タブ見出しを避けた領域に置く
	CRect r;
	tab2_.GetClientRect(&r);
	r.left += 2;
	r.right -= 4;
	r.top += 20;
	r.bottom -= 4;

	child1_.Create(Child1::IDD, &tab2_);
	child1_.MoveWindow(&r);

	child2_.Create(Child2::IDD, &tab2_);
	child2_.MoveWindow(&r);

	tab2_.SetCurSel(0);
	child1_.ShowWindow(SW_SHOW);

	return TRUE;
}

void TabControlDlg::OnSysCommand(UINT id, LPARAM param)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg about_dlg;
		about_dlg.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(id, param);
	}
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void TabControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int icon_width = GetSystemMetrics(SM_CXICON);
		int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - icon_width + 1) / 2;
		int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR TabControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void TabControlDlg::OnTcnSelchangeTab1(NMHDR* /*nmhdr*/, LRESULT* result)
{
	int sel = tab1_.GetCurSel();
	if (sel >= 0)
	{
		CString text;
		text.Format("Page%d selected", sel + 1);
		SetDlgItemText(IDC_STATIC1, text);
	}

	*result = 0;
}

void TabControlDlg::OnTcnSelchangeTab2(NMHDR* /*nmhdr*/, LRESULT* result)
{
	switch (tab2_.GetCurSel())
	{
	case 0:
		child2_.ShowWindow(SW_HIDE);
		child1_.ShowWindow(SW_SHOW);
		break;

	case 1:
		child1_.ShowWindow(SW_HIDE);
		child2_.ShowWindow(SW_SHOW);
		break;
	}

	*result = 0;
}
