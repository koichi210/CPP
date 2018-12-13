
// ProgressBar_SingleThreadDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"

#define PROGRESS_MAX	(100000)

// CProgressBar_SingleThreadDlg ダイアログ
class CProgressBar_SingleThreadDlg : public CDialogEx
{
// コンストラクション
public:
	CProgressBar_SingleThreadDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_PROGRESSBAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ddx_progress;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
};
