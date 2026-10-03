// standard_template_library_sample.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class StandardTemplateLibrarySampleApp : public CWinApp
{
public:
	StandardTemplateLibrarySampleApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern StandardTemplateLibrarySampleApp the_app;
