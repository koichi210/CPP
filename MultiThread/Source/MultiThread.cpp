// MultiThread.cpp : アプリケーションクラス

#include "stdafx.h"
#include "MultiThread.h"
#include "MultiThreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMultiThreadApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMultiThreadApp theApp;

CMultiThreadApp::CMultiThreadApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL CMultiThreadApp::InitInstance()
{
	// ComCtl32.dll Version 6 を使うマニフェストの場合、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX initCtrls = {};
	initCtrls.dwSize = sizeof(initCtrls);
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();

	CMultiThreadDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
