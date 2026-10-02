// EnumTokenDlg.h : メインダイアログ（自プロセスのトークンが属するグループを列挙）

#pragma once

class CEnumTokenDlg : public CDialog
{
public:
	explicit CEnumTokenDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_SECURITY_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetproc();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};
