// main_frm.cc : MDI メインフレーム

#include "stdafx.h"
#include "binary_edit_mfc.h"
#include "main_frm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(MainFrame, CMDIFrameWndEx)

namespace
{
	constexpr int	kMaxUserToolbars		= 10;
	constexpr UINT	kFirstUserToolBarId		= AFX_IDW_CONTROLBAR_FIRST + 40;
	constexpr UINT	kLastUserToolBarId		= kFirstUserToolBarId + kMaxUserToolbars - 1;

	const UINT kIndicators[] =
	{
		ID_SEPARATOR,	// ステータス ライン インジケーター
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
	};

	// ツールバーの「ユーザー設定」ボタンの文言
	CString LoadCustomizeLabel()
	{
		CString customize_label;
		VERIFY(customize_label.LoadString(IDS_TOOLBAR_CUSTOMIZE));
		return customize_label;
	}
}

BEGIN_MESSAGE_MAP(MainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &MainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &MainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &MainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &MainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &MainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

MainFrame::MainFrame()
{
	the_app.app_look_ = the_app.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

int MainFrame::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CMDIFrameWndEx::OnCreate(create_struct) == -1)
		return -1;

	// 保存されている外観を適用する
	OnApplicationLook(the_app.app_look_);

	if (!menu_bar_.Create(this))
	{
		TRACE0("メニュー バーを作成できませんでした\n");
		return -1;
	}

	menu_bar_.SetPaneStyle(menu_bar_.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// アクティブになったときメニュー バーにフォーカスを移動しない
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!tool_bar_.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!tool_bar_.LoadToolBar(the_app.hi_color_icons_ ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;
	}

	CString tool_bar_name;
	VERIFY(tool_bar_name.LoadString(IDS_TOOLBAR_STANDARD));
	tool_bar_.SetWindowText(tool_bar_name);

	const CString customize_label = LoadCustomizeLabel();
	tool_bar_.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, customize_label);

	// ユーザー定義のツール バーを許可する
	InitUserToolbars(nullptr, kFirstUserToolBarId, kLastUserToolBarId);

	if (!status_bar_.Create(this))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;
	}
	status_bar_.SetIndicators(kIndicators, static_cast<int>(_countof(kIndicators)));

	menu_bar_.EnableDocking(CBRS_ALIGN_ANY);
	tool_bar_.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&menu_bar_);
	DockPane(&tool_bar_);

	// Visual Studio 2005 スタイルのドッキングと自動非表示
	CDockingManager::SetDockingMode(DT_SMART);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// どの標準ツール バーにもないメニュー項目のイメージ
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, the_app.hi_color_icons_ ? IDB_MENU_IMAGES_24 : 0);

	if (!CreateDockingWindows())
	{
		TRACE0("ドッキング ウィンドウを作成できませんでした\n");
		return -1;
	}

	file_view_.EnableDocking(CBRS_ALIGN_ANY);
	class_view_.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&file_view_);
	CDockablePane* tabbed_bar = nullptr;
	class_view_.AttachToTabWnd(&file_view_, DM_SHOW, TRUE, &tabbed_bar);
	output_.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&output_);
	properties_.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&properties_);

	// 拡張ウィンドウ管理ダイアログ
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ツール バーとドッキング ウィンドウのメニュー
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, customize_label, ID_VIEW_TOOLBAR);

	// Alt+ドラッグでのツール バーのカスタマイズ
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		if (user_images_.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&user_images_);
		}
	}

	// メニューのパーソナル化（最近使ったコマンド）で常に表示する基本コマンド
	static const UINT kBasicCommands[] =
	{
		ID_FILE_NEW,
		ID_FILE_OPEN,
		ID_FILE_SAVE,
		ID_FILE_PRINT,
		ID_APP_EXIT,
		ID_EDIT_CUT,
		ID_EDIT_PASTE,
		ID_EDIT_UNDO,
		ID_APP_ABOUT,
		ID_VIEW_STATUS_BAR,
		ID_VIEW_TOOLBAR,
		ID_VIEW_APPLOOK_OFF_2003,
		ID_VIEW_APPLOOK_VS_2005,
		ID_VIEW_APPLOOK_OFF_2007_BLUE,
		ID_VIEW_APPLOOK_OFF_2007_SILVER,
		ID_VIEW_APPLOOK_OFF_2007_BLACK,
		ID_VIEW_APPLOOK_OFF_2007_AQUA,
		ID_VIEW_APPLOOK_WINDOWS_7,
		ID_SORTING_SORTALPHABETIC,
		ID_SORTING_SORTBYTYPE,
		ID_SORTING_SORTBYACCESS,
		ID_SORTING_GROUPBYTYPE,
	};
	CList<UINT, UINT> basic_command_list;
	for (UINT id : kBasicCommands)
	{
		basic_command_list.AddTail(id);
	}
	CMFCToolBar::SetBasicCommands(basic_command_list);

	return 0;
}

