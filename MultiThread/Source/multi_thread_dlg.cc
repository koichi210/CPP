// multi_thread_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "multi_thread.h"
#include "multi_thread_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int kMaxCount = 32767;		// これに達したら 0 に戻す
	constexpr DWORD kIntervalMs = 1000;
	constexpr DWORD kStopCheckMs = 50;		// 待ちの途中でも停止にすぐ気づけるよう、この間隔で確認する
}

MultiThreadDlg::MultiThreadDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(MultiThreadDlg::IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(MultiThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &MultiThreadDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON2, &MultiThreadDlg::OnBnClickedStop)
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()

BOOL MultiThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void MultiThreadDlg::OnPaint()
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

HCURSOR MultiThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// 「Start」：カウントアップするワーカースレッドを起動する（押すたびに1本ずつ増える）
void MultiThreadDlg::OnBnClickedStart()
{
	stop_ = false;
	workers_.Start(CountThreadProc, this);
}

// 「Stop」：動いているワーカースレッドすべてに停止を指示する
void MultiThreadDlg::OnBnClickedStop()
{
	stop_ = true;
}

void MultiThreadDlg::OnOK()
{
	StopWorkers();
	CDialogEx::OnOK();
}

void MultiThreadDlg::OnCancel()
{
	StopWorkers();
	CDialogEx::OnCancel();
}

// シャットダウン・ログオフでは、この後すぐプロセスごと終了させられるので、その前に止める
void MultiThreadDlg::OnEndSession(BOOL ending)
{
	if (ending)
	{
		StopWorkers();
	}
	CDialogEx::OnEndSession(ending);
}

// ワーカーはダイアログを触るので、閉じる（ダイアログが破棄される）前に止めて終了を待つ
void MultiThreadDlg::StopWorkers()
{
	stop_ = true;
	EnableWindow(FALSE);	// 待っている間に Start を押させない
	workers_.WaitAll();
	EnableWindow(TRUE);
}

// 1秒ごとにカウンタを進めてダイアログのタイトルに表示する
UINT MultiThreadDlg::CountThreadProc(LPVOID param)
{
	auto* dlg = static_cast<MultiThreadDlg*>(param);
	int count = 0;
	CString title;

	while (!dlg->stop_)
	{
		count = (count >= kMaxCount) ? 0 : count + 1;

		title.Format(_T("test %04d"), count);
		dlg->SetWindowText(title);

		for (DWORD waited = 0; waited < kIntervalMs && !dlg->stop_; waited += kStopCheckMs)
		{
			Sleep(kStopCheckMs);
		}
	}
	return TRUE;
}
