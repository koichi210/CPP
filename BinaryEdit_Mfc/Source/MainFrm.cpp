// MainFrm.cpp : MDI メインフレーム

#include "stdafx.h"
#include "BinaryEdit_Mfc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

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
		CString strCustomize;
		VERIFY(strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE));
		return strCustomize;
	}
}

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 保存されている外観を適用する
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("メニュー バーを作成できませんでした\n");
		return -1;
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// アクティブになったときメニュー バーにフォーカスを移動しない
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;
	}

	CString strToolBarName;
	VERIFY(strToolBarName.LoadString(IDS_TOOLBAR_STANDARD));
	m_wndToolBar.SetWindowText(strToolBarName);

	const CString strCustomize = LoadCustomizeLabel();
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// ユーザー定義のツール バーを許可する
	InitUserToolbars(nullptr, kFirstUserToolBarId, kLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;
	}
	m_wndStatusBar.SetIndicators(kIndicators, static_cast<int>(_countof(kIndicators)));

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// Visual Studio 2005 スタイルのドッキングと自動非表示
	CDockingManager::SetDockingMode(DT_SMART);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// どの標準ツール バーにもないメニュー項目のイメージ
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	if (!CreateDockingWindows())
	{
		TRACE0("ドッキング ウィンドウを作成できませんでした\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = nullptr;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// 拡張ウィンドウ管理ダイアログ
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ツール バーとドッキング ウィンドウのメニュー
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// Alt+ドラッグでのツール バーのカスタマイズ
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// メニューのパーソナル化（最近使ったコマンド）で常に表示する基本コマンド
	static const UINT basicCommands[] =
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
	CList<UINT, UINT> lstBasicCommands;
	for (UINT id : basicCommands)
	{
		lstBasicCommands.AddTail(id);
	}
	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::CreateDockingWindows()
{
	auto create = [this](CDockablePane& pane, UINT nNameId, int size, UINT nId, DWORD dwAlign)
	{
		CString strName;
		VERIFY(strName.LoadString(nNameId));
		if (!pane.Create(strName, this, CRect(0, 0, size, size), TRUE, nId, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwAlign | CBRS_FLOAT_MULTI))
		{
			TRACE(_T("ドッキング ウィンドウを作成できませんでした: %s\n"), static_cast<LPCTSTR>(strName));
			return false;
		}
		return true;
	};

	if (!create(m_wndClassView, IDS_CLASS_VIEW, 200, ID_VIEW_CLASSVIEW, CBRS_LEFT) ||
		!create(m_wndFileView, IDS_FILE_VIEW, 200, ID_VIEW_FILEVIEW, CBRS_LEFT) ||
		!create(m_wndOutput, IDS_OUTPUT_WND, 100, ID_VIEW_OUTPUTWND, CBRS_BOTTOM) ||
		!create(m_wndProperties, IDS_PROPERTIES_WND, 200, ID_VIEW_PROPERTIESWND, CBRS_RIGHT))
	{
		return FALSE;
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	auto setIcon = [bHiColorIcons](CDockablePane& pane, UINT nIconId, UINT nHiColorIconId)
	{
		HICON hIcon = static_cast<HICON>(::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? nHiColorIconId : nIconId),
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0));
		pane.SetIcon(hIcon, FALSE);
	};

	setIcon(m_wndFileView, IDI_FILE_VIEW, IDI_FILE_VIEW_HC);
	setIcon(m_wndClassView, IDI_CLASS_VIEW, IDI_CLASS_VIEW_HC);
	setIcon(m_wndOutput, IDI_OUTPUT_WND, IDI_OUTPUT_WND_HC);
	setIcon(m_wndProperties, IDI_PROPERTIES_WND, IDI_PROPERTIES_WND_HC);
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	// モードレスで、閉じたときに自分自身を delete する
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* メニューをスキャンする */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = reinterpret_cast<CMFCToolBar*>(lres);
	ASSERT_VALID(pUserToolbar);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, LoadCustomizeLabel());
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
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
		switch (theApp.m_nAppLook)
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

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 保存から復元されたユーザー定義ツール バーにも「ユーザー設定」ボタンを付ける
	const CString strCustomize = LoadCustomizeLabel();
	for (int i = 0; i < kMaxUserToolbars; i++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}
