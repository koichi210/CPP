// progress_bar_single_thread.h : アプリケーションクラス

#ifndef PROGRESSBAR_SINGLETHREAD_SOURCE_PROGRESS_BAR_SINGLE_THREAD_H_
#define PROGRESSBAR_SINGLETHREAD_SOURCE_PROGRESS_BAR_SINGLE_THREAD_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class ProgressBarSingleThreadApp : public CWinApp
{
public:
	ProgressBarSingleThreadApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern ProgressBarSingleThreadApp the_app;

#endif  // PROGRESSBAR_SINGLETHREAD_SOURCE_PROGRESS_BAR_SINGLE_THREAD_H_
