// PCHangUpDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "PCHangUp.h"
#include "PCHangUpDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ******** ここを有効にするとPCがハングします。 ********
//#define PC_HANG_UP

CPCHangUpDlg::CPCHangUpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CPCHangUpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CPCHangUpDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPCHangUpDlg::OnBnClickedHangUp)
END_MESSAGE_MAP()

BOOL CPCHangUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void CPCHangUpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CPCHangUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPCHangUpDlg::OnBnClickedHangUp()
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

UINT CPCHangUpDlg::HangUpThreadProc(LPVOID /*pParam*/)
{
	// ここがスレッドで実行される処理
	// 重い処理を書いたらPC負荷が増大していく
	return TRUE;
}
