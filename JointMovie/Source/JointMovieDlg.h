// JointMovieDlg.h : ヘッダー ファイル
//

#pragma once

#define GROUP_MAX	(8)

// CJointMovieDlg ダイアログ
class CJointMovieDlg : public CDialog
{
// コンストラクション
public:
	CJointMovieDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_JOINTMOVIE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnExecute();
	DECLARE_MESSAGE_MAP()
};
