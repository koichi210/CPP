// OutputWnd.h : 出力ウィンドウ（ドッキングペイン）

#pragma once

// 出力タブ1枚分のリスト
class COutputList : public CListBox
{
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
public:
	void UpdateFonts();

protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList	m_wndOutputBuild;
	COutputList	m_wndOutputDebug;
	COutputList	m_wndOutputFind;

	void FillWindow(COutputList& wndList, LPCTSTR kind);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
