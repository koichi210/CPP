// TurnMemoryDlg.h : ヘッダー ファイル
//

#if !defined(AFX_TURNMEMORYDLG_H__DBAA0EB2_1E2F_47F7_88D1_F83B473736BE__INCLUDED_)
#define AFX_TURNMEMORYDLG_H__DBAA0EB2_1E2F_47F7_88D1_F83B473736BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// 定義
#define INIT		0
#define START		1
#define STOP		2
#define END			3
#define EXIT		3

#define STR_BUFF	256

#define CELL_MAX	10
#define CELL_MIN	3

#define DEF_PRB		4

#define EVENT_SHOW	1
#define EVENT_WAIT	2

#define SHOW_INVAL	1000
#define WAIT_INVAL	1000
#define WAIT_MAX	5

#define KEEP_MIND_STR		"順番を記憶して下さい。"
#define CNT_REMEMBER_STR	"秒間覚えて下さい。"
#define START_STR			"スタートを押して下さい。"
#define END_STR				"解答を入力して下さい。"
#define START_BUTTON		"スタート"
#define STOP_BUTTON			"ストップ"

#define CurCellMax(a)	(a * a)
/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryDlg ダイアログ

class CTurnMemoryDlg : public CDialog
{
// 構築
public:
	CTurnMemoryDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTurnMemoryDlg)
	enum { IDD = IDD_TURNMEMORY_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTurnMemoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

public:
	int	anstbl[CELL_MAX*CELL_MAX];
	int	intbl[CELL_MAX*CELL_MAX];
	int prb_num;
	int states;
	int m_cnt;

	void EndProc();
	void ExitProc();
	void InitProc();
	void BuildNumber();
	void Refresh();
	void ShowProc();
	void WaitProc();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTurnMemoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnAns();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TURNMEMORYDLG_H__DBAA0EB2_1E2F_47F7_88D1_F83B473736BE__INCLUDED_)
