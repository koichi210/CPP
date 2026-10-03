// class_view.cc : クラス ビュー（ドッキングペイン）

#include "stdafx.h"
#include "main_frm.h"
#include "class_view.h"
#include "Resource.h"
#include "binary_edit_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 並べ替えメニュー付きのツールバーボタン
class ClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class ClassView;

	DECLARE_SERIAL(ClassViewMenuButton)

public:
	ClassViewMenuButton(HMENU menu = nullptr) : CMFCToolBarMenuButton(static_cast<UINT>(-1), menu, -1)
	{
	}

	virtual void OnDraw(CDC* dc, const CRect& rect, CMFCToolBarImages* images, BOOL horz = TRUE,
		BOOL customize_mode = FALSE, BOOL highlight = FALSE, BOOL draw_border = TRUE, BOOL gray_disabled_buttons = TRUE)
	{
		// ペインのツールバーではなく共通のコマンドイメージで描く
		images = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		images->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(dc, rect, images, horz, customize_mode, highlight, draw_border, gray_disabled_buttons);

		images->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(ClassViewMenuButton, CMFCToolBarMenuButton, 1)

ClassView::ClassView()
	: curr_sort_(ID_SORTING_GROUPBYTYPE)
{
}

BEGIN_MESSAGE_MAP(ClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, &ClassView::OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, &ClassView::OnNotImplemented)
	ON_COMMAND(ID_CLASS_DEFINITION, &ClassView::OnNotImplemented)
	ON_COMMAND(ID_CLASS_PROPERTIES, &ClassView::OnNotImplemented)
	ON_COMMAND(ID_NEW_FOLDER, &ClassView::OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, &ClassView::OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, &ClassView::OnUpdateSort)
END_MESSAGE_MAP()

int ClassView::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CDockablePane::OnCreate(create_struct) == -1)
		return -1;

	CRect rect_dummy;
	rect_dummy.SetRectEmpty();

	const DWORD view_style = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!class_tree_.Create(view_style, rect_dummy, this, 2))
	{
		TRACE0("クラス ビューを作成できませんでした\n");
		return -1;
	}

	tool_bar_.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	tool_bar_.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ロック */);

	OnChangeVisualStyle();

	tool_bar_.SetPaneStyle(tool_bar_.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	tool_bar_.SetPaneStyle(tool_bar_.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	tool_bar_.SetOwner(this);

	// コマンドを親フレーム経由ではなくこのペインで受ける
	tool_bar_.SetRouteCommandsViaFrame(FALSE);

	CMenu menu_sort;
	menu_sort.LoadMenu(IDR_POPUP_SORT);

	tool_bar_.ReplaceButton(ID_SORT_MENU, ClassViewMenuButton(menu_sort.GetSubMenu(0)->GetSafeHmenu()));

	ClassViewMenuButton* button = DYNAMIC_DOWNCAST(ClassViewMenuButton, tool_bar_.GetButton(0));
	if (button != nullptr)
	{
		button->m_bText = FALSE;
		button->m_bImage = TRUE;
		button->SetImage(GetCmdMgr()->GetCmdImage(curr_sort_));
		button->SetMessageWnd(this);
	}

	// 表示確認用のダミーデータ
	FillClassView();

	return 0;
}

void ClassView::OnSize(UINT type, int cx, int cy)
{
	CDockablePane::OnSize(type, cx, cy);
	AdjustLayout();
}

void ClassView::FillClassView()
{
	HTREEITEM root = class_tree_.InsertItem(_T("FakeApp クラス"), 0, 0);
	class_tree_.SetItemState(root, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM class_item = class_tree_.InsertItem(_T("CFakeAboutDlg"), 1, 1, root);
	class_tree_.InsertItem(_T("CFakeAboutDlg()"), 3, 3, class_item);

	class_tree_.Expand(root, TVE_EXPAND);

	class_item = class_tree_.InsertItem(_T("CFakeApp"), 1, 1, root);
	class_tree_.InsertItem(_T("CFakeApp()"), 3, 3, class_item);
	class_tree_.InsertItem(_T("InitInstance()"), 3, 3, class_item);
	class_tree_.InsertItem(_T("OnAppAbout()"), 3, 3, class_item);

	class_item = class_tree_.InsertItem(_T("CFakeAppDoc"), 1, 1, root);
	class_tree_.InsertItem(_T("CFakeAppDoc()"), 4, 4, class_item);
	class_tree_.InsertItem(_T("~CFakeAppDoc()"), 3, 3, class_item);
	class_tree_.InsertItem(_T("OnNewDocument()"), 3, 3, class_item);

	class_item = class_tree_.InsertItem(_T("CFakeAppView"), 1, 1, root);
	class_tree_.InsertItem(_T("CFakeAppView()"), 4, 4, class_item);
	class_tree_.InsertItem(_T("~CFakeAppView()"), 3, 3, class_item);
	class_tree_.InsertItem(_T("GetDocument()"), 3, 3, class_item);
	class_tree_.Expand(class_item, TVE_EXPAND);

	class_item = class_tree_.InsertItem(_T("CFakeAppFrame"), 1, 1, root);
	class_tree_.InsertItem(_T("CFakeAppFrame()"), 3, 3, class_item);
	class_tree_.InsertItem(_T("~CFakeAppFrame()"), 3, 3, class_item);
	class_tree_.InsertItem(_T("m_wndMenuBar"), 6, 6, class_item);
	class_tree_.InsertItem(_T("m_wndToolBar"), 6, 6, class_item);
	class_tree_.InsertItem(_T("m_wndStatusBar"), 6, 6, class_item);

	class_item = class_tree_.InsertItem(_T("Globals"), 2, 2, root);
	class_tree_.InsertItem(_T("theFakeApp"), 5, 5, class_item);
	class_tree_.Expand(class_item, TVE_EXPAND);
}

void ClassView::OnContextMenu(CWnd* wnd, CPoint point)
{
	CTreeCtrl* wnd_tree = &class_tree_;
	ASSERT_VALID(wnd_tree);

	if (wnd != wnd_tree)
	{
		CDockablePane::OnContextMenu(wnd, point);
		return;
	}

	// キーボード（Shift+F10 等）から開いたときは (-1, -1) が来る
	if (point != CPoint(-1, -1))
	{
		CPoint pt_tree = point;
		wnd_tree->ScreenToClient(&pt_tree);

		UINT flags = 0;
		HTREEITEM tree_item = wnd_tree->HitTest(pt_tree, &flags);
		if (tree_item != nullptr)
		{
			wnd_tree->SelectItem(tree_item);
		}
	}

	wnd_tree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* sum_menu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		// CMFCPopupMenu は閉じたときに自分自身を delete する
		CMFCPopupMenu* popup_menu = new CMFCPopupMenu;

		if (!popup_menu->Create(this, point.x, point.y, sum_menu->GetSafeHmenu(), FALSE, TRUE))
			return;

		static_cast<CMDIFrameWndEx*>(AfxGetMainWnd())->OnShowPopupMenu(popup_menu);
		UpdateDialogControls(this, FALSE);
	}
}

void ClassView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rect_client;
	GetClientRect(rect_client);

	int toolbar_height = tool_bar_.CalcFixedLayout(FALSE, TRUE).cy;

	tool_bar_.SetWindowPos(nullptr, rect_client.left, rect_client.top, rect_client.Width(), toolbar_height, SWP_NOACTIVATE | SWP_NOZORDER);
	class_tree_.SetWindowPos(nullptr, rect_client.left + 1, rect_client.top + toolbar_height + 1, rect_client.Width() - 2, rect_client.Height() - toolbar_height - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void ClassView::OnSort(UINT id)
{
	if (curr_sort_ == id)
	{
		return;
	}

	curr_sort_ = id;

	ClassViewMenuButton* button = DYNAMIC_DOWNCAST(ClassViewMenuButton, tool_bar_.GetButton(0));
	if (button != nullptr)
	{
		button->SetImage(GetCmdMgr()->GetCmdImage(id));
		tool_bar_.Invalidate();
		tool_bar_.UpdateWindow();
	}
}

void ClassView::OnUpdateSort(CCmdUI* cmd_ui)
{
	cmd_ui->SetCheck(cmd_ui->m_nID == curr_sort_);
}

void ClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("メンバー関数の追加..."));
}

