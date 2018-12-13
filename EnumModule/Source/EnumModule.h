
// EnumModule.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CEnumModuleApp:
// このクラスの実装については、EnumModule.cpp を参照してください。
//

class CEnumModuleApp : public CWinApp
{
public:
	CEnumModuleApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CEnumModuleApp theApp;