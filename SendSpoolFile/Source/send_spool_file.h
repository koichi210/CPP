// send_spool_file.h : アプリケーションクラス

#ifndef SENDSPOOLFILE_SOURCE_SEND_SPOOL_FILE_H_
#define SENDSPOOLFILE_SOURCE_SEND_SPOOL_FILE_H_

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class SendSpoolFileApp : public CWinApp
{
public:
	virtual BOOL InitInstance() override;

	DECLARE_MESSAGE_MAP()
};

extern SendSpoolFileApp the_app;

#endif  // SENDSPOOLFILE_SOURCE_SEND_SPOOL_FILE_H_
