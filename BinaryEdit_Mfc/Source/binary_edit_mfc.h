// binary_edit_mfc.h : アプリケーションクラス

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"

class CBinaryEdit_MfcApp : public CWinAppEx
{
public:
	CBinaryEdit_MfcApp();

	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;
	virtual void PreLoadState() override;

	UINT	m_nAppLook = 0;			// 選択中の外観（ID_VIEW_APPLOOK_*）
	BOOL	m_bHiColorIcons = TRUE;	// 24bit カラーのアイコン・ツールバーを使う

protected:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBinaryEdit_MfcApp theApp;
