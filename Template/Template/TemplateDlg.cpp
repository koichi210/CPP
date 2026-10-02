// TemplateDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "Template.h"
#include "TemplateDlg.h"
#include "afxdialogex.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// 関数テンプレートの確認用
	template <typename T>
	T add(T x, T y)
	{
		return x + y;
	}
}

CTemplateDlg::CTemplateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
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
	ON_BN_CLICKED(IDC_BUTTON1, &CTemplateDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CTemplateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CTemplateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CTemplateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTemplateDlg::OnBnClickedButton1()
{
	// 関数名の後ろに <型> を書いて、テンプレート引数を明示できる
	CString Msg;
	Msg.AppendFormat("%s + %s = %s\n", "ABC", "def", add<std::string>("ABC", "def").c_str());	// string を明示的に指定
	Msg.AppendFormat("%d + %d = %d\n", 12, 34, add<int>(12, 34));								// int を明示的に指定
	Msg.AppendFormat("%d + %d = %d\n", 5, 6, add(5, 6));										// 引数から推論できるので省略可能

	MessageBox(Msg);
}
