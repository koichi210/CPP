
// MultiThreadDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MultiThread.h"
#include "MultiThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiThreadDlg ダイアログ




CMultiThreadDlg::CMultiThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMultiThreadDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMultiThreadDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMultiThreadDlg メッセージ ハンドラー

BOOL CMultiThreadDlg::OnInitDialog()
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

void CMultiThreadDlg::OnPaint()
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
HCURSOR CMultiThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMultiThreadDlg::OnBnClickedButton1()
{
	m_stop=FALSE;
	AfxBeginThread(ProcThread, this);
}


void CMultiThreadDlg::OnBnClickedButton2()
{
	m_stop=TRUE;
}


UINT ProcThread(LPVOID pParam)
{
	CMultiThreadDlg* pDlg=(CMultiThreadDlg*)pParam;
	int cnt=0;
	CString str;

	while(TRUE)
	{
		if ( pDlg->m_stop )
		{
			break;
		}

		if ( cnt >= 32767 )
		{
			cnt = 0;
		}
		else
		{
			cnt++;
		}

		str.Format("test %04d", cnt);
		pDlg->SetWindowText(str);
		Sleep(1000);
	}
	return TRUE;
}
