
// BinaryEdit_Mfc.h : BinaryEdit_Mfc アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CBinaryEdit_MfcApp:
// このクラスの実装については、BinaryEdit_Mfc.cpp を参照してください。
//

class CBinaryEdit_MfcApp : public CWinAppEx
{
public:
	CBinaryEdit_MfcApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBinaryEdit_MfcApp theApp;
