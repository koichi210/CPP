// EnumModuleDlg.h : メインダイアログ（プロセスが読み込んでいるモジュールを列挙）

#pragma once

class EnumModuleDlg : public CDialogEx
{
public:
	explicit EnumModuleDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_ENUMMODULE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedGetModulename();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
	CString process_name_;
	CString result_;
};
