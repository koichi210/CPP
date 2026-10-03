// standard_template_library_sample.h : アプリケーションクラス

#ifndef STANDARDTEMPLATELIBRARYSAMPLE_STANDARDTEMPLATELIBRARYSAMPLE_STANDARD_TEMPLATE_LIBRARY_SAMPLE_H_
#define STANDARDTEMPLATELIBRARYSAMPLE_STANDARDTEMPLATELIBRARYSAMPLE_STANDARD_TEMPLATE_LIBRARY_SAMPLE_H_

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

#endif  // STANDARDTEMPLATELIBRARYSAMPLE_STANDARDTEMPLATELIBRARYSAMPLE_STANDARD_TEMPLATE_LIBRARY_SAMPLE_H_
