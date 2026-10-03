// json_if_dlg.cc : メインダイアログ

#include "pch.h"
#include "framework.h"
#include "json_if.h"
#include "json_if_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CJsonIFDlg::CJsonIFDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JSONIF_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CJsonIFDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IBT_PICOJSON, &CJsonIFDlg::OnBnClickedPicojson)
	ON_BN_CLICKED(IBT_RAPIDJSON, &CJsonIFDlg::OnBnClickedRapidjson)
	ON_BN_CLICKED(IBT_NLOMANNJSON, &CJsonIFDlg::OnBnClickedNlomannjson)
END_MESSAGE_MAP()

BOOL CJsonIFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void CJsonIFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CJsonIFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJsonIFDlg::OnBnClickedPicojson()
{
	// 未実装
}

void CJsonIFDlg::OnBnClickedRapidjson()
{
	// 未実装
}

void CJsonIFDlg::OnBnClickedNlomannjson()
{
	// お試し用なので、JSON 操作の例外で落ちないよう内容を表示して止める
	try
	{
		nlohmann::json json_data;

		// 文字列を入れた要素に子要素は付けられない（type_error）ので、"top" はオブジェクトにする
		json_data["top"]["value"] = "TOP_LAYER";
		json_data["top"]["sub"] = "SUB_LAYER";

		// dump() は UTF-8 の std::string。文字セットに依らずビルドできるよう CString に変換して渡す
		MessageBox(CString(json_data.dump(2).c_str()), _T("nlohmann::json"));
	}
	catch (const nlohmann::json::exception& e)
	{
		MessageBox(CString(e.what()), _T("nlohmann::json エラー"), MB_ICONERROR);
	}
}
