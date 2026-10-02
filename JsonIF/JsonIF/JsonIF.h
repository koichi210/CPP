// JsonIF.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"

class CJsonIFApp : public CWinApp
{
public:
	CJsonIFApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};
