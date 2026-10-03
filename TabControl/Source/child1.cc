// Child1.cpp : タブ（PageA）に表示する子ダイアログ

#include "stdafx.h"
#include "tab_control.h"
#include "child1.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CChild1, CDialogEx)

CChild1::CChild1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
}

void CChild1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChild1, CDialogEx)
END_MESSAGE_MAP()
