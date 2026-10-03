// variable_argument.h : アプリケーションクラス

#ifndef VARIABLEARGUMENT_SOURCE_VARIABLE_ARGUMENT_H_
#define VARIABLEARGUMENT_SOURCE_VARIABLE_ARGUMENT_H_

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

#endif  // VARIABLEARGUMENT_SOURCE_VARIABLE_ARGUMENT_H_
