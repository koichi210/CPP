// zodiacDlg.h : ヘッダー ファイル
//

#if !defined(AFX_ZODIACDLG_H__6E535628_9651_4FD1_AE54_155DD1AE66CD__INCLUDED_)
#define AFX_ZODIACDLG_H__6E535628_9651_4FD1_AE54_155DD1AE66CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MIN_YEAR		1900
#define MAX_YEAR		2100
#define MIN_AGE			0
#define MAX_AGE			130
#define CHINEZODIAC_NUM	12

#define ZOD_NAME_BUFF	14
#define STR_BUFF		256
#define VIEW_BUFF		512
#define VIEW_ALL_BUFF	3072

#define DEFAULT_AGE		20
#define DEFAULT_BIRTH	1980

#define FIX_BIRTH	1
#define FIX_AGE		2
#define FIX_ZODIAC	3

#define AGE_FORMAT		"満%d才"
#define BIRTH_FORMAT	"%d年"


static const char zod_name[CHINEZODIAC_NUM][MAX_PATH] = { "子(ねずみ)", "丑(うし)", "寅(とら)", "卯(うさぎ)", "辰(たつ)", "巳(み)",
												"午(うま)", "未(ひつじ)", "申(さる)", "酉(とり)", "戌(いぬ)", "亥(いのしし)"};

/////////////////////////////////////////////////////////////////////////////
// CZodiacDlg ダイアログ

class CZodiacDlg : public CDialog
{
// 構築
public:
	CZodiacDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	int GetZodiac(int year, char * zod);
	void Refresh();
	void SetListData(char *str, int idx);
	int chk_states;
	int m_year;

// ダイアログ データ
	//{{AFX_DATA(CZodiacDlg)
	enum { IDD = IDD_ZODIAC_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CZodiacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CZodiacDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAge();
	afx_msg void OnChineZodiac();
	afx_msg void OnView();
	afx_msg void OnBirth();
	afx_msg void OnSelchangeYear();
	afx_msg void OnAllView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ZODIACDLG_H__6E535628_9651_4FD1_AE54_155DD1AE66CD__INCLUDED_)
