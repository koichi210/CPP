// view_tree.cc : ドッキングペイン内のツリー

#include "stdafx.h"
#include "view_tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL ViewTree::OnNotify(WPARAM w_param, LPARAM l_param, LRESULT* result)
{
	BOOL res = CTreeCtrl::OnNotify(w_param, l_param, result);

	const NMHDR* nmhdr = reinterpret_cast<const NMHDR*>(l_param);
	ASSERT(nmhdr != nullptr);

	// ツールチップがドッキングペインの後ろに隠れないよう最前面に出す
	if (nmhdr && nmhdr->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return res;
}
