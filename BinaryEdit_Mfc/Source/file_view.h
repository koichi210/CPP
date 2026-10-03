// file_view.h : ファイル ビュー（ドッキングペイン）

#pragma once

#include "view_tree.h"

class CFileViewToolBar : public CMFCToolBar
{
	// コマンドの更新を親フレームではなくペインに回す
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override
	{
		CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
public:
	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CViewTree			m_wndFileView;
	CImageList			m_FileViewImages;
	CFileViewToolBar	m_wndToolBar;

	void FillFileView();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnNotImplemented();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};
