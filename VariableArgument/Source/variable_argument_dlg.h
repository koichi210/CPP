// variable_argument_dlg.h : メインダイアログ

#pragma once

class VariableArgumentDlg : public CDialogEx
{
public:
	explicit VariableArgumentDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_VARIABLEARGUMENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonExecC();
	afx_msg void OnBnClickedButtonExeCpp();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
	CString input_;
	CString replace_;
	CString output_;
};
