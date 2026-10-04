// pointer_dlg.h : メインダイアログ

#ifndef POINTER_SOURCE_POINTER_DLG_H_
#define POINTER_SOURCE_POINTER_DLG_H_

struct TestData
{
	UINT param1;
	UINT param2;
	UINT param3;
};

class PointerDlg : public CDialogEx
{
public:
	explicit PointerDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_POINTER_DIALOG };

	// ポインタの受け取り方の比較用：引数（ポインタのポインタ）で返す版と戻り値で返す版
	void GetParam(TestData** pp);
	TestData* GetParam();

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
	TestData test_;
};

#endif  // POINTER_SOURCE_POINTER_DLG_H_
