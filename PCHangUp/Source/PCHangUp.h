
// PCHangUp.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CPCHangUpApp:
// このクラスの実装については、PCHangUp.cpp を参照してください。
//

class CPCHangUpApp : public CWinApp
{
public:
	CPCHangUpApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CPCHangUpApp theApp;