// view_tree.h : ドッキングペイン内のツリー

#ifndef BINARYEDIT_MFC_SOURCE_VIEW_TREE_H_
#define BINARYEDIT_MFC_SOURCE_VIEW_TREE_H_

class ViewTree : public CTreeCtrl
{
protected:
	virtual BOOL OnNotify(WPARAM w_param, LPARAM l_param, LRESULT* result) override;
};

#endif  // BINARYEDIT_MFC_SOURCE_VIEW_TREE_H_
