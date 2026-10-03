// json_if.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"

class JsonIFApp : public CWinApp
{
public:
	JsonIFApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};
