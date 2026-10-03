// view_tree.h : ドッキングペイン内のツリー

#pragma once

class ViewTree : public CTreeCtrl
{
protected:
	virtual BOOL OnNotify(WPARAM w_param, LPARAM l_param, LRESULT* result) override;
};
