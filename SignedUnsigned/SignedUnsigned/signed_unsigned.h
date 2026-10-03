// signed_unsigned.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class SignedUnsignedApp : public CWinApp
{
public:
	SignedUnsignedApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern SignedUnsignedApp the_app;
