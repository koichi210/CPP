// BackUpDiff.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CBackUpDiffApp:
// このクラスの実装については、BackUpDiff.cpp を参照してください。
//

class CBackUpDiffApp : public CWinApp
{
public:
	CBackUpDiffApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CBackUpDiffApp theApp;