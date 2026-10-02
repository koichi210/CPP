// StandardTemplateLibrarySampleDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "StandardTemplateLibrarySample.h"
#include "StandardTemplateLibrarySampleDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStandardTemplateLibrarySampleDlg::CStandardTemplateLibrarySampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStandardTemplateLibrarySampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStandardTemplateLibrarySampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CStandardTemplateLibrarySampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CStandardTemplateLibrarySampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CStandardTemplateLibrarySampleDlg::OnPaint()
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

HCURSOR CStandardTemplateLibrarySampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// std::string の find_first_not_of / find_last_not_of で両端の空白を取り除く
void CStandardTemplateLibrarySampleDlg::OnBnClickedButton1()
{
	const char* SrcName = " Sample Test !! ";
	const char* TrimCharList = " ";

	std::string strFileName = SrcName;
	std::string::size_type left = strFileName.find_first_not_of(TrimCharList);
	std::string::size_type right = strFileName.find_last_not_of(TrimCharList);
	std::string strResult = strFileName.substr(left, right - left + 1);

	CString ResultMsg;
	ResultMsg.Format("Src[%d]  = %s\nDest[%d]=%s",
		static_cast<int>(strFileName.size()), SrcName,
		static_cast<int>(strResult.size()), strResult.c_str());
	MessageBox(ResultMsg, "両端のスペース削除", MB_OK);
}
