// memcpy.h : アプリケーションクラス

#ifndef MEMCPY_SOURCE_MEMCPY_H_
#define MEMCPY_SOURCE_MEMCPY_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class MemcpyApp : public CWinApp
{
public:
	MemcpyApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern MemcpyApp the_app;

#endif  // MEMCPY_SOURCE_MEMCPY_H_
