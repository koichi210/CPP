// output_wnd.h : 出力ウィンドウ（ドッキングペイン）

#ifndef BINARYEDIT_MFC_SOURCE_OUTPUT_WND_H_
#define BINARYEDIT_MFC_SOURCE_OUTPUT_WND_H_

// 出力タブ1枚分のリスト
class OutputList : public CListBox
{
protected:
	afx_msg void OnContextMenu(CWnd* wnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class OutputWnd : public CDockablePane
{
public:
	void UpdateFonts();

protected:
	CMFCTabCtrl	tabs_;

	OutputList	output_build_;
	OutputList	output_debug_;
	OutputList	output_find_;

	void FillWindow(OutputList& output_list, LPCTSTR kind);

	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnSize(UINT type, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif  // BINARYEDIT_MFC_SOURCE_OUTPUT_WND_H_
