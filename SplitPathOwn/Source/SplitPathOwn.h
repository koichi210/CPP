
// SplitPathOwn.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSplitPathOwnApp:
// このクラスの実装については、SplitPathOwn.cpp を参照してください。
//

class CSplitPathOwnApp : public CWinApp
{
public:
	CSplitPathOwnApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSplitPathOwnApp theApp;