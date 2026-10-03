// TabControlDlg.h : メインダイアログ

#pragma once

#include "child1.h"
#include "child2.h"

class TabControlDlg : public CDialogEx
{
public:
	explicit TabControlDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_TABCONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangeTab1(NMHDR* nmhdr, LRESULT* result);
	afx_msg void OnTcnSelchangeTab2(NMHDR* nmhdr, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
	CTabCtrl tab1_;	// 選択されたページ名を表示するだけのタブ
	CTabCtrl tab2_;	// ページごとに子ダイアログを切り替えるタブ
	Child1 child1_;
	Child2 child2_;
};
