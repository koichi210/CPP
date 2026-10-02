// ProgressBar_MultiThreadDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "ProgressBar_MultiThread.h"
#include "ProgressBar_MultiThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int PROGRESS_MAX = 100000;
}

CProgressBarDlg::CProgressBarDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDST_STATUS_BAR, m_strStatus);
}

BEGIN_MESSAGE_MAP(CProgressBarDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CProgressBarDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CProgressBarDlg::OnBnClickedStop)
END_MESSAGE_MAP()

BOOL CProgressBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_progress.SetRange32(0, PROGRESS_MAX - 1);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CProgressBarDlg::OnPaint()
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

HCURSOR CProgressBarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProgressBarDlg::OnBnClickedStart()
{
	m_stopRequested = false;
	m_strStatus = "Start が押されたよ";
	UpdateData(FALSE);
	AfxBeginThread(ProgressThread, this);
}

void CProgressBarDlg::OnBnClickedStop()
{
	m_stopRequested = true;
	m_strStatus = "Stop が押されたよ";
	UpdateData(FALSE);
}

UINT CProgressBarDlg::ProgressThread(LPVOID pParam)
{
	auto* pDlg = static_cast<CProgressBarDlg*>(pParam);

	for (int i = 0; i < PROGRESS_MAX; i++)
	{
		if (pDlg->m_stopRequested)
		{
			break;
		}
		pDlg->m_progress.SetPos(i);
	}
	return TRUE;
}
