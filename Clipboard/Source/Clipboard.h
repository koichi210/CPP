
// Clipboard.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CClipboardApp:
// このクラスの実装については、Clipboard.cpp を参照してください。
//

class CClipboardApp : public CWinApp
{
public:
	CClipboardApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CClipboardApp theApp;