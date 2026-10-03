// file_compare.cc : アプリケーションクラス

#include "stdafx.h"
#include "file_compare.h"
#include "file_compare_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(FileCompareApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

FileCompareApp::FileCompareApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

FileCompareApp the_app;

BOOL FileCompareApp::InitInstance()
{
	// プログレスバー（コモンコントロール）を使うため
	INITCOMMONCONTROLSEX init_ctrls;
	init_ctrls.dwSize = sizeof(init_ctrls);
	init_ctrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();

	FileCompareDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
