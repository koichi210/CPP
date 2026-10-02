// memory.cpp : アプリケーションクラス

#include "stdafx.h"
#include "memory.h"
#include "memoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMemoryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMemoryApp theApp;

BOOL CMemoryApp::InitInstance()
{
	AfxEnableControlContainer();

	CMemoryDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
