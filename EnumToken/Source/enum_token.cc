// enum_token.cc : アプリケーションクラス

#include "stdafx.h"
#include "enum_token.h"
#include "enum_token_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CEnumTokenApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CEnumTokenApp theApp;

BOOL CEnumTokenApp::InitInstance()
{
	CEnumTokenDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
