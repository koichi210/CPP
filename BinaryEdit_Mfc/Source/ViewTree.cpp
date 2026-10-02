// ViewTree.cpp : ドッキングペイン内のツリー

#include "stdafx.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	const NMHDR* pNMHDR = reinterpret_cast<const NMHDR*>(lParam);
	ASSERT(pNMHDR != nullptr);

	// ツールチップがドッキングペインの後ろに隠れないよう最前面に出す
	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}
