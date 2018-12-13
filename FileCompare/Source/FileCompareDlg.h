
// FileCompareDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"

#define MAX_FILE_NUM		(100000)
#define MAX_FOLDER_NUM		(20)
#define MAX_FOLDER_PATH		(5000)
#define MAX_FILE_READ_VALUE	(1024)
#define UNKNOWN_SAME		(0)
#define END_OF_LINE			"\r\n"
#define STR_DEFALUT_PATH	"C:\\windows;D:\\download;E:\\tmp"
#define POS_END				(-1)
#define POS_INIT			(0)

#define STR_START			"比較"
#define STR_STOP			"停止"

typedef struct FileList{
	int		nFindSame;
	CString strFileName;
}FileList;

UINT ProcThread(LPVOID pParam);

// CFileCompareDlg ダイアログ
class CFileCompareDlg : public CDialogEx
{
// コンストラクション
public:
	HICON m_hIcon;
	int m_SameGroup;						// 同じファイルの塊（idx）
	CProgressCtrl mdx_progress;				// プログレスバー
	BOOL m_bProc;							// 処理フラグ（FALSE=停止要求）
// ダイアログ データ
	enum { IDD = IDD_FILECOMPARE_DIALOG };
	FileList m_FileList[MAX_FILE_NUM];		// 全ファイルのフルパス
	int		 m_FileNum;						// 対象ファイルの全体数
	CString m_FolderList[MAX_FOLDER_NUM];	// 対象フォルダのリスト


// 実装
public:
	CFileCompareDlg(CWnd* pParent = NULL);	// 標準コンストラクター

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	void StopProc();	// 停止処理
	void InitProc();	// 初期化処理
	void GetFolder();	// フォルダ一覧を取得
	void GetFile();		// ファイル一覧を取得
	void Compare();		// ファイル比較の親
	int GetProgressBarEnd();	// プログレスバーの終了位置
	void UpdateProgressBar(int nPos);		// プログレスバーの進捗
	void OutputResult(BOOL bClear = FALSE);	// 結果の画面表示
	void Result();		// 終了処理
	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedExecute();
	DECLARE_MESSAGE_MAP()
};
