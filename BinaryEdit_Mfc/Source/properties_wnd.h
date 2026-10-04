// properties_wnd.h : プロパティ ウィンドウ（ドッキングペイン）

#ifndef BINARYEDIT_MFC_SOURCE_PROPERTIES_WND_H_
#define BINARYEDIT_MFC_SOURCE_PROPERTIES_WND_H_

#include "pane_tool_bar.h"

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
	PaneToolBar tool_bar_;
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

#endif  // BINARYEDIT_MFC_SOURCE_PROPERTIES_WND_H_
