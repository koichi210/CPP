// file_compare.h : アプリケーションクラス

#ifndef FILECOMPARE_SOURCE_FILE_COMPARE_H_
#define FILECOMPARE_SOURCE_FILE_COMPARE_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class FileCompareApp : public CWinApp
{
public:
	FileCompareApp();

	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern FileCompareApp the_app;

#endif  // FILECOMPARE_SOURCE_FILE_COMPARE_H_
