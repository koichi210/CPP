// variable_argument.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class VariableArgumentApp : public CWinApp
{
public:
	VariableArgumentApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern VariableArgumentApp the_app;
