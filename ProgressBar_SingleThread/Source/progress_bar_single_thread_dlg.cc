// progress_bar_single_thread_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "progress_bar_single_thread.h"
#include "progress_bar_single_thread_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int kProgressMax = 100000;
}

ProgressBarSingleThreadDlg::ProgressBarSingleThreadDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ProgressBarSingleThreadDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Control(dx, IDC_PROGRESS1, progress_);
}

BEGIN_MESSAGE_MAP(ProgressBarSingleThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &ProgressBarSingleThreadDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &ProgressBarSingleThreadDlg::OnBnClickedStop)
END_MESSAGE_MAP()

BOOL ProgressBarSingleThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void ProgressBarSingleThreadDlg::OnPaint()
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

HCURSOR ProgressBarSingleThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// UI スレッドでループを回すので、終わるまで他のボタンは反応しない（マルチスレッド版との比較用）
void ProgressBarSingleThreadDlg::OnBnClickedStart()
{
	progress_.SetRange32(0, kProgressMax - 1);

	for (int i = 0; i < kProgressMax; i++)
	{
		progress_.SetPos(i);
	}
	progress_.SetPos(0);
}

void ProgressBarSingleThreadDlg::OnBnClickedStop()
{
	// シングルスレッドなので、処理中には呼ばれない
	MessageBox("処理を中断します。");
}
