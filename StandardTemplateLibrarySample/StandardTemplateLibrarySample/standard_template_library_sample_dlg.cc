// standard_template_library_sample_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "standard_template_library_sample.h"
#include "standard_template_library_sample_dlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

StandardTemplateLibrarySampleDlg::StandardTemplateLibrarySampleDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void StandardTemplateLibrarySampleDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(StandardTemplateLibrarySampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &StandardTemplateLibrarySampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL StandardTemplateLibrarySampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void StandardTemplateLibrarySampleDlg::OnPaint()
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

HCURSOR StandardTemplateLibrarySampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// std::string の find_first_not_of / find_last_not_of で両端の空白を取り除く
void StandardTemplateLibrarySampleDlg::OnBnClickedButton1()
{
	const char* src_name = " Sample Test !! ";
	const char* trim_char_list = " ";

	std::string file_name = src_name;
	std::string::size_type left = file_name.find_first_not_of(trim_char_list);
	std::string::size_type right = file_name.find_last_not_of(trim_char_list);
	std::string result = file_name.substr(left, right - left + 1);

	CString result_msg;
	result_msg.Format("Src[%d]  = %s\nDest[%d]=%s",
		static_cast<int>(file_name.size()), src_name,
		static_cast<int>(result.size()), result.c_str());
	MessageBox(result_msg, "両端のスペース削除", MB_OK);
}
