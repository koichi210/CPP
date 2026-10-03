// signed_unsigned.cc : アプリケーションクラス

#include "stdafx.h"
#include "signed_unsigned.h"
#include "signed_unsigned_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(SignedUnsignedApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

SignedUnsignedApp::SignedUnsignedApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

SignedUnsignedApp the_app;

BOOL SignedUnsignedApp::InitInstance()
{
	// ComCtl32 v6 を使うマニフェストのとき、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX init_ctrls = { sizeof(init_ctrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	SignedUnsignedDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
