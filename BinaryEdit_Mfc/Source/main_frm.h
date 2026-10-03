// main_frm.h : MDI メインフレーム

#pragma once

#include "file_view.h"
#include "class_view.h"
#include "output_wnd.h"
#include "properties_wnd.h"

class MainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(MainFrame)
public:
	MainFrame();

	virtual BOOL LoadFrame(UINT resource_id, DWORD default_style = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* parent_wnd = nullptr, CCreateContext* context = nullptr) override;

protected:
	CMFCMenuBar			menu_bar_;
	CMFCToolBar			tool_bar_;
	CMFCStatusBar		status_bar_;
	CMFCToolBarImages	user_images_;
	FileView			file_view_;
	ClassView			class_view_;
	OutputWnd			output_;
	PropertiesWnd		properties_;

	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* cmd_ui);
	afx_msg void OnSettingChange(UINT flags, LPCTSTR section);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL hi_color_icons);
};
