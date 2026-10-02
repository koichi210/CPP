// AnserDlg.h : 解答ダイアログ

#pragma once

#include "memoryDef.h"

class CMemoryDlg;

class CAnserDlg : public CDialog
{
public:
	CAnserDlg(const CMemoryDlg& game, CWnd* pParent = nullptr);

	enum { IDD = IDD_MEM_ANSER };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnAnserCheck();
	afx_msg void OnAnsok();
	afx_msg void OnAnsShow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	const CMemoryDlg&	m_game;			// 直前の出題内容
	BOOL				m_bCheat = FALSE;	// 答えを表示中か
};
