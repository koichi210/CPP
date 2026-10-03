// fname_exchange.cc : アプリケーションクラス

#include "stdafx.h"
#include "fname_exchange.h"
#include "fname_exchange_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(FnameExchangeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

FnameExchangeApp the_app;

BOOL FnameExchangeApp::InitInstance()
{
	CWinApp::InitInstance();

	FnameExchangeDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
