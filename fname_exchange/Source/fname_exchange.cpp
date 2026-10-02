// fname_exchange.cpp : アプリケーションクラス

#include "stdafx.h"
#include "fname_exchange.h"
#include "fname_exchangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFnameExchangeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CFnameExchangeApp theApp;

BOOL CFnameExchangeApp::InitInstance()
{
	CWinApp::InitInstance();

	CFnameExchangeDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
