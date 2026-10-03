// template.h : アプリケーションクラス

#ifndef TEMPLATE_TEMPLATE_TEMPLATE_H_
#define TEMPLATE_TEMPLATE_TEMPLATE_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class TemplateApp : public CWinApp
{
public:
	TemplateApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern TemplateApp the_app;

#endif  // TEMPLATE_TEMPLATE_TEMPLATE_H_
