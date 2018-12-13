
// ClipboardDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Clipboard.h"
#include "ClipboardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClipboardDlg ダイアログ




CClipboardDlg::CClipboardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClipboardDlg::IDD, pParent)
	, m_Text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClipboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_TEXT, m_Text);
}

BEGIN_MESSAGE_MAP(CClipboardDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_COPY_CLIPBOARD, &CClipboardDlg::OnBnClickedCopyClipboard)
END_MESSAGE_MAP()


// CClipboardDlg メッセージ ハンドラー

BOOL CClipboardDlg::OnInitDialog()
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

void CClipboardDlg::OnPaint()
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
HCURSOR CClipboardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClipboardDlg::OnBnClickedCopyClipboard()
{
	UpdateData(TRUE);
	if ( ! SetClipboardText(m_Text))
	{
		MessageBox("エラーが発生しました");
	}
}

BOOL CClipboardDlg::SetClipboardText( const CHAR *Str )
{
	BOOL IsSuccess = TRUE;

	try
	{
		// 移動可能な共有メモリを確保
		int BufSize = strlen( Str ) + 1;
		HANDLE hMem = GlobalAlloc( GMEM_SHARE | GMEM_MOVEABLE, BufSize );
		if ( !hMem )
		{
			throw  FALSE;
		}

		// 確保したメモリをロックし，アクセス可能にする
		char  *Buf = (char *)GlobalLock( hMem );
		if ( !Buf )
		{
			throw  FALSE;
		}

		strcpy_s( Buf, BufSize, m_Text );	// 文字列をセット
		GlobalUnlock( hMem );				// メモリのロックを解除
		if ( OpenClipboard() )
		{
			EmptyClipboard();                  // クリップボード内の古いデータを解放
			SetClipboardData( CF_TEXT, hMem ); // クリップボードに新しいデータを入力
			CloseClipboard();
		}
	}
	catch(BOOL Result)
	{
		IsSuccess = Result;
	}
	return IsSuccess;
}