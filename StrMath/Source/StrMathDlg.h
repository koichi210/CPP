// StrMathDlg.h : ヘッダー ファイル
//

#if !defined(AFX_STRMATHDLG_H__4BEDFCF0_EC9E_47F9_968D_AC1AA9E3ED68__INCLUDED_)
#define AFX_STRMATHDLG_H__4BEDFCF0_EC9E_47F9_968D_AC1AA9E3ED68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// 定義
#define DEF1	"いち"
#define DEF2	"に"
#define DEF3	"さん"
#define DEF4	"よん"
#define DEF5	"ご"
#define DEF6A	"ろく"
#define DEF6B	"ろっ"
#define DEF7	"なな"
#define DEF8A	"はち"
#define DEF8B	"はっ"
#define DEF9	"きゅう"
#define DEF10	"じゅう"
#define DEF100A	"ひゃく"
#define DEF100B	"びゃく"
#define DEF100C	"ぴゃく"

#define SUM		1
#define SUB		2
#define STR_BUFF	256
#define SHOW_WEIGHT	16				//表示文字列の幅
#define SHOW_HEIGHT	(SHOW_WEIGHT)	//表示文字列の高さ

void GetString(int num, LPSTR str);
void GetString2(int num, LPSTR str, BOOL flg);

/////////////////////////////////////////////////////////////////////////////
// CStrMathDlg ダイアログ

class CStrMathDlg : public CDialog
{
// 構築
public:
	CStrMathDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CStrMathDlg)
	enum { IDD = IDD_STRMATH_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStrMathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	void BuiltNumber(int * num);

	int num1;
	int num2;
	int keta;
	int mark;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStrMathDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnStart();
	afx_msg void OnSum();
	afx_msg void OnSub();
	afx_msg void On2keta();
	afx_msg void On3keta();
	afx_msg void OnAns();
	afx_msg void OnHlp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRMATHDLG_H__4BEDFCF0_EC9E_47F9_968D_AC1AA9E3ED68__INCLUDED_)
