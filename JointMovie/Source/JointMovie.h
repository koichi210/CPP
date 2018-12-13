// JointMovie.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CJointMovieApp:
// このクラスの実装については、JointMovie.cpp を参照してください。
//

class CJointMovieApp : public CWinApp
{
public:
	CJointMovieApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CJointMovieApp theApp;