// motion_capture.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class MotionCaptureApp : public CWinApp
{
public:
	MotionCaptureApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern MotionCaptureApp the_app;
