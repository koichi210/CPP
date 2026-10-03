// file_view.h : ファイル ビュー（ドッキングペイン）

#ifndef BINARYEDIT_MFC_SOURCE_FILE_VIEW_H_
#define BINARYEDIT_MFC_SOURCE_FILE_VIEW_H_

#include "view_tree.h"

class FileViewToolBar : public CMFCToolBar
{
	// コマンドの更新を親フレームではなくペインに回す
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override
	{
		CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class FileView : public CDockablePane
{
public:
	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	ViewTree			file_tree_;
	CImageList			file_view_images_;
	FileViewToolBar	tool_bar_;

	void FillFileView();

	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnSize(UINT type, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* wnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnNotImplemented();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* old_wnd);

	DECLARE_MESSAGE_MAP()
};

#endif  // BINARYEDIT_MFC_SOURCE_FILE_VIEW_H_
