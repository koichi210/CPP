// memory.h : アプリケーションクラス

#ifndef MEMORY_SOURCE_MEMORY_H_
#define MEMORY_SOURCE_MEMORY_H_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class MemoryApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // MEMORY_SOURCE_MEMORY_H_
