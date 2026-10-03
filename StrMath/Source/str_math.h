// StrMath.h : アプリケーションクラス

#ifndef STRMATH_SOURCE_STR_MATH_H_
#define STRMATH_SOURCE_STR_MATH_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class StrMathApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // STRMATH_SOURCE_STR_MATH_H_
