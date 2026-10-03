// division_coupling.h : アプリケーションクラス

#ifndef DIVISIONCOUPLING_SOURCE_DIVISION_COUPLING_H_
#define DIVISIONCOUPLING_SOURCE_DIVISION_COUPLING_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class DivisionCouplingApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // DIVISIONCOUPLING_SOURCE_DIVISION_COUPLING_H_
