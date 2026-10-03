// pc_hang_up_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "pc_hang_up.h"
#include "pc_hang_up_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ******** ここを有効にするとPCがハングします。 ********
//#define PC_HANG_UP

PCHangUpDlg::PCHangUpDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(PCHangUpDlg::IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(PCHangUpDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &PCHangUpDlg::OnBnClickedHangUp)
END_MESSAGE_MAP()

BOOL PCHangUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void PCHangUpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - icon_width + 1) / 2;
		const int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR PCHangUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void PCHangUpDlg::OnBnClickedHangUp()
{
#ifdef PC_HANG_UP
	MessageBox(_T("PCがハングします"));

	// スレッドを無限に作り続ける（終わるより速く作るので資源を食い尽くす）
	for (;;)
	{
		AfxBeginThread(HangUpThreadProc, this);
	}
#else
	MessageBox(_T("PCをハングさせる場合、下記マクロを有効にしてください\n   PC_HANG_UP"));
#endif
}

UINT PCHangUpDlg::HangUpThreadProc(LPVOID /*param*/)
{
	// ここがスレッドで実行される処理
	// 重い処理を書いたらPC負荷が増大していく
	return TRUE;
}
