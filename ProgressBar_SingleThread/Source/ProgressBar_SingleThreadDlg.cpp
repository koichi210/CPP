
// ProgressBar_SingleThreadDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ProgressBar_SingleThread.h"
#include "ProgressBar_SingleThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgressBar_SingleThreadDlg ダイアログ




CProgressBar_SingleThreadDlg::CProgressBar_SingleThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressBar_SingleThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressBar_SingleThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ddx_progress);
}

BEGIN_MESSAGE_MAP(CProgressBar_SingleThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CProgressBar_SingleThreadDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CProgressBar_SingleThreadDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CProgressBar_SingleThreadDlg メッセージ ハンドラー

BOOL CProgressBar_SingleThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CProgressBar_SingleThreadDlg::OnPaint()
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
HCURSOR CProgressBar_SingleThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProgressBar_SingleThreadDlg::OnBnClickedStart()
{
	// プログレスバー範囲設定
	m_ddx_progress.SetRange32(0, PROGRESS_MAX -1);

	for (int i = 0; i < PROGRESS_MAX; i++)
	{
		// プログレスバー現在位置設定
		m_ddx_progress.SetPos(i);
	}
	m_ddx_progress.SetPos(0);
}

void CProgressBar_SingleThreadDlg::OnBnClickedStop()
{
	// SingleThreadなので、処理中に呼ばれない。
	MessageBox("処理を中断します。");
}
