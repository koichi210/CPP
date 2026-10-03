// zodiac.h : アプリケーションクラス

#ifndef ZODIAC_SOURCE_ZODIAC_H_
#define ZODIAC_SOURCE_ZODIAC_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class ZodiacApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // ZODIAC_SOURCE_ZODIAC_H_
