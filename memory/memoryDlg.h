// memoryDlg.h : ヘッダー ファイル
//

#if !defined(AFX_MEMORYDLG_H__B30A7A81_A157_4530_9E1F_C0528EF9B54E__INCLUDED_)
#define AFX_MEMORYDLG_H__B30A7A81_A157_4530_9E1F_C0528EF9B54E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "memoryDef.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoryDlg ダイアログ

class CMemoryDlg : public CDialog
{
// 構築
public:
	CMemoryDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMemoryDlg)
	enum { IDD = IDD_MEMORY_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMemoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnAns();
	virtual void OnOK();
	afx_msg void OnKeisan();
	afx_msg void OnAnki();
	afx_msg void OnEngSmall();
	afx_msg void OnEngLarge();
	afx_msg void OnNumber();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHelp();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// 定義
public:
	char record[PR_NUM_MAX][KETA_ANIKI_MAX+2] ;	//表示した値を覚えておく('\0'と'\n'を考慮)
	int timer_cyc ;		//タイマ起動周期
	int count ;			//現在の個数
	int states ;		//状況
	int num ;			//表示回数(出題数)
	int keta ;			//桁数
	int type ;			//数字 and アルファベット
	int old_mode ;		//以前のモード
	int new_mode ;		//新しいモード
	CScrollBar cyc_bar ;	// 表示速度を設定
	void ViewText() ;	//表示関数
	void InitProc() ;	//初期化処理
	void StartProc() ;	//開始処理
	void EndProc() ;	//終了処理
	void ItemSts(BOOL flg) ;	// UIの禁則
	void KeyGen(char str[KETA_ANIKI_MAX]) ;	// 表示文字作成
	int GetKeyGenType(int val) ;		// KeyGenのタイプ取得
	void GetKeyGenVal(char * wk, int str_type, int number, int val) ;	// KeyGenの値取得

	void MatchProc(int org_val,int * new_val) ;	// 同一文字内被り処理
	int StartCheck() ;	//開始できるか設定値をチェック
	void SetType(int type, BOOL flg) ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMORYDLG_H__B30A7A81_A157_4530_9E1F_C0528EF9B54E__INCLUDED_)
