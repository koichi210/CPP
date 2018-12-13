
// ProgressBar_SingleThread.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CProgressBar_SingleThreadApp:
// このクラスの実装については、ProgressBar_SingleThread.cpp を参照してください。
//

class CProgressBar_SingleThreadApp : public CWinApp
{
public:
	CProgressBar_SingleThreadApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CProgressBar_SingleThreadApp theApp;