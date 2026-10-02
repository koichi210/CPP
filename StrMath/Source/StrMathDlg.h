// StrMathDlg.h : メインダイアログ（ひらがなで足し算／引き算）

#pragma once

class CStrMathDlg : public CDialog
{
public:
	explicit CStrMathDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_STRMATH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnSum();
	afx_msg void OnSub();
	afx_msg void On2keta();
	afx_msg void On3keta();
	afx_msg void OnAns();
	afx_msg void OnHlp();
	DECLARE_MESSAGE_MAP()

private:
	enum class Operation { Sum, Sub };

	int BuildNumber() const;

	HICON m_hIcon;
	CFont m_font;
	int m_num1 = 0;
	int m_num2 = 0;
	int m_digits = 2;
	Operation m_operation = Operation::Sum;
};
