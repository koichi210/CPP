// template_dlg.h : メインダイアログ

#pragma once

class TemplateDlg : public CDialogEx
{
public:
	explicit TemplateDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_TEMPLATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
};
