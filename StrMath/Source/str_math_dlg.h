// StrMathDlg.h : メインダイアログ（ひらがなで足し算／引き算）

#pragma once

class StrMathDlg : public CDialog
{
public:
	explicit StrMathDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_STRMATH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
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
	enum class Operation { kSum, kSub };

	int BuildNumber() const;

	HICON icon_;
	CFont font_;
	int num1_ = 0;
	int num2_ = 0;
	int digits_ = 2;
	Operation operation_ = Operation::kSum;
};
