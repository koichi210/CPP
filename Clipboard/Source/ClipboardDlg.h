// ClipboardDlg.h : メインダイアログ（入力した文字列をクリップボードへコピー）

#pragma once

class CClipboardDlg : public CDialogEx
{
public:
	explicit CClipboardDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_CLIPBOARD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCopyClipboard();
	DECLARE_MESSAGE_MAP()

private:
	bool SetClipboardText(const CStringA& text);

	HICON m_hIcon;
	CString m_strText;
};
