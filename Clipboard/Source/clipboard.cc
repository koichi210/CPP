// clipboard.cc : アプリケーションクラス

#include "stdafx.h"
#include "clipboard.h"
#include "clipboard_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CClipboardApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CClipboardApp theApp;

CClipboardApp::CClipboardApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL CClipboardApp::InitInstance()
{
	// ComCtl32.dll Version 6 を使うマニフェストの場合、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX initCtrls = {};
	initCtrls.dwSize = sizeof(initCtrls);
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();

	CClipboardDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
