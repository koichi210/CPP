// TemplateDlg.h : メインダイアログ

#pragma once

class CTemplateDlg : public CDialogEx
{
public:
	explicit CTemplateDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_TEMPLATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};
