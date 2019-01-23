// StrMath.h : STRMATH アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STRMATH_H__28139293_0EB6_4B5C_A315_4C6821FBD7E5__INCLUDED_)
#define AFX_STRMATH_H__28139293_0EB6_4B5C_A315_4C6821FBD7E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CStrMathApp:
// このクラスの動作の定義に関しては StrMath.cpp ファイルを参照してください。
//

class CStrMathApp : public CWinApp
{
public:
	CStrMathApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStrMathApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CStrMathApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRMATH_H__28139293_0EB6_4B5C_A315_4C6821FBD7E5__INCLUDED_)
