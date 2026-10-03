// SplitPathOwn.h : アプリケーションクラス

#pragma once

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
