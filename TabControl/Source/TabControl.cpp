// TabControl.cpp : アプリケーションクラス

#include "stdafx.h"
#include "TabControl.h"
#include "TabControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTabControlApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CTabControlApp::CTabControlApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CTabControlApp theApp;

BOOL CTabControlApp::InitInstance()
{
	// ComCtl32 v6 を使うマニフェストのとき、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX initCtrls = { sizeof(initCtrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	CTabControlDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
