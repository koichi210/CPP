// anser_dlg.h : 解答ダイアログ

#ifndef MEMORY_SOURCE_ANSER_DLG_H_
#define MEMORY_SOURCE_ANSER_DLG_H_

#include "memory_def.h"

class MemoryDlg;

class AnserDlg : public CDialog
{
public:
	AnserDlg(const MemoryDlg& game, CWnd* parent = nullptr);

	enum { IDD = IDD_MEM_ANSER };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnAnserCheck();
	afx_msg void OnAnsok();
	afx_msg void OnAnsShow();
	afx_msg HBRUSH OnCtlColor(CDC* dc, CWnd* wnd, UINT ctl_color);
	DECLARE_MESSAGE_MAP()

private:
	const MemoryDlg&	game_;			// 直前の出題内容
	BOOL				cheat_ = FALSE;	// 答えを表示中か
};

#endif  // MEMORY_SOURCE_ANSER_DLG_H_
