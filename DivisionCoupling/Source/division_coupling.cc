// division_coupling.cc : アプリケーションクラス

#include "stdafx.h"
#include "division_coupling.h"
#include "division_coupling_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DivisionCouplingApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

DivisionCouplingApp the_app;

BOOL DivisionCouplingApp::InitInstance()
{
	CWinApp::InitInstance();

	DivisionCouplingDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
