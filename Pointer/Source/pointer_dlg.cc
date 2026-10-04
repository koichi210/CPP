// pointer_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "pointer.h"
#include "pointer_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PointerDlg::PointerDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
	, test_{}
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(PointerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &PointerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL PointerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	test_.param1 = 1;
	test_.param2 = 20;
	test_.param3 = 300;

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void PointerDlg::OnPaint()
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

HCURSOR PointerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// どちらの受け取り方でも test_ 本体を指すこと、
// ポインタ変数自体は別々の場所にあることを表示して確かめる
void PointerDlg::OnBnClickedButton1()
{
	TestData* test = nullptr;
	GetParam(&test);
	TestData* test2 = GetParam();

	// アドレスは %p で表示する（64bit ではポインタが8バイトなので、%08x だと後ろの値までずれる）
	CString addr_str;
	addr_str.Format(
		"Addr  test_本体\t\t = %p\n"
		"Addr  testが指す先\t = %p\n"
		"Addr  test2が指す先\t = %p\n"
		"Addr  test本体\t\t = %p\n"
		"Addr  test2本体\t\t = %p\n\n"

		"Data  test_.param2\t = %08x\n"
		"Data  test->param2\t = %08x\n"
		"Data  test2->param2\t = %08x\n" ,
		static_cast<void*>(&test_),
		static_cast<void*>(test),
		static_cast<void*>(test2),

		static_cast<void*>(&test),
		static_cast<void*>(&test2),

		test_.param2,
		test->param2,
		test2->param2);

	MessageBox(addr_str);
}

void PointerDlg::GetParam(TestData** pp)
{
	*pp = &test_;
}

TestData* PointerDlg::GetParam()
{
	return &test_;
}
