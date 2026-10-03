// progress_bar_multi_thread.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class ProgressBarApp : public CWinApp
{
public:
	ProgressBarApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern ProgressBarApp the_app;
