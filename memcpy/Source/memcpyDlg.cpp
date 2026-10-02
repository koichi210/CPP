// memcpyDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "memcpy.h"
#include "memcpyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMemcpyDlg::CMemcpyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemcpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMemcpyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMemcpyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CMemcpyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CMemcpyDlg::OnPaint()
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

HCURSOR CMemcpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// コピー元より大きいサイズ（コピー先のサイズ）で memcpy するとどうなるかの確認用。
// str は6バイトしか無いのに256バイト読むので、範囲外読み出し（未定義動作）を承知で残している
void CMemcpyDlg::OnBnClickedButton1()
{
	char buff[256];
	char str[] = "abcde";

	CString Result = "";
	for (int i = 0; i < 100; i++)
	{
		memcpy(buff, str, sizeof(buff));
		Result += buff;
		Result += " ";
	}
	MessageBox(Result);
}
