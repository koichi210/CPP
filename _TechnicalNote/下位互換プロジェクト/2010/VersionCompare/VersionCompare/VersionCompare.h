
// VersionCompare.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CVersionCompareApp:
// このクラスの実装については、VersionCompare.cpp を参照してください。
//

class CVersionCompareApp : public CWinApp
{
public:
	CVersionCompareApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CVersionCompareApp theApp;