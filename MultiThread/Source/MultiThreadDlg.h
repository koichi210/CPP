
// MultiThreadDlg.h : ヘッダー ファイル
//

#pragma once


// CMultiThreadDlg ダイアログ
class CMultiThreadDlg : public CDialogEx
{
// コンストラクション
public:
	CMultiThreadDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_MULTITHREAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_stop ;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

UINT ProcThread(LPVOID pParam);
