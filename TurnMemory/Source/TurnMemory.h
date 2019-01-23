// TurnMemory.h : TURNMEMORY アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_TURNMEMORY_H__6E461B87_B055_43F8_84C0_9B42194B7434__INCLUDED_)
#define AFX_TURNMEMORY_H__6E461B87_B055_43F8_84C0_9B42194B7434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryApp:
// このクラスの動作の定義に関しては TurnMemory.cpp ファイルを参照してください。
//

class CTurnMemoryApp : public CWinApp
{
public:
	CTurnMemoryApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTurnMemoryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CTurnMemoryApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TURNMEMORY_H__6E461B87_B055_43F8_84C0_9B42194B7434__INCLUDED_)
