// json_if.h : アプリケーションクラス

#ifndef JSONIF_JSONIF_JSON_IF_H_
#define JSONIF_JSONIF_JSON_IF_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"

class JsonIFApp : public CWinApp
{
public:
	JsonIFApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

#endif  // JSONIF_JSONIF_JSON_IF_H_
