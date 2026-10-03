// enum_token_dlg.h : メインダイアログ（自プロセスのトークンが属するグループを列挙）

#ifndef ENUMTOKEN_SOURCE_ENUM_TOKEN_DLG_H_
#define ENUMTOKEN_SOURCE_ENUM_TOKEN_DLG_H_

class EnumTokenDlg : public CDialog
{
public:
	explicit EnumTokenDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_SECURITY_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM l_param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetproc();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
};

#endif  // ENUMTOKEN_SOURCE_ENUM_TOKEN_DLG_H_
