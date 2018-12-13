// zodiac.h : ZODIAC アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_ZODIAC_H__3FFB9082_4534_4A0D_911F_45F6F34D33F4__INCLUDED_)
#define AFX_ZODIAC_H__3FFB9082_4534_4A0D_911F_45F6F34D33F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CZodiacApp:
// このクラスの動作の定義に関しては zodiac.cpp ファイルを参照してください。
//

class CZodiacApp : public CWinApp
{
public:
	CZodiacApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CZodiacApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CZodiacApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ZODIAC_H__3FFB9082_4534_4A0D_911F_45F6F34D33F4__INCLUDED_)
