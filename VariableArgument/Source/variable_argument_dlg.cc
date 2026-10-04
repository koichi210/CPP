// variable_argument_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "variable_argument.h"
#include "variable_argument_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

VariableArgumentDlg::VariableArgumentDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void VariableArgumentDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Text(dx, IDC_EDIT_INPUT, input_);
	DDX_Text(dx, IDC_EDIT_REPLACE, replace_);
	DDX_Text(dx, IDC_EDIT_OUTPUT, output_);
}

BEGIN_MESSAGE_MAP(VariableArgumentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC_C, &VariableArgumentDlg::OnBnClickedButtonExecC)
	ON_BN_CLICKED(IDC_BUTTON_EXE_CPP, &VariableArgumentDlg::OnBnClickedButtonExeCpp)
END_MESSAGE_MAP()

BOOL VariableArgumentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	input_ = "Filename_%d.bin";
	replace_ = "1";
	UpdateData(FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void VariableArgumentDlg::OnPaint()
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

HCURSOR VariableArgumentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// C言語風：入力文字列をそのまま書式として sprintf に渡す。
// 比較用にあえて固定長バッファと可変長引数のまま残している
// （%d 以外の書式や長い文字列を入れると壊れるのが C 版の弱点）
void VariableArgumentDlg::OnBnClickedButtonExecC()
{
	UpdateData(TRUE);

	char input[256] = "";
	char output[256] = "";
	strcpy(input, input_);
	const int replace = atoi(replace_);
	sprintf(output, input, replace);

	output_ = output;
	UpdateData(FALSE);
}

// C++風：CString の置換で "%d" を値の文字列に置き換える
void VariableArgumentDlg::OnBnClickedButtonExeCpp()
{
	UpdateData(TRUE);
	output_ = input_;
	output_.Replace("%d", replace_);
	UpdateData(FALSE);
}
