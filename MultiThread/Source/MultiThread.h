
// MultiThread.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMultiThreadApp:
// このクラスの実装については、MultiThread.cpp を参照してください。
//

class CMultiThreadApp : public CWinApp
{
public:
	CMultiThreadApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMultiThreadApp theApp;