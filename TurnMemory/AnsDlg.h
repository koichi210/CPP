#if !defined(AFX_ANSDLG_H__10E74F65_7B5F_4506_A68B_6EB9A9E4D00E__INCLUDED_)
#define AFX_ANSDLG_H__10E74F65_7B5F_4506_A68B_6EB9A9E4D00E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnsDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// AnsDlg ダイアログ

class AnsDlg : public CDialog
{
// コンストラクション
public:
	AnsDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void CheckProc();
	CTurnMemoryDlg *pParent;
	int judge[CELL_MAX*CELL_MAX];

// ダイアログ データ
	//{{AFX_DATA(AnsDlg)
	enum { IDD = IDD_ANS };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(AnsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(AnsDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ANSDLG_H__10E74F65_7B5F_4506_A68B_6EB9A9E4D00E__INCLUDED_)
