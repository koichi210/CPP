// signed_unsigned_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "signed_unsigned.h"
#include "signed_unsigned_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSignedUnsignedDlg::CSignedUnsignedDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignedUnsignedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSignedUnsignedDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSignedUnsignedDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CSignedUnsignedDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CSignedUnsignedDlg::OnPaint()
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

HCURSOR CSignedUnsignedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 符号なし同士の引き算が負になる（ラップアラウンドする）と、その後の乗除算の結果が
// 期待した負の値にならないことを確かめる。結果はわざと %d（符号付き）で表示している
void CSignedUnsignedDlg::OnBnClickedButton1()
{
	UINT32	BitDepthValue		= 8;
	UINT32	YOffset				= 1;
	UINT32	BaseWidth			= 1;
	UINT32	XOffset				= 1;
	UINT32	Compass_Hcopy_num	= 18;
	UINT32	Pix2ByteDen			= 1;

	UINT32	CompIn1Offset = (( YOffset * BaseWidth + XOffset ) - Compass_Hcopy_num ) * BitDepthValue / Pix2ByteDen;
	CString str;
	str.Format("Result = %d", CompIn1Offset );
	MessageBox(str);
}
