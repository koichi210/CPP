// back_up.cc : アプリケーションクラス

#include "stdafx.h"
#include "back_up.h"
#include "back_up_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(BackUpApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

BackUpApp the_app;

BOOL BackUpApp::InitInstance()
{
	// visual スタイル（ComCtl32 v6）を使うにはコモンコントロールの初期化が要る
	INITCOMMONCONTROLSEX init_ctrls;
	init_ctrls.dwSize = sizeof(init_ctrls);
	init_ctrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	BackUpDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
