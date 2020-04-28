
// TemplateDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Template.h"
#include "TemplateDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;
 
//  テンプレート関数
template <typename T>
T add(T x, T y){
    return x + y;
}

// CTemplateDlg ダイアログ
CTemplateDlg::CTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTemplateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTemplateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CTemplateDlg メッセージ ハンドラー

BOOL CTemplateDlg::OnInitDialog()
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

void CTemplateDlg::OnPaint()
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
HCURSOR CTemplateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTemplateDlg::OnBnClickedButton1()
{
	// 関数の後ろに型を記載
	char Msg[MAX_PATH] = "";
	char AddStr[MAX_PATH] = "";

	sprintf(AddStr, "%s + %s = %s\n", "ABC", "def", add<string>("ABC", "def"));	// stringを明示的に指定
	sprintf(Msg, "%s%s", Msg, AddStr);

	sprintf(AddStr, "%d + %d = %d\n", 12, 34, add<int>(12, 34));				// intを明示的に指定
	sprintf(Msg, "%s%s", Msg, AddStr);

	sprintf(AddStr, "%d + %d = %d\n", 5, 6, add(5, 6));							// intの場合、指定省略可能
	sprintf(Msg, "%s%s", Msg, AddStr);

	MessageBox(Msg);
}
