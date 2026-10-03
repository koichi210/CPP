// variable_argument.cc : アプリケーションクラス

#include "stdafx.h"
#include "variable_argument.h"
#include "variable_argument_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(VariableArgumentApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

VariableArgumentApp::VariableArgumentApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

VariableArgumentApp the_app;

BOOL VariableArgumentApp::InitInstance()
{
	// ComCtl32 v6 を使うマニフェストのとき、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX init_ctrls = { sizeof(init_ctrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	VariableArgumentDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
