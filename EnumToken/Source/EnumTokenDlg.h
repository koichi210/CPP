// EnumTokenDlg.h : ヘッダー ファイル
//

#if !defined(AFX_SECURITYDLG_H__A699A18B_2757_4552_B7EE_1256C77BF9FC__INCLUDED_)
#define AFX_SECURITYDLG_H__A699A18B_2757_4552_B7EE_1256C77BF9FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSecurityDlg ダイアログ

class CSecurityDlg : public CDialog
{
// 構築
public:
	CSecurityDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSecurityDlg)
	enum { IDD = IDD_SECURITY_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSecurityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSecurityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetproc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SECURITYDLG_H__A699A18B_2757_4552_B7EE_1256C77BF9FC__INCLUDED_)
