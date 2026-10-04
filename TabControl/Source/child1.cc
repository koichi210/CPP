// child1.cc : タブ（PageA）に表示する子ダイアログ

#include "stdafx.h"
#include "tab_control.h"
#include "child1.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(Child1, CDialogEx)

Child1::Child1(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
}

void Child1::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(Child1, CDialogEx)
END_MESSAGE_MAP()
