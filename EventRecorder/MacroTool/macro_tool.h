// macro_tool.h : アプリケーションクラス

#ifndef EVENTRECORDER_MACROTOOL_MACRO_TOOL_H_
#define EVENTRECORDER_MACROTOOL_MACRO_TOOL_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class MacroToolApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // EVENTRECORDER_MACROTOOL_MACRO_TOOL_H_
