// zodiac.cpp : アプリケーションクラス

#include "stdafx.h"
#include "zodiac.h"
#include "zodiac_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ZodiacApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

ZodiacApp the_app;

BOOL ZodiacApp::InitInstance()
{
	AfxEnableControlContainer();

	ZodiacDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
