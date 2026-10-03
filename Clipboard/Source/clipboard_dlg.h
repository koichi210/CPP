// clipboard_dlg.h : メインダイアログ（入力した文字列をクリップボードへコピー）

#ifndef CLIPBOARD_SOURCE_CLIPBOARD_DLG_H_
#define CLIPBOARD_SOURCE_CLIPBOARD_DLG_H_

class ClipboardDlg : public CDialogEx
{
public:
	explicit ClipboardDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_CLIPBOARD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCopyClipboard();
	DECLARE_MESSAGE_MAP()

private:
	bool SetClipboardText(const CStringA& text);

	HICON icon_;
	CString text_;
};

#endif  // CLIPBOARD_SOURCE_CLIPBOARD_DLG_H_
