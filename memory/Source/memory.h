// memory.h : MEMORY アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_MEMORY_H__D06AC81B_FFBF_4291_88C3_591F8BC2365B__INCLUDED_)
#define AFX_MEMORY_H__D06AC81B_FFBF_4291_88C3_591F8BC2365B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMemoryApp:
// このクラスの動作の定義に関しては memory.cpp ファイルを参照してください。
//

class CMemoryApp : public CWinApp
{
public:
	CMemoryApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMemoryApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMORY_H__D06AC81B_FFBF_4291_88C3_591F8BC2365B__INCLUDED_)
