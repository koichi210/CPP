// SendSpoolFileDlg.h : メインダイアログ

#pragma once

class SendSpoolFileDlg : public CDialog
{
public:
	explicit SendSpoolFileDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_SPOOLJOB2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnExecute();
	DECLARE_MESSAGE_MAP()

private:
	void AddPrinters(DWORD enum_flags);

	HICON icon_;
};
