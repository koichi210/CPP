// SplitPathOwn.cpp : アプリケーションクラス

#include "stdafx.h"
#include "SplitPathOwn.h"
#include "SplitPathOwnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSplitPathOwnApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CSplitPathOwnApp::CSplitPathOwnApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CSplitPathOwnApp theApp;

BOOL CSplitPathOwnApp::InitInstance()
{
	// ComCtl32 v6 を使うマニフェストのとき、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX initCtrls = { sizeof(initCtrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	CSplitPathOwnDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
