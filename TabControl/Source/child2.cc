// child2.cc : タブ（PageB）に表示する子ダイアログ

#include "stdafx.h"
#include "tab_control.h"
#include "child2.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(Child2, CDialogEx)

Child2::Child2(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
}

void Child2::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(Child2, CDialogEx)
END_MESSAGE_MAP()
