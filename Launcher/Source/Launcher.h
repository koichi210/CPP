
// Launcher.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CLauncherApp:
// このクラスの実装については、Launcher.cpp を参照してください。
//

class CLauncherApp : public CWinApp
{
public:
	CLauncherApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CLauncherApp theApp;