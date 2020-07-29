
// SignedUnsigned.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSignedUnsignedApp:
// このクラスの実装については、SignedUnsigned.cpp を参照してください。
//

class CSignedUnsignedApp : public CWinApp
{
public:
	CSignedUnsignedApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSignedUnsignedApp theApp;