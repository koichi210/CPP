// TurnMemory.cpp : アプリケーションクラス

#include "stdafx.h"
#include "TurnMemory.h"
#include "TurnMemoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTurnMemoryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CTurnMemoryApp theApp;

BOOL CTurnMemoryApp::InitInstance()
{
	AfxEnableControlContainer();

	CTurnMemoryDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
