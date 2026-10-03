// properties_wnd.h : プロパティ ウィンドウ（ドッキングペイン）

#pragma once

class PropertiesToolBar : public CMFCToolBar
{
public:
	// コマンドの更新を親フレームではなくペインに回す
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override
	{
		CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class PropertiesWnd : public CDockablePane
{
public:
	void AdjustLayout();

	void SetVSDotNetLook(BOOL set)
	{
		prop_list_.SetVSDotNetLook(set);
		prop_list_.SetGroupNameFullWidth(set);
	}

protected:
	CFont prop_list_font_;
	CComboBox object_combo_;
	PropertiesToolBar tool_bar_;
	CMFCPropertyGridCtrl prop_list_;

	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnSize(UINT type, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* cmd_ui);
	afx_msg void OnNotImplemented();
	afx_msg void OnSetFocus(CWnd* old_wnd);
	afx_msg void OnSettingChange(UINT flags, LPCTSTR section);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};
