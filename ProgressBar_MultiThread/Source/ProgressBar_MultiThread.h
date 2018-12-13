
// ProgressBar_MultiThread.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CProgressBarApp:
// このクラスの実装については、ProgressBar_MultiThread.cpp を参照してください。
//

class CProgressBarApp : public CWinApp
{
public:
	CProgressBarApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CProgressBarApp theApp;