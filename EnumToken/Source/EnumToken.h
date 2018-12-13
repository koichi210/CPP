// EnumToken.h : SECURITY アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_SECURITY_H__B92FE000_4F5D_4C82_A9C8_36DBCC1FCEB3__INCLUDED_)
#define AFX_SECURITY_H__B92FE000_4F5D_4C82_A9C8_36DBCC1FCEB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSecurityApp:
// このクラスの動作の定義に関しては EnumToken.cpp ファイルを参照してください。
//

class CSecurityApp : public CWinApp
{
public:
	CSecurityApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSecurityApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CSecurityApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SECURITY_H__B92FE000_4F5D_4C82_A9C8_36DBCC1FCEB3__INCLUDED_)
