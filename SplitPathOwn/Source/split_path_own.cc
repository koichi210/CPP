// SplitPathOwn.cpp : アプリケーションクラス

#include "stdafx.h"
#include "split_path_own.h"
#include "split_path_own_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(SplitPathOwnApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

SplitPathOwnApp::SplitPathOwnApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

SplitPathOwnApp the_app;

BOOL SplitPathOwnApp::InitInstance()
{
	// ComCtl32 v6 を使うマニフェストのとき、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX init_ctrls = { sizeof(init_ctrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	SplitPathOwnDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
