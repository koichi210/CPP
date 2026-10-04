// json_if_dlg.h : メインダイアログ（JSON ライブラリの使い方を試す）

#ifndef JSONIF_JSONIF_JSON_IF_DLG_H_
#define JSONIF_JSONIF_JSON_IF_DLG_H_

class JsonIFDlg : public CDialogEx
{
public:
	explicit JsonIFDlg(CWnd* parent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JSONIF_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedPicojson();
	afx_msg void OnBnClickedRapidjson();
	afx_msg void OnBnClickedNlohmannjson();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
};

#endif  // JSONIF_JSONIF_JSON_IF_DLG_H_
