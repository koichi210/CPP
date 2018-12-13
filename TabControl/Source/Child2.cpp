// Child2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TabControl.h"
#include "Child2.h"
#include "afxdialogex.h"


// CChild2 ダイアログ

IMPLEMENT_DYNAMIC(CChild2, CDialogEx)

CChild2::CChild2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChild2::IDD, pParent)
{

}

CChild2::~CChild2()
{
}

void CChild2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChild2, CDialogEx)
END_MESSAGE_MAP()


// CChild2 メッセージ ハンドラー
