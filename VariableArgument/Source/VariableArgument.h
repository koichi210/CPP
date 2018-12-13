
// VariableArgument.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CVariableArgumentApp:
// このクラスの実装については、VariableArgument.cpp を参照してください。
//

class CVariableArgumentApp : public CWinApp
{
public:
	CVariableArgumentApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CVariableArgumentApp theApp;