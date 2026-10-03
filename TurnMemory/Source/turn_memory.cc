// turn_memory.cc : アプリケーションクラス

#include "stdafx.h"
#include "turn_memory.h"
#include "turn_memory_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(TurnMemoryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

TurnMemoryApp the_app;

BOOL TurnMemoryApp::InitInstance()
{
	AfxEnableControlContainer();

	TurnMemoryDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
