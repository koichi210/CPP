// motion_capture.h : アプリケーションクラス

#ifndef MOTIONCAPTURE_SOURCE_MOTION_CAPTURE_H_
#define MOTIONCAPTURE_SOURCE_MOTION_CAPTURE_H_

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

#endif  // MOTIONCAPTURE_SOURCE_MOTION_CAPTURE_H_
