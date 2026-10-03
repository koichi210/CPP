// class_view.h : クラス ビュー（ドッキングペイン）

#pragma once

#include "view_tree.h"

class ClassToolBar : public CMFCToolBar
{
	// コマンドの更新を親フレームではなくペインに回す
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override
	{
		CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class ClassView : public CDockablePane
{
public:
	ClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	ClassToolBar	tool_bar_;
	ViewTree		class_tree_;
	CImageList		class_view_images_;
	UINT			curr_sort_;

	void FillClassView();

	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnSize(UINT type, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* wnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnNewFolder();
	afx_msg void OnNotImplemented();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* old_wnd);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* cmd_ui);

	DECLARE_MESSAGE_MAP()
};
