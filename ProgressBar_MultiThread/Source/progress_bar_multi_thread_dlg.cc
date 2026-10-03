// progress_bar_multi_thread_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "progress_bar_multi_thread.h"
#include "progress_bar_multi_thread_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int kProgressMax = 100000;
}

ProgressBarDlg::ProgressBarDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ProgressBarDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Control(dx, IDC_PROGRESS1, progress_);
	DDX_Text(dx, IDST_STATUS_BAR, status_);
}

BEGIN_MESSAGE_MAP(ProgressBarDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &ProgressBarDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &ProgressBarDlg::OnBnClickedStop)
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()

BOOL ProgressBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	progress_.SetRange32(0, kProgressMax - 1);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void ProgressBarDlg::OnPaint()
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

HCURSOR ProgressBarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void ProgressBarDlg::OnBnClickedStart()
{
	stop_requested_ = false;
	status_ = "Start が押されたよ";
	UpdateData(FALSE);
	workers_.Start(ProgressThread, this);
}

void ProgressBarDlg::OnBnClickedStop()
{
	stop_requested_ = true;
	status_ = "Stop が押されたよ";
	UpdateData(FALSE);
}

void ProgressBarDlg::OnOK()
{
	StopWorkers();
	CDialogEx::OnOK();
}

void ProgressBarDlg::OnCancel()
{
	StopWorkers();
	CDialogEx::OnCancel();
}

// シャットダウン・ログオフでは、この後すぐプロセスごと終了させられるので、その前に止める
void ProgressBarDlg::OnEndSession(BOOL ending)
{
	if (ending)
	{
		StopWorkers();
	}
	CDialogEx::OnEndSession(ending);
}

// ワーカーはプログレスバーを触るので、閉じる（ダイアログが破棄される）前に止めて終了を待つ
void ProgressBarDlg::StopWorkers()
{
	stop_requested_ = true;
	EnableWindow(FALSE);	// 待っている間に Start を押させない
	workers_.WaitAll();
	EnableWindow(TRUE);
}

UINT ProgressBarDlg::ProgressThread(LPVOID param)
{
	auto* dlg = static_cast<ProgressBarDlg*>(param);

	for (int i = 0; i < kProgressMax; i++)
	{
		if (dlg->stop_requested_)
		{
			break;
		}
		dlg->progress_.SetPos(i);
	}
	return TRUE;
}
