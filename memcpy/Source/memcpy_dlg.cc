// memcpy_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "memcpy.h"
#include "memcpy_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MemcpyDlg::MemcpyDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(MemcpyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &MemcpyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL MemcpyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void MemcpyDlg::OnPaint()
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

HCURSOR MemcpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// コピー元より大きいサイズ（コピー先のサイズ）で memcpy するとどうなるかの確認用。
// str は6バイトしか無いのに256バイト読むので、範囲外読み出し（未定義動作）を承知で残している
void MemcpyDlg::OnBnClickedButton1()
{
	char buff[256];
	char str[] = "abcde";

	memcpy(buff, str, sizeof(buff));

	// コピー結果を100回並べて表示する（毎回コピーし直す必要は無いのでループの外で1回だけ）
	CString result;
	for (int i = 0; i < 100; i++)
	{
		result += buff;
		result += " ";
	}
	MessageBox(result);
}
