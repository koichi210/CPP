// template_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "template.h"
#include "template_dlg.h"
#include "afxdialogex.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// 関数テンプレートの確認用
	template <typename T>
	T Add(T x, T y)
	{
		return x + y;
	}
}

TemplateDlg::TemplateDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TemplateDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(TemplateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &TemplateDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL TemplateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void TemplateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int icon_width = GetSystemMetrics(SM_CXICON);
		int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - icon_width + 1) / 2;
		int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR TemplateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void TemplateDlg::OnBnClickedButton1()
{
	// 関数名の後ろに <型> を書いて、テンプレート引数を明示できる
	CString msg;
	msg.AppendFormat("%s + %s = %s\n", "ABC", "def", Add<std::string>("ABC", "def").c_str());	// string を明示的に指定
	msg.AppendFormat("%d + %d = %d\n", 12, 34, Add<int>(12, 34));								// int を明示的に指定
	msg.AppendFormat("%d + %d = %d\n", 5, 6, Add(5, 6));										// 引数から推論できるので省略可能

	MessageBox(msg);
}
