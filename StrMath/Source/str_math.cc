// StrMath.cpp : アプリケーションクラス

#include "stdafx.h"
#include "str_math.h"
#include "str_math_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(StrMathApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

StrMathApp the_app;

BOOL StrMathApp::InitInstance()
{
	AfxEnableControlContainer();

	StrMathDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
