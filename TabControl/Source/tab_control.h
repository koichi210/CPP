// tab_control.h : アプリケーションクラス

#ifndef TABCONTROL_SOURCE_TAB_CONTROL_H_
#define TABCONTROL_SOURCE_TAB_CONTROL_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class TabControlApp : public CWinApp
{
public:
	TabControlApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern TabControlApp the_app;

#endif  // TABCONTROL_SOURCE_TAB_CONTROL_H_
