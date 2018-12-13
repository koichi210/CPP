// SendSpoolFile.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSendSpoolFileApp:
// このクラスの実装については、SendSpoolFile.cpp を参照してください。
//

class CSendSpoolFileApp : public CWinApp
{
public:
	CSendSpoolFileApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSendSpoolFileApp theApp;
