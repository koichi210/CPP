
// memcpy.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CmemcpyApp:
// このクラスの実装については、memcpy.cpp を参照してください。
//

class CmemcpyApp : public CWinApp
{
public:
	CmemcpyApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CmemcpyApp theApp;