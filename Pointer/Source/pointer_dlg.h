// pointer_dlg.h : メインダイアログ

#pragma once

struct TEST_T
{
	UINT param1;
	UINT param2;
	UINT param3;
};

class CPointerDlg : public CDialogEx
{
public:
	explicit CPointerDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_POINTER_DIALOG };

	// ポインタの受け取り方の比較用：引数（ポインタのポインタ）で返す版と戻り値で返す版
	void GetParam(TEST_T** pp);
	TEST_T* GetParam();

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	TEST_T m_test;
};
