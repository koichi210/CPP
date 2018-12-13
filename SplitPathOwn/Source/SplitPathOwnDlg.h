
// SplitPathOwnDlg.h : ヘッダー ファイル
//

#pragma once


// CSplitPathOwnDlg ダイアログ
class CSplitPathOwnDlg : public CDialogEx
{
// コンストラクション
public:
	CSplitPathOwnDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_SPLITPATHOWN_DIALOG };

	void SplitPath( char *pFileFullPath, char *pDrive, char *pDir, char *pFile );

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
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

};
