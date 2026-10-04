// file_view.cc : ファイル ビュー（ドッキングペイン）

#include "stdafx.h"
#include "main_frm.h"
#include "file_view.h"
#include "Resource.h"
#include "binary_edit_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(FileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, &FileView::OnProperties)
	ON_COMMAND(ID_OPEN, &FileView::OnNotImplemented)
	ON_COMMAND(ID_OPEN_WITH, &FileView::OnNotImplemented)
	ON_COMMAND(ID_DUMMY_COMPILE, &FileView::OnNotImplemented)
	ON_COMMAND(ID_EDIT_CUT, &FileView::OnNotImplemented)
	ON_COMMAND(ID_EDIT_COPY, &FileView::OnNotImplemented)
	ON_COMMAND(ID_EDIT_CLEAR, &FileView::OnNotImplemented)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

int FileView::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CDockablePane::OnCreate(create_struct) == -1)
		return -1;

	CRect rect_dummy;
	rect_dummy.SetRectEmpty();

	const DWORD view_style = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!file_tree_.Create(view_style, rect_dummy, this, 4))
	{
		TRACE0("ファイル ビューを作成できませんでした\n");
		return -1;
	}

	file_view_images_.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	file_tree_.SetImageList(&file_view_images_, TVSIL_NORMAL);

	tool_bar_.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	tool_bar_.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロック */);

	OnChangeVisualStyle();

	tool_bar_.SetPaneStyle(tool_bar_.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	tool_bar_.SetPaneStyle(tool_bar_.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	tool_bar_.SetOwner(this);

	// コマンドを親フレーム経由ではなくこのペインで受ける
	tool_bar_.SetRouteCommandsViaFrame(FALSE);

	// 表示確認用のダミーデータ
	FillFileView();
	AdjustLayout();

	return 0;
}

void FileView::OnSize(UINT type, int cx, int cy)
{
	CDockablePane::OnSize(type, cx, cy);
	AdjustLayout();
}

void FileView::FillFileView()
{
	HTREEITEM root = file_tree_.InsertItem(_T("FakeApp ファイル"), 0, 0);
	file_tree_.SetItemState(root, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM src_item = file_tree_.InsertItem(_T("FakeApp ソース ファイル"), 0, 0, root);

	file_tree_.InsertItem(_T("FakeApp.cpp"), 1, 1, src_item);
	file_tree_.InsertItem(_T("FakeApp.rc"), 1, 1, src_item);
	file_tree_.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, src_item);
	file_tree_.InsertItem(_T("FakeAppView.cpp"), 1, 1, src_item);
	file_tree_.InsertItem(_T("main_frm.cc"), 1, 1, src_item);
	file_tree_.InsertItem(_T("StdAfx.cpp"), 1, 1, src_item);

	HTREEITEM inc_item = file_tree_.InsertItem(_T("FakeApp ヘッダー ファイル"), 0, 0, root);

	file_tree_.InsertItem(_T("FakeApp.h"), 2, 2, inc_item);
	file_tree_.InsertItem(_T("FakeAppDoc.h"), 2, 2, inc_item);
	file_tree_.InsertItem(_T("FakeAppView.h"), 2, 2, inc_item);
	file_tree_.InsertItem(_T("Resource.h"), 2, 2, inc_item);
	file_tree_.InsertItem(_T("main_frm.h"), 2, 2, inc_item);
	file_tree_.InsertItem(_T("StdAfx.h"), 2, 2, inc_item);

	HTREEITEM res_item = file_tree_.InsertItem(_T("FakeApp リソース ファイル"), 0, 0, root);

	file_tree_.InsertItem(_T("FakeApp.ico"), 2, 2, res_item);
	file_tree_.InsertItem(_T("FakeApp.rc2"), 2, 2, res_item);
	file_tree_.InsertItem(_T("FakeAppDoc.ico"), 2, 2, res_item);
	file_tree_.InsertItem(_T("FakeToolbar.bmp"), 2, 2, res_item);

	file_tree_.Expand(root, TVE_EXPAND);
	file_tree_.Expand(src_item, TVE_EXPAND);
	file_tree_.Expand(inc_item, TVE_EXPAND);
}

void FileView::OnContextMenu(CWnd* wnd, CPoint point)
{
	ASSERT_VALID(&file_tree_);

	if (wnd != &file_tree_)
	{
		CDockablePane::OnContextMenu(wnd, point);
		return;
	}

	file_tree_.SelectItemForContextMenu(point);

	the_app.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void FileView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rect_client;
	GetClientRect(rect_client);

	const int toolbar_height = tool_bar_.CalcFixedLayout(FALSE, TRUE).cy;

	tool_bar_.SetWindowPos(nullptr, rect_client.left, rect_client.top, rect_client.Width(), toolbar_height, SWP_NOACTIVATE | SWP_NOZORDER);
	file_tree_.SetWindowPos(nullptr, rect_client.left + 1, rect_client.top + toolbar_height + 1, rect_client.Width() - 2, rect_client.Height() - toolbar_height - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void FileView::OnProperties()
{
	AfxMessageBox(_T("プロパティ..."));
}

// 未実装のメニュー項目。ハンドラーが無いとメニューが灰色になるため空で受ける
void FileView::OnNotImplemented()
{
}

void FileView::OnPaint()
{
	CPaintDC dc(this);

	CRect rect_tree;
	file_tree_.GetWindowRect(rect_tree);
	ScreenToClient(rect_tree);

	rect_tree.InflateRect(1, 1);
	dc.Draw3dRect(rect_tree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void FileView::OnSetFocus(CWnd* old_wnd)
{
	CDockablePane::OnSetFocus(old_wnd);

	file_tree_.SetFocus();
}

void FileView::OnChangeVisualStyle()
{
	tool_bar_.CleanUpLockedImages();
	tool_bar_.LoadBitmap(the_app.hi_color_icons_ ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロック */);

	file_view_images_.DeleteImageList();

	const UINT bmp_id = the_app.hi_color_icons_ ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(bmp_id))
	{
		TRACE(_T("ビットマップを読み込めませんでした: %x\n"), bmp_id);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmp_obj;
	bmp.GetBitmap(&bmp_obj);

	const UINT flags = ILC_MASK | (the_app.hi_color_icons_ ? ILC_COLOR24 : ILC_COLOR4);

	file_view_images_.Create(16, bmp_obj.bmHeight, flags, 0, 0);
	file_view_images_.Add(&bmp, RGB(255, 0, 255));

	file_tree_.SetImageList(&file_view_images_, TVSIL_NORMAL);
}
