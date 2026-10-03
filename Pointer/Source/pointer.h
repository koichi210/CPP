// pointer.h : アプリケーションクラス

#ifndef POINTER_SOURCE_POINTER_H_
#define POINTER_SOURCE_POINTER_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class PointerApp : public CWinApp
{
public:
	PointerApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern PointerApp the_app;

#endif  // POINTER_SOURCE_POINTER_H_
