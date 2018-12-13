
// ProgressBar_MultiThreadDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ProgressBar_MultiThread.h"
#include "ProgressBar_MultiThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgressBarDlg ダイアログ




CProgressBarDlg::CProgressBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressBarDlg::IDD, pParent)
	, m_StatusBar(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ddx_progress);
	DDX_Text(pDX, IDST_STATUS_BAR, m_StatusBar);
}

BEGIN_MESSAGE_MAP(CProgressBarDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CProgressBarDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CProgressBarDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CProgressBarDlg メッセージ ハンドラー

BOOL CProgressBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// プログレスバー範囲設定
	m_ddx_progress.SetRange32(0, PROGRESS_MAX -1);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CProgressBarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CProgressBarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProgressBarDlg::OnBnClickedStart()
{
	m_stop = FALSE;
	m_StatusBar.SetString("Start が押されたよ");
	UpdateData(false);
	AfxBeginThread(ProcThread, this);
}

void CProgressBarDlg::OnBnClickedStop()
{
	m_stop = TRUE;
	m_StatusBar.SetString("Stop が押されたよ");
	UpdateData(false);
}

UINT ProcThread(LPVOID pParam)
{
	CProgressBarDlg* pDlg = (CProgressBarDlg*)pParam;
	int cnt=0;
	CString str;

	for (int i = 0; i < PROGRESS_MAX; i++)
	{
		if ( pDlg->m_stop )
		{
			break;
		}
		// プログレスバー現在位置設定
		pDlg->m_ddx_progress.SetPos(i);
	}
//	pDlg->m_ddx_progress.SetPos(0);
	return TRUE;
}