void ClassView::OnNewFolder()
{
	AfxMessageBox(_T("新しいフォルダー..."));
}

// 未実装のメニュー項目。ハンドラーが無いとメニューが灰色になるため空で受ける
void ClassView::OnNotImplemented()
{
}

void ClassView::OnPaint()
{
	CPaintDC dc(this);

	CRect rect_tree;
	class_tree_.GetWindowRect(rect_tree);
	ScreenToClient(rect_tree);

	rect_tree.InflateRect(1, 1);
	dc.Draw3dRect(rect_tree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void ClassView::OnSetFocus(CWnd* old_wnd)
{
	CDockablePane::OnSetFocus(old_wnd);

	class_tree_.SetFocus();
}

void ClassView::OnChangeVisualStyle()
{
	class_view_images_.DeleteImageList();

	UINT bmp_id = the_app.hi_color_icons_ ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(bmp_id))
	{
		TRACE(_T("ビットマップを読み込めませんでした: %x\n"), bmp_id);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmp_obj;
	bmp.GetBitmap(&bmp_obj);

	UINT flags = ILC_MASK | (the_app.hi_color_icons_ ? ILC_COLOR24 : ILC_COLOR4);

	class_view_images_.Create(16, bmp_obj.bmHeight, flags, 0, 0);
	class_view_images_.Add(&bmp, RGB(255, 0, 0));

	class_tree_.SetImageList(&class_view_images_, TVSIL_NORMAL);

	tool_bar_.CleanUpLockedImages();
	tool_bar_.LoadBitmap(the_app.hi_color_icons_ ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ロック */);
}
