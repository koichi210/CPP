// pointer_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "pointer.h"
#include "pointer_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPointerDlg::CPointerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
	, m_test{}
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPointerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPointerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPointerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CPointerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_test.param1 = 1;
	m_test.param2 = 20;
	m_test.param3 = 300;

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CPointerDlg::OnPaint()
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

HCURSOR CPointerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// どちらの受け取り方でも m_test 本体を指すこと、
// ポインタ変数自体は別々の場所にあることを表示して確かめる
void CPointerDlg::OnBnClickedButton1()
{
	TEST_T*	pTest = nullptr;
	TEST_T*	pTest2;

	GetParam(&pTest);
	pTest2 = GetParam();

	CString AddrStr;
	AddrStr.Format(
		"Addr  m_test本体\t\t = %08x\n"
		"Addr  pTestが指す先\t = %08x\n"
		"Addr  pTest2が指す先\t = %08x\n"
		"Addr  pTest本体\t\t = %08x\n"
		"Addr  pTest2本体\t\t = %08x\n\n"

		"Data  m_test.param2\t = %08x\n"
		"Data  pTest->param2\t = %08x\n"
		"Data  pTest2->param2\t = %08x\n" ,
		&m_test,
		pTest,
		pTest2,

		&pTest,
		&pTest2,

		m_test.param2,
		pTest->param2,
		pTest2->param2);

	MessageBox(AddrStr);
}

void CPointerDlg::GetParam(TEST_T** pp)
{
	*pp = &m_test;
}

TEST_T* CPointerDlg::GetParam()
{
	return &m_test;
}
