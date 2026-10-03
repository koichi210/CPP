// macro_tool.cc : アプリケーションクラス

#include "stdafx.h"
#include "macro_tool.h"
#include "main_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MacroToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

MacroToolApp the_app;

BOOL MacroToolApp::InitInstance()
{
	// 一覧（リストビュー）などのコモンコントロールを使えるようにする
	INITCOMMONCONTROLSEX init_ctrls = {};
	init_ctrls.dwSize = sizeof(init_ctrls);
	init_ctrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();

	MainDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
