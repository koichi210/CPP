// variable_argument_dlg.h : メインダイアログ

#pragma once

class CVariableArgumentDlg : public CDialogEx
{
public:
	explicit CVariableArgumentDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_VARIABLEARGUMENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonExecC();
	afx_msg void OnBnClickedButtonExeCpp();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	CString m_strInput;
	CString m_strReplace;
	CString m_strOutput;
};
