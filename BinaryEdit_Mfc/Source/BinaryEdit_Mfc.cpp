// BinaryEdit_Mfc.cpp : アプリケーションクラス

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BinaryEdit_Mfc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "BinaryEdit_MfcDoc.h"
#include "BinaryEdit_MfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBinaryEdit_MfcApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBinaryEdit_MfcApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

CBinaryEdit_MfcApp theApp;

CBinaryEdit_MfcApp::CBinaryEdit_MfcApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	SetAppID(_T("BinaryEdit_Mfc.AppID.NoVersion"));
}

BOOL CBinaryEdit_MfcApp::InitInstance()
{
	// visual スタイル（ComCtl32 v6）を使うにはコモンコントロールの初期化が要る
	INITCOMMONCONTROLSEX initCtrls;
	initCtrls.dwSize = sizeof(initCtrls);
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);

	CWinAppEx::InitInstance();

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	LoadStdProfileSettings(4);	// MRU を含む標準の設定を読み込む

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	AddDocTemplate(new CMultiDocTemplate(IDR_BinaryEdit_MfcTYPE,
		RUNTIME_CLASS(CBinaryEdit_MfcDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CBinaryEdit_MfcView)));

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// MDI では m_pMainWnd を設定した直後に呼ぶ必要がある
	m_pMainWnd->DragAcceptFiles();

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// /RegServer 等で起動されたときは FALSE が返り、そのまま終了する
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CBinaryEdit_MfcApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

void CBinaryEdit_MfcApp::PreLoadState()
{
	CString strName;
	VERIFY(strName.LoadString(IDS_EDIT_MENU));
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	VERIFY(strName.LoadString(IDS_EXPLORER));
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	enum { IDD = IDD_ABOUTBOX };

	CAboutDlg() : CDialogEx(IDD) {}
};

void CBinaryEdit_MfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
