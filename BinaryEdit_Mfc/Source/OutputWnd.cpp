// OutputWnd.cpp : 出力ウィンドウ（ドッキングペイン）

#include "stdafx.h"
#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("タブ付き出力ウィンドウを作成できませんでした\n");
		return -1;
	}

	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("出力ウィンドウを作成できませんでした\n");
		return -1;
	}

	UpdateFonts();

	struct TabDef
	{
		COutputList*	pList;
		UINT			nNameId;
	};
	const TabDef tabs[] =
	{
		{ &m_wndOutputBuild, IDS_BUILD_TAB },
		{ &m_wndOutputDebug, IDS_DEBUG_TAB },
		{ &m_wndOutputFind,  IDS_FIND_TAB },
	};
	for (UINT i = 0; i < _countof(tabs); i++)
	{
		CString strTabName;
		VERIFY(strTabName.LoadString(tabs[i].nNameId));
		m_wndTabs.AddTab(tabs[i].pList, strTabName, i);
	}

	// 表示確認用のダミーデータ
	FillWindow(m_wndOutputBuild, _T("ビルド"));
	FillWindow(m_wndOutputDebug, _T("デバッグ"));
	FillWindow(m_wndOutputFind, _T("検索"));

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// タブはクライアント領域全体を覆う
	m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::FillWindow(COutputList& wndList, LPCTSTR kind)
{
	wndList.AddString(CString(kind) + _T("出力データがここに表示されます。"));
	wndList.AddString(_T("出力データはリスト ビューの各行に表示されます"));
	wndList.AddString(_T("表示方法を変更することもできます..."));
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, &COutputList::OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, &COutputList::OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, &COutputList::OnViewOutput)
END_MESSAGE_MAP()

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		// CMFCPopupMenu は閉じたときに自分自身を delete する
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, pSumMenu->GetSafeHmenu(), FALSE, TRUE))
			return;

		static_cast<CMDIFrameWndEx*>(AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("出力データをコピーします"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("出力データをクリアします"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != nullptr && pParentBar != nullptr)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();
	}
}
