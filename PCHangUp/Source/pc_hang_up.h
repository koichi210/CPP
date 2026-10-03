// pc_hang_up.h : アプリケーションクラス

#ifndef PCHANGUP_SOURCE_PC_HANG_UP_H_
#define PCHANGUP_SOURCE_PC_HANG_UP_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class PCHangUpApp : public CWinApp
{
public:
	PCHangUpApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // PCHANGUP_SOURCE_PC_HANG_UP_H_
