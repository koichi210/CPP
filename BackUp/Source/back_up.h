// back_up.h : アプリケーションクラス

#ifndef BACKUP_SOURCE_BACK_UP_H_
#define BACKUP_SOURCE_BACK_UP_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class BackUpApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern BackUpApp the_app;

#endif  // BACKUP_SOURCE_BACK_UP_H_
