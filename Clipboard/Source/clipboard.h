// clipboard.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class ClipboardApp : public CWinApp
{
public:
	ClipboardApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};
