// variable_argument_dlg.h : メインダイアログ

#ifndef VARIABLEARGUMENT_SOURCE_VARIABLE_ARGUMENT_DLG_H_
#define VARIABLEARGUMENT_SOURCE_VARIABLE_ARGUMENT_DLG_H_

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

#endif  // VARIABLEARGUMENT_SOURCE_VARIABLE_ARGUMENT_DLG_H_
