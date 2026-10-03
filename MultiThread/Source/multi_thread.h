// multi_thread.h : アプリケーションクラス

#ifndef MULTITHREAD_SOURCE_MULTI_THREAD_H_
#define MULTITHREAD_SOURCE_MULTI_THREAD_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class MultiThreadApp : public CWinApp
{
public:
	MultiThreadApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // MULTITHREAD_SOURCE_MULTI_THREAD_H_
