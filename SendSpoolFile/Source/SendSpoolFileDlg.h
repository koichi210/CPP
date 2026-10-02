// SendSpoolFileDlg.h : メインダイアログ

#pragma once

class CSendSpoolFileDlg : public CDialog
{
public:
	explicit CSendSpoolFileDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_SPOOLJOB2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnExecute();
	DECLARE_MESSAGE_MAP()

private:
	void AddPrinters(DWORD enumFlags);

	HICON m_hIcon;
};
