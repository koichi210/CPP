// EnumModule.h : アプリケーションクラス

#ifndef ENUMMODULE_SOURCE_ENUM_MODULE_H_
#define ENUMMODULE_SOURCE_ENUM_MODULE_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class EnumModuleApp : public CWinApp
{
public:
	EnumModuleApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // ENUMMODULE_SOURCE_ENUM_MODULE_H_
