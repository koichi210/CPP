
// StandardTemplateLibrarySampleDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StandardTemplateLibrarySample.h"
#include "StandardTemplateLibrarySampleDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStandardTemplateLibrarySampleDlg ダイアログ




CStandardTemplateLibrarySampleDlg::CStandardTemplateLibrarySampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStandardTemplateLibrarySampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStandardTemplateLibrarySampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStandardTemplateLibrarySampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CStandardTemplateLibrarySampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CStandardTemplateLibrarySampleDlg メッセージ ハンドラー

BOOL CStandardTemplateLibrarySampleDlg::OnInitDialog()
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

void CStandardTemplateLibrarySampleDlg::OnPaint()
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
HCURSOR CStandardTemplateLibrarySampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStandardTemplateLibrarySampleDlg::OnBnClickedButton1()
{
	CHAR SrcName[MAX_PATH] = " Sample Test !! ";
	CHAR DestName[MAX_PATH] = "";
	const char* TrimCharList = " ";

	std::string strFileName = SrcName;
	std::string::size_type left = strFileName.find_first_not_of(TrimCharList);
	std::string::size_type right = strFileName.find_last_not_of(TrimCharList);
	std::string strResult = strFileName.substr(left, right - left + 1);
	std::strcpy(DestName, strResult.c_str());  

	CHAR ResultMsg[MAX_PATH] = "";
	sprintf(ResultMsg, "Src[%d]  = %s\nDest[%d]=%s", strlen(SrcName), SrcName, strlen(DestName), DestName);
	MessageBox(ResultMsg, "両端のスペース削除", MB_OK);
}
