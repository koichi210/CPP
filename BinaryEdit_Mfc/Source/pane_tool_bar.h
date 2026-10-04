// pane_tool_bar.h : ドッキングペイン内のツールバー（クラス ビュー・ファイル ビュー・プロパティ ウィンドウ共通）

#ifndef BINARYEDIT_MFC_SOURCE_PANE_TOOL_BAR_H_
#define BINARYEDIT_MFC_SOURCE_PANE_TOOL_BAR_H_

class PaneToolBar : public CMFCToolBar
{
public:
	// コマンドの更新を親フレームではなくペインに回す
	virtual void OnUpdateCmdUI(CFrameWnd* /*target*/, BOOL disable_if_no_handler) override
	{
		CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), disable_if_no_handler);
	}

	virtual BOOL AllowShowOnList() const override { return FALSE; }
};

#endif  // BINARYEDIT_MFC_SOURCE_PANE_TOOL_BAR_H_
