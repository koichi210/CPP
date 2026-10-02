// StrMath.cpp : アプリケーションクラス

#include "stdafx.h"
#include "StrMath.h"
#include "StrMathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CStrMathApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CStrMathApp theApp;

BOOL CStrMathApp::InitInstance()
{
	AfxEnableControlContainer();

	CStrMathDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
