// standard_template_library_sample_dlg.h : メインダイアログ

#pragma once

class StandardTemplateLibrarySampleDlg : public CDialogEx
{
public:
	explicit StandardTemplateLibrarySampleDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_STANDARDTEMPLATELIBRARYSAMPLE_DIALOG };

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
