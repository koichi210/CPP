// view_tree.h : ドッキングペイン内のツリー

#pragma once

class CViewTree : public CTreeCtrl
{
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;
};
