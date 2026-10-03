// standard_template_library_sample.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class CStandardTemplateLibrarySampleApp : public CWinApp
{
public:
	CStandardTemplateLibrarySampleApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern CStandardTemplateLibrarySampleApp theApp;
