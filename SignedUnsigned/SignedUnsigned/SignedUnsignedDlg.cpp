
// SignedUnsignedDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SignedUnsigned.h"
#include "SignedUnsignedDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSignedUnsignedDlg ダイアログ




CSignedUnsignedDlg::CSignedUnsignedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignedUnsignedDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignedUnsignedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSignedUnsignedDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSignedUnsignedDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSignedUnsignedDlg メッセージ ハンドラー

BOOL CSignedUnsignedDlg::OnInitDialog()
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

void CSignedUnsignedDlg::OnPaint()
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
HCURSOR CSignedUnsignedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSignedUnsignedDlg::OnBnClickedButton1()
{
	UINT32	BitDepthValue		= 8;
	UINT32	YOffset				= 1;
	UINT32	BaseWidth			= 1;
	UINT32	XOffset				= 1;
	UINT32	Compass_Hcopy_num	= 18;
	UINT32	Pix2ByteDen			= 1;

	UINT32	CompIn1Offset = (( YOffset * BaseWidth + XOffset ) - Compass_Hcopy_num ) * BitDepthValue / Pix2ByteDen;
	CString str;
	str.Format("Result = %d", CompIn1Offset );
	MessageBox(str);
}
