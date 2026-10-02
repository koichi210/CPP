// zodiac.cpp : アプリケーションクラス

#include "stdafx.h"
#include "zodiac.h"
#include "zodiacDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CZodiacApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CZodiacApp theApp;

BOOL CZodiacApp::InitInstance()
{
	AfxEnableControlContainer();

	CZodiacDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
