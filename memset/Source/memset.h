// memset.h : アプリケーションクラス

#ifndef MEMSET_SOURCE_MEMSET_H_
#define MEMSET_SOURCE_MEMSET_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class MemsetApp : public CWinApp
{
public:
	MemsetApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern MemsetApp the_app;

#endif  // MEMSET_SOURCE_MEMSET_H_
