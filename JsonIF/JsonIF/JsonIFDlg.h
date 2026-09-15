
// JsonIFDlg.h : ヘッダー ファイル
//

#pragma once

#include "External/picojson.h"
#include "External/rapidjson.h"
#include "External/json.hpp"

// CJsonIFDlg ダイアログ
class CJsonIFDlg : public CDialogEx
{
// コンストラクション
public:
	CJsonIFDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JSONIF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPicojson();
	afx_msg void OnBnClickedRapidjson();
	afx_msg void OnBnClickedNlomannjson();
};
