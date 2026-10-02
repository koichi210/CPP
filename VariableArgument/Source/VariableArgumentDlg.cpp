// VariableArgumentDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "VariableArgument.h"
#include "VariableArgumentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVariableArgumentDlg::CVariableArgumentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVariableArgumentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Text(pDX, IDC_EDIT_REPLACE, m_strReplace);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CVariableArgumentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC_C, &CVariableArgumentDlg::OnBnClickedButtonExecC)
	ON_BN_CLICKED(IDC_BUTTON_EXE_CPP, &CVariableArgumentDlg::OnBnClickedButtonExeCpp)
END_MESSAGE_MAP()

BOOL CVariableArgumentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_strInput = "Filename_%d.bin";
	m_strReplace = "1";
	UpdateData(FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CVariableArgumentDlg::OnPaint()
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

HCURSOR CVariableArgumentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// C言語風：入力文字列をそのまま書式として sprintf に渡す。
// 比較用にあえて固定長バッファと可変長引数のまま残している
// （%d 以外の書式や長い文字列を入れると壊れるのが C 版の弱点）
void CVariableArgumentDlg::OnBnClickedButtonExecC()
{
	char Input[256] = "";
	char Output[256] = "";
	int Replace = 0;

	UpdateData(TRUE);

	strcpy(Input, m_strInput);
	Replace = atoi(m_strReplace);
	sprintf(Output, Input, Replace);

	m_strOutput = Output;
	UpdateData(FALSE);
}

// C++風：CString の置換で "%d" を値の文字列に置き換える
void CVariableArgumentDlg::OnBnClickedButtonExeCpp()
{
	UpdateData(TRUE);
	m_strOutput = m_strInput;
	m_strOutput.Replace("%d", m_strReplace);
	UpdateData(FALSE);
}
