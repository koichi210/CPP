// ProgressBar_SingleThreadDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "ProgressBar_SingleThread.h"
#include "ProgressBar_SingleThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int PROGRESS_MAX = 100000;
}

CProgressBar_SingleThreadDlg::CProgressBar_SingleThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressBar_SingleThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BEGIN_MESSAGE_MAP(CProgressBar_SingleThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CProgressBar_SingleThreadDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CProgressBar_SingleThreadDlg::OnBnClickedStop)
END_MESSAGE_MAP()

BOOL CProgressBar_SingleThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CProgressBar_SingleThreadDlg::OnPaint()
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

HCURSOR CProgressBar_SingleThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// UI スレッドでループを回すので、終わるまで他のボタンは反応しない（マルチスレッド版との比較用）
void CProgressBar_SingleThreadDlg::OnBnClickedStart()
{
	m_progress.SetRange32(0, PROGRESS_MAX - 1);

	for (int i = 0; i < PROGRESS_MAX; i++)
	{
		m_progress.SetPos(i);
	}
	m_progress.SetPos(0);
}

void CProgressBar_SingleThreadDlg::OnBnClickedStop()
{
	// シングルスレッドなので、処理中には呼ばれない
	MessageBox("処理を中断します。");
}
