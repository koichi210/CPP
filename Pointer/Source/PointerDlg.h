
// PointerDlg.h : ヘッダー ファイル
//

#pragma once


typedef struct TEST {
    UINT param1;
    UINT param2;
    UINT param3;
}TEST_T;


// CPointerDlg ダイアログ
class CPointerDlg : public CDialogEx
{
// コンストラクション
public:
	CPointerDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	void GetParam(TEST_T** p );
	TEST_T* GetParam();

	TEST_T m_test;

// ダイアログ データ
	enum { IDD = IDD_POINTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()
};