BOOL MainFrame::CreateDockingWindows()
{
	auto create = [this](CDockablePane& pane, UINT name_id, int size, UINT id, DWORD align)
	{
		CString name;
		VERIFY(name.LoadString(name_id));
		if (!pane.Create(name, this, CRect(0, 0, size, size), TRUE, id, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | align | CBRS_FLOAT_MULTI))
		{
			TRACE(_T("ドッキング ウィンドウを作成できませんでした: %s\n"), static_cast<LPCTSTR>(name));
			return false;
		}
		return true;
	};

	if (!create(class_view_, IDS_CLASS_VIEW, 200, ID_VIEW_CLASSVIEW, CBRS_LEFT) ||
		!create(file_view_, IDS_FILE_VIEW, 200, ID_VIEW_FILEVIEW, CBRS_LEFT) ||
		!create(output_, IDS_OUTPUT_WND, 100, ID_VIEW_OUTPUTWND, CBRS_BOTTOM) ||
		!create(properties_, IDS_PROPERTIES_WND, 200, ID_VIEW_PROPERTIESWND, CBRS_RIGHT))
	{
		return FALSE;
	}

	SetDockingWindowIcons(the_app.hi_color_icons_);
	return TRUE;
}

void MainFrame::SetDockingWindowIcons(BOOL hi_color_icons)
{
	auto set_icon = [hi_color_icons](CDockablePane& pane, UINT icon_id, UINT hi_color_icon_id)
	{
		HICON icon = static_cast<HICON>(::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(hi_color_icons ? hi_color_icon_id : icon_id),
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0));
		pane.SetIcon(icon, FALSE);
	};

	set_icon(file_view_, IDI_FILE_VIEW, IDI_FILE_VIEW_HC);
	set_icon(class_view_, IDI_CLASS_VIEW, IDI_CLASS_VIEW_HC);
	set_icon(output_, IDI_OUTPUT_WND, IDI_OUTPUT_WND_HC);
	set_icon(properties_, IDI_PROPERTIES_WND, IDI_PROPERTIES_WND_HC);
}

void MainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void MainFrame::OnViewCustomize()
{
	// モードレスで、閉じたときに自分自身を delete する
	CMFCToolBarsCustomizeDialog* dlg_cust = new CMFCToolBarsCustomizeDialog(this, TRUE /* メニューをスキャンする */);
	dlg_cust->EnableUserDefinedToolbars();
	dlg_cust->Create();
}

LRESULT MainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* user_toolbar = reinterpret_cast<CMFCToolBar*>(lres);
	ASSERT_VALID(user_toolbar);

	user_toolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, LoadCustomizeLabel());
	return lres;
}

void MainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	the_app.app_look_ = id;

	switch (the_app.app_look_)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		// Office 2007 系はスタイルを決めてから同じマネージャーを使う
		switch (the_app.app_look_)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	the_app.WriteInt(_T("ApplicationLook"), the_app.app_look_);
}

void MainFrame::OnUpdateApplicationLook(CCmdUI* cmd_ui)
{
	cmd_ui->SetRadio(the_app.app_look_ == cmd_ui->m_nID);
}

BOOL MainFrame::LoadFrame(UINT resource_id, DWORD default_style, CWnd* parent_wnd, CCreateContext* context)
{
	if (!CMDIFrameWndEx::LoadFrame(resource_id, default_style, parent_wnd, context))
	{
		return FALSE;
	}

	// 保存から復元されたユーザー定義ツール バーにも「ユーザー設定」ボタンを付ける
	const CString customize_label = LoadCustomizeLabel();
	for (int i = 0; i < kMaxUserToolbars; i++)
	{
		CMFCToolBar* user_toolbar = GetUserToolBarByIndex(i);
		if (user_toolbar != nullptr)
		{
			user_toolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, customize_label);
		}
	}

	return TRUE;
}

void MainFrame::OnSettingChange(UINT flags, LPCTSTR section)
{
	CMDIFrameWndEx::OnSettingChange(flags, section);
	output_.UpdateFonts();
}
