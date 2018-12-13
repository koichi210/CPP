// Child1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TabControl.h"
#include "Child1.h"
#include "afxdialogex.h"


// CChild1 ダイアログ

IMPLEMENT_DYNAMIC(CChild1, CDialogEx)

CChild1::CChild1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChild1::IDD, pParent)
{

}

CChild1::~CChild1()
{
}

void CChild1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChild1, CDialogEx)
END_MESSAGE_MAP()


// CChild1 メッセージ ハンドラー
