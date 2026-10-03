// multi_thread.cc : アプリケーションクラス

#include "stdafx.h"
#include "multi_thread.h"
#include "multi_thread_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MultiThreadApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

MultiThreadApp the_app;

MultiThreadApp::MultiThreadApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL MultiThreadApp::InitInstance()
{
	// ComCtl32.dll Version 6 を使うマニフェストの場合、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX init_ctrls = {};
	init_ctrls.dwSize = sizeof(init_ctrls);
	init_ctrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();

	MultiThreadDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
