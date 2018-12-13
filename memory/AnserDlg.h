#if !defined(AFX_ANSERDLG_H__F0A0A981_0BDB_4378_BC33_08C104CD441F__INCLUDED_)
#define AFX_ANSERDLG_H__F0A0A981_0BDB_4378_BC33_08C104CD441F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnserDlg.h : ヘッダー ファイル
//
#include "memoryDef.h"

//typedef struct{
//	char text[STR_BUFF] ;
//}ANSER;

/////////////////////////////////////////////////////////////////////////////
// CAnserDlg ダイアログ

class CAnserDlg : public CDialog
{
// コンストラクション
public:
	CAnserDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	BOOL cheat ;

// ダイアログ データ
	//{{AFX_DATA(CAnserDlg)
	enum { IDD = IDD_MEM_ANSER };
	CAnserDlg*	anser;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAnserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAnserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAnserCheck();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnAnsok();
	afx_msg void OnAnsShow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ANSERDLG_H__F0A0A981_0BDB_4378_BC33_08C104CD441F__INCLUDED_)
