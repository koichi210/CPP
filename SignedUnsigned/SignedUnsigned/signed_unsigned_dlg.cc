// signed_unsigned_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "signed_unsigned.h"
#include "signed_unsigned_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SignedUnsignedDlg::SignedUnsignedDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SignedUnsignedDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(SignedUnsignedDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &SignedUnsignedDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL SignedUnsignedDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void SignedUnsignedDlg::OnPaint()
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

HCURSOR SignedUnsignedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// 符号なし同士の引き算が負になる（ラップアラウンドする）と、その後の乗除算の結果が
// 期待した負の値にならないことを確かめる。結果はわざと %d（符号付き）で表示している
void SignedUnsignedDlg::OnBnClickedButton1()
{
	UINT32	bit_depth_value		= 8;
	UINT32	y_offset				= 1;
	UINT32	base_width			= 1;
	UINT32	x_offset				= 1;
	UINT32	compass_hcopy_num	= 18;
	UINT32	pix2_byte_den			= 1;

	UINT32	comp_in1_offset = (( y_offset * base_width + x_offset ) - compass_hcopy_num ) * bit_depth_value / pix2_byte_den;
	CString str;
	str.Format("Result = %d", comp_in1_offset );
	MessageBox(str);
}
