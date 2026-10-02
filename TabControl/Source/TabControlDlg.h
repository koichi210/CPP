// TabControlDlg.h : メインダイアログ

#pragma once

#include "Child1.h"
#include "Child2.h"

class CTabControlDlg : public CDialogEx
{
public:
	explicit CTabControlDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_TABCONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	CTabCtrl m_tab1;	// 選択されたページ名を表示するだけのタブ
	CTabCtrl m_tab2;	// ページごとに子ダイアログを切り替えるタブ
	CChild1 m_child1;
	CChild2 m_child2;
};
