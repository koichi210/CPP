// turn_memory.h : アプリケーションクラス

#ifndef TURNMEMORY_SOURCE_TURN_MEMORY_H_
#define TURNMEMORY_SOURCE_TURN_MEMORY_H_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class TurnMemoryApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // TURNMEMORY_SOURCE_TURN_MEMORY_H_
