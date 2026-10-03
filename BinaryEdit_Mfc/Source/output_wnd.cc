// output_wnd.cc : 出力ウィンドウ（ドッキングペイン）

#include "stdafx.h"
#include "output_wnd.h"
#include "Resource.h"
#include "main_frm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(OutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int OutputWnd::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CDockablePane::OnCreate(create_struct) == -1)
		return -1;

	CRect rect_dummy;
	rect_dummy.SetRectEmpty();

	if (!tabs_.Create(CMFCTabCtrl::STYLE_FLAT, rect_dummy, this, 1))
	{
		TRACE0("タブ付き出力ウィンドウを作成できませんでした\n");
		return -1;
	}

	const DWORD style = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!output_build_.Create(style, rect_dummy, &tabs_, 2) ||
		!output_debug_.Create(style, rect_dummy, &tabs_, 3) ||
		!output_find_.Create(style, rect_dummy, &tabs_, 4))
	{
		TRACE0("出力ウィンドウを作成できませんでした\n");
		return -1;
	}

	UpdateFonts();

	struct TabDef
	{
		OutputList*	list;
		UINT			name_id;
	};
	const TabDef tabs[] =
	{
		{ &output_build_, IDS_BUILD_TAB },
		{ &output_debug_, IDS_DEBUG_TAB },
		{ &output_find_,  IDS_FIND_TAB },
	};
	for (UINT i = 0; i < _countof(tabs); i++)
	{
		CString tab_name;
		VERIFY(tab_name.LoadString(tabs[i].name_id));
		tabs_.AddTab(tabs[i].list, tab_name, i);
	}

	// 表示確認用のダミーデータ
	FillWindow(output_build_, _T("ビルド"));
	FillWindow(output_debug_, _T("デバッグ"));
	FillWindow(output_find_, _T("検索"));

	return 0;
}

void OutputWnd::OnSize(UINT type, int cx, int cy)
{
	CDockablePane::OnSize(type, cx, cy);

	// タブはクライアント領域全体を覆う
	tabs_.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void OutputWnd::FillWindow(OutputList& output_list, LPCTSTR kind)
{
	output_list.AddString(CString(kind) + _T("出力データがここに表示されます。"));
	output_list.AddString(_T("出力データはリスト ビューの各行に表示されます"));
	output_list.AddString(_T("表示方法を変更することもできます..."));
}

void OutputWnd::UpdateFonts()
{
	output_build_.SetFont(&afxGlobalData.fontRegular);
	output_debug_.SetFont(&afxGlobalData.fontRegular);
	output_find_.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// OutputList

BEGIN_MESSAGE_MAP(OutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, &OutputList::OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, &OutputList::OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, &OutputList::OnViewOutput)
END_MESSAGE_MAP()

void OutputList::OnContextMenu(CWnd* /*wnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

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

	SetFocus();
}

void OutputList::OnEditCopy()
{
	MessageBox(_T("出力データをコピーします"));
}

void OutputList::OnEditClear()
{
	MessageBox(_T("出力データをクリアします"));
}

void OutputList::OnViewOutput()
{
	CDockablePane* parent_bar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* main_frame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (main_frame != nullptr && parent_bar != nullptr)
	{
		main_frame->SetFocus();
		main_frame->ShowPane(parent_bar, FALSE, FALSE, FALSE);
		main_frame->RecalcLayout();
	}
}
