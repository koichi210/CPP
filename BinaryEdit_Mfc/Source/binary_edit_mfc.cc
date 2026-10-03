// binary_edit_mfc.cc : アプリケーションクラス

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "binary_edit_mfc.h"
#include "main_frm.h"
#include "child_frm.h"
#include "binary_edit_mfc_doc.h"
#include "binary_edit_mfc_view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(BinaryEditMfcApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &BinaryEditMfcApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

BinaryEditMfcApp the_app;

BinaryEditMfcApp::BinaryEditMfcApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	SetAppID(_T("BinaryEdit_Mfc.AppID.NoVersion"));
}

BOOL BinaryEditMfcApp::InitInstance()
{
	// visual スタイル（ComCtl32 v6）を使うにはコモンコントロールの初期化が要る
	INITCOMMONCONTROLSEX init_ctrls;
	init_ctrls.dwSize = sizeof(init_ctrls);
	init_ctrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&init_ctrls);

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

	CMFCToolTipInfo tt_params;
	tt_params.m_bVislManagerTheme = TRUE;
	GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &tt_params);

	AddDocTemplate(new CMultiDocTemplate(IDR_BinaryEdit_MfcTYPE,
		RUNTIME_CLASS(BinaryEditMfcDoc),
		RUNTIME_CLASS(ChildFrame),
		RUNTIME_CLASS(BinaryEditMfcView)));

	MainFrame* main_frame = new MainFrame;
	if (!main_frame->LoadFrame(IDR_MAINFRAME))
	{
		delete main_frame;
		return FALSE;
	}
	m_pMainWnd = main_frame;

	// MDI では m_pMainWnd を設定した直後に呼ぶ必要がある
	m_pMainWnd->DragAcceptFiles();

	CCommandLineInfo cmd_info;
	ParseCommandLine(cmd_info);

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// /RegServer 等で起動されたときは FALSE が返り、そのまま終了する
	if (!ProcessShellCommand(cmd_info))
		return FALSE;

	main_frame->ShowWindow(m_nCmdShow);
	main_frame->UpdateWindow();

	return TRUE;
}

int BinaryEditMfcApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

void BinaryEditMfcApp::PreLoadState()
{
	CString name;
	VERIFY(name.LoadString(IDS_EDIT_MENU));
	GetContextMenuManager()->AddMenu(name, IDR_POPUP_EDIT);
	VERIFY(name.LoadString(IDS_EXPLORER));
	GetContextMenuManager()->AddMenu(name, IDR_POPUP_EXPLORER);
}

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class AboutDlg : public CDialogEx
{
public:
	enum { IDD = IDD_ABOUTBOX };

	AboutDlg() : CDialogEx(IDD) {}
};

void BinaryEditMfcApp::OnAppAbout()
{
	AboutDlg about_dlg;
	about_dlg.DoModal();
}
