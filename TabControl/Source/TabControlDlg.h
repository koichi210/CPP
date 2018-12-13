
// TabControlDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"

#include "Child1.h"
#include "Child2.h"


// CTabControlDlg ダイアログ
class CTabControlDlg : public CDialogEx
{
// コンストラクション
public:
	CTabControlDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_TABCONTROL_DIALOG };

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
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_tab1;
	CTabCtrl m_tab2;
	CChild1 m_Child1;
	CChild2 m_Child2;

};
