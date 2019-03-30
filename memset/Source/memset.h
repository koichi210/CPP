
// memset.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CmemsetApp:
// このクラスの実装については、memset.cpp を参照してください。
//

class CmemsetApp : public CWinApp
{
public:
	CmemsetApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CmemsetApp theApp;