// MultiThreadDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "MultiThread.h"
#include "MultiThreadDlg.h"
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

CMultiThreadDlg::CMultiThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CMultiThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CMultiThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMultiThreadDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CMultiThreadDlg::OnBnClickedStop)
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()

BOOL CMultiThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void CMultiThreadDlg::OnPaint()
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

HCURSOR CMultiThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 「Start」：カウントアップするワーカースレッドを起動する（押すたびに1本ずつ増える）
void CMultiThreadDlg::OnBnClickedStart()
{
	m_bStop = false;
	m_workers.Start(CountThreadProc, this);
}

// 「Stop」：動いているワーカースレッドすべてに停止を指示する
void CMultiThreadDlg::OnBnClickedStop()
{
	m_bStop = true;
}

void CMultiThreadDlg::OnOK()
{
	StopWorkers();
	CDialogEx::OnOK();
}

void CMultiThreadDlg::OnCancel()
{
	StopWorkers();
	CDialogEx::OnCancel();
}

// シャットダウン・ログオフでは、この後すぐプロセスごと終了させられるので、その前に止める
void CMultiThreadDlg::OnEndSession(BOOL bEnding)
{
	if (bEnding)
	{
		StopWorkers();
	}
	CDialogEx::OnEndSession(bEnding);
}

// ワーカーはダイアログを触るので、閉じる（ダイアログが破棄される）前に止めて終了を待つ
void CMultiThreadDlg::StopWorkers()
{
	m_bStop = true;
	EnableWindow(FALSE);	// 待っている間に Start を押させない
	m_workers.WaitAll();
	EnableWindow(TRUE);
}

// 1秒ごとにカウンタを進めてダイアログのタイトルに表示する
UINT CMultiThreadDlg::CountThreadProc(LPVOID pParam)
{
	auto* pDlg = static_cast<CMultiThreadDlg*>(pParam);
	int count = 0;
	CString title;

	while (!pDlg->m_bStop)
	{
		count = (count >= kMaxCount) ? 0 : count + 1;

		title.Format(_T("test %04d"), count);
		pDlg->SetWindowText(title);

		for (DWORD waited = 0; waited < kIntervalMs && !pDlg->m_bStop; waited += kStopCheckMs)
		{
			Sleep(kStopCheckMs);
		}
	}
	return TRUE;
}
