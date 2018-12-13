
// MotionCapture.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMotionCaptureApp:
// このクラスの実装については、MotionCapture.cpp を参照してください。
//

class CMotionCaptureApp : public CWinApp
{
public:
	CMotionCaptureApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMotionCaptureApp theApp;