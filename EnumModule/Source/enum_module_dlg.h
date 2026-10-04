// enum_module_dlg.h : メインダイアログ（プロセスが読み込んでいるモジュールを列挙）

#ifndef ENUMMODULE_SOURCE_ENUM_MODULE_DLG_H_
#define ENUMMODULE_SOURCE_ENUM_MODULE_DLG_H_

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

#endif  // ENUMMODULE_SOURCE_ENUM_MODULE_DLG_H_
