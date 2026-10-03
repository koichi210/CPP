// JointMovie.cpp : アプリケーションクラス

#include "stdafx.h"
#include "joint_movie.h"
#include "joint_movie_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(JointMovieApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

JointMovieApp the_app;

BOOL JointMovieApp::InitInstance()
{
	// ComCtl32.dll Version 6 を使うマニフェストの場合、これが無いとウィンドウ作成に失敗する
	INITCOMMONCONTROLSEX init_ctrls = {};
	init_ctrls.dwSize = sizeof(init_ctrls);
	init_ctrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&init_ctrls);

	CWinApp::InitInstance();

	JointMovieDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログを閉じたらメッセージポンプを開始せずに終了する
	return FALSE;
}
