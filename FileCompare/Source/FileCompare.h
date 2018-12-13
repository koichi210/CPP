
// FileCompare.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CFileCompareApp:
// このクラスの実装については、FileCompare.cpp を参照してください。
//

class CFileCompareApp : public CWinApp
{
public:
	CFileCompareApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CFileCompareApp theApp;