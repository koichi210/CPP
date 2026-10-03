// fname_exchange.h : アプリケーションクラス

#ifndef FNAME_EXCHANGE_SOURCE_FNAME_EXCHANGE_H_
#define FNAME_EXCHANGE_SOURCE_FNAME_EXCHANGE_H_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class FnameExchangeApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // FNAME_EXCHANGE_SOURCE_FNAME_EXCHANGE_H_
