// MacroTool.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMacroToolApp:
// このクラスの実装については、MacroTool.cpp を参照してください。
//

class CMacroToolApp : public CWinApp
{
public:
	CMacroToolApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMacroToolApp theApp;