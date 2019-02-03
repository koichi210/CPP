
// mesetDlg.h : ヘッダー ファイル
//

#pragma once


// CmesetDlg ダイアログ
class CmesetDlg : public CDialogEx
{
// コンストラクション
public:
	CmesetDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_MESET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_FillVal;
	afx_msg void OnBnClickedExe();
};
