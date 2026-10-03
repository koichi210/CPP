// memory.cc : アプリケーションクラス

#include "stdafx.h"
#include "memory.h"
#include "memory_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MemoryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

MemoryApp the_app;

BOOL MemoryApp::InitInstance()
{
	AfxEnableControlContainer();

	MemoryDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
