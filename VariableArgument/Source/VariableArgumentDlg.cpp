
// VariableArgumentDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "VariableArgument.h"
#include "VariableArgumentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVariableArgumentDlg ダイアログ




CVariableArgumentDlg::CVariableArgumentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVariableArgumentDlg::IDD, pParent)
	, m_Input(_T(""))
	, m_Replace(_T(""))
	, m_Output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVariableArgumentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_Input);
	DDX_Text(pDX, IDC_EDIT_REPLACE, m_Replace);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_Output);
}

BEGIN_MESSAGE_MAP(CVariableArgumentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC_C, OnBnClickedButtonExecC)
	ON_BN_CLICKED(IDC_BUTTON_EXE_CPP, OnBnClickedButtonExeCpp)
END_MESSAGE_MAP()


// CVariableArgumentDlg メッセージ ハンドラー

BOOL CVariableArgumentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	m_Input = "Filename_%d.bin";
	m_Replace = "1";
	UpdateData(FALSE);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CVariableArgumentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CVariableArgumentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVariableArgumentDlg::OnBnClickedButtonExecC()
{
	char Input[256] = "";
	char Output[256] = "";
	int Replace = 0;

	UpdateData(TRUE);

	strcpy(Input, m_Input);
	Replace = atoi(m_Replace);
	strcpy(Output, m_Output);
	sprintf(Output, Input, Replace);

	m_Output = Output;
	UpdateData(FALSE);
}

void CVariableArgumentDlg::OnBnClickedButtonExeCpp()
{
	UpdateData(TRUE);
	m_Output = m_Input;
	m_Output.Replace("%d", m_Replace);
	UpdateData(FALSE);
}
