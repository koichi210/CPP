// str_math_dlg.h : メインダイアログ（ひらがなで足し算／引き算）

#ifndef STRMATH_SOURCE_STR_MATH_DLG_H_
#define STRMATH_SOURCE_STR_MATH_DLG_H_

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
	bool started_ = false;	// スタートで出題済みか
	int digits_ = 2;
	Operation operation_ = Operation::kSum;
};

#endif  // STRMATH_SOURCE_STR_MATH_DLG_H_
