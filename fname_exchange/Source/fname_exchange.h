// fname_exchange.h : FNAME_EXCHANGE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_FNAME_EXCHANGE_H__45AC3F65_5AF9_4C42_9187_E74D82503712__INCLUDED_)
#define AFX_FNAME_EXCHANGE_H__45AC3F65_5AF9_4C42_9187_E74D82503712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CFname_exchangeApp:
// このクラスの動作の定義に関しては fname_exchange.cpp ファイルを参照してください。
//

class CFname_exchangeApp : public CWinApp
{
public:
	CFname_exchangeApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFname_exchangeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CFname_exchangeApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FNAME_EXCHANGE_H__45AC3F65_5AF9_4C42_9187_E74D82503712__INCLUDED_)
