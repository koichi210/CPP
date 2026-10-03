// motion_capture.cc : アプリケーションクラス

#include "stdafx.h"
#include "motion_capture.h"
#include "motion_capture_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMotionCaptureApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMotionCaptureApp theApp;

CMotionCaptureApp::CMotionCaptureApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL CMotionCaptureApp::InitInstance()
{
	// visual スタイル（ComCtl32 v6）を使うにはコモンコントロールの初期化が要る
	INITCOMMONCONTROLSEX initCtrls;
	initCtrls.dwSize = sizeof(initCtrls);
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	CMotionCaptureDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
