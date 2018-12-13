
// DivisionCouplingDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"

#define UNIT_VALUE (1024)	//	KB

#define POS_END				(-1)
#define POS_INIT			(0)

// CDivisionCouplingDlg ダイアログ
class CDivisionCouplingDlg : public CDialogEx
{
// コンストラクション
public:
	CDivisionCouplingDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	int			m_fDivSize;		// 分割サイズ
	int			m_fRestSize;	// 分割残りサイズ
	int			m_nIdx;			// 分割時のインデックス
	CString		m_fNameOrg;		// 分割元ファイル名
	CString		m_fNameNew;		// 分割後ファイル名
	CStdioFile	m_cstOrgFile;	// 分割元ファイルポインタ
	CStdioFile	m_cstNewFile;	// 分割後ファイルポインタ
	int			m_Err;			// 処理中のエラー
	int			m_MaxNum;		// 分割したファイルの数（プログレスバー用）
	BOOL		m_bProc;		// 処理中フラグ
	enum {				// Err
		//NO_ERROR		 = 0,	// WinError.hで定義済み
		ERR_OPEN_ORGFILE = 1,
		ERR_OPEN_NEWFILE = 2,
		ERR_CALLOC		 = 3,
		ERR_DIV_SIZE	 = 4
	};

// ダイアログ データ
	enum { IDD = IDD_DIVISION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
public:
	void Initialize();
	void Result();
	void SplitProc();
	void MergeProc();
	void OnBrowse(UINT nID, BOOL bIsFileOpen);
	void EnableControl(BOOL bEnable);
	void UpdateProgressBar(int nPos);
	int GetProgressBarEnd();
	void SetProgressSplitFileNum();
	void SetProgressMergeFileNum();

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSplit();
	afx_msg void OnBnClickedMerge();
	afx_msg void OnBnClickedSplitBrowse();
	afx_msg void OnBnClickedMergeBrowse();
	CProgressCtrl mdx_progress;
	afx_msg void OnBnClickedStop();
};

UINT SplitProcThread(LPVOID pParam);
UINT MergeProcThread(LPVOID pParam);
