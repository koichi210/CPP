// division_coupling.cc : アプリケーションクラス

#include "stdafx.h"
#include "division_coupling.h"
#include "division_coupling_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDivisionCouplingApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CDivisionCouplingApp theApp;

BOOL CDivisionCouplingApp::InitInstance()
{
	CWinApp::InitInstance();

	CDivisionCouplingDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
