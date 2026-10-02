// JsonIFDlg.h : メインダイアログ（JSON ライブラリの使い方を試す）

#pragma once

#include "External/picojson.h"
#include "External/rapidjson.h"
#include "External/json.hpp"

class CJsonIFDlg : public CDialogEx
{
public:
	explicit CJsonIFDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JSONIF_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedPicojson();
	afx_msg void OnBnClickedRapidjson();
	afx_msg void OnBnClickedNlomannjson();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};
