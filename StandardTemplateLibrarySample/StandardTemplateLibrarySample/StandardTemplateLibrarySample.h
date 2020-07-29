
// StandardTemplateLibrarySample.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CStandardTemplateLibrarySampleApp:
// このクラスの実装については、StandardTemplateLibrarySample.cpp を参照してください。
//

class CStandardTemplateLibrarySampleApp : public CWinApp
{
public:
	CStandardTemplateLibrarySampleApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CStandardTemplateLibrarySampleApp theApp;