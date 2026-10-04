// split_path_own.h : アプリケーションクラス

#ifndef SPLITPATHOWN_SOURCE_SPLIT_PATH_OWN_H_
#define SPLITPATHOWN_SOURCE_SPLIT_PATH_OWN_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class SplitPathOwnApp : public CWinApp
{
public:
	SplitPathOwnApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern SplitPathOwnApp the_app;

#endif  // SPLITPATHOWN_SOURCE_SPLIT_PATH_OWN_H_
