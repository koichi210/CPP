// Child2.cpp : タブ（PageB）に表示する子ダイアログ

#include "stdafx.h"
#include "tab_control.h"
#include "child2.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CChild2, CDialogEx)

CChild2::CChild2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
}

void CChild2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChild2, CDialogEx)
END_MESSAGE_MAP()
