// json_if_dlg.cc : メインダイアログ

#include "pch.h"
#include "framework.h"
#include "json_if.h"
#include "json_if_dlg.h"
#include "afxdialogex.h"
#include "External/picojson.h"
#include "External/rapidjson.h"
#include "External/json.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JsonIFDlg::JsonIFDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD_JSONIF_DIALOG, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(JsonIFDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IBT_PICOJSON, &JsonIFDlg::OnBnClickedPicojson)
	ON_BN_CLICKED(IBT_RAPIDJSON, &JsonIFDlg::OnBnClickedRapidjson)
	ON_BN_CLICKED(IBT_NLOHMANNJSON, &JsonIFDlg::OnBnClickedNlohmannjson)
END_MESSAGE_MAP()

BOOL JsonIFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void JsonIFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - icon_width + 1) / 2;
		const int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR JsonIFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void JsonIFDlg::OnBnClickedPicojson()
{
	// 未実装
}

void JsonIFDlg::OnBnClickedRapidjson()
{
	// 未実装
}

void JsonIFDlg::OnBnClickedNlohmannjson()
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
