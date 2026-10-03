// enum_token.h : アプリケーションクラス

#ifndef ENUMTOKEN_SOURCE_ENUM_TOKEN_H_
#define ENUMTOKEN_SOURCE_ENUM_TOKEN_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class EnumTokenApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // ENUMTOKEN_SOURCE_ENUM_TOKEN_H_
