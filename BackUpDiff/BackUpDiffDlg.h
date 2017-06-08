// BackUpDiffDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define CURRENT_IDX				(-1)
#define MAXNUM_IDX				(50)
#define LC_VAL_WIDTH			(25)	//リストコントロールの幅(No)
#define LC_STR_WIDTH			(170)	//リストコントロールの幅(path)

#define SUBDIR_MASK				(0x1)
#define DIFF_MASK				(0x2)
#define OVERWRITE_MASK			(0x4)

#define END_NONE				(0)
#define END_APP					(1)
#define END_REBOOT				(2)
#define END_SHUTDOWN			(3)


#define SUBITEM_ENABLE_BK		(0)
#define SUBITEM_NUMBER			(1)
#define SUBITEM_SRC_PATH		(2)
#define SUBITEM_DST_PATH		(3)
#define SUBITEM_SUBDIRECTORY	(4)
#define SUBITEM_DIFF_FILE		(5)
#define SUBITEM_OVERWRITE		(6)

#define STR_ENABLE_BK			"バックアップ有効"
#define STR_NUMBER				"No"
#define STR_ORG_PATH			"元フォルダ"
#define STR_NEW_PATH			"先フォルダ"
#define STR_SUBDIRECTORY		"サブディレクトリも対象"
#define STR_DIFF_FILE			"差分ファイルのみ対象"
#define STR_OVERWRITE			"上書きの確認を表示"
#define STR_ON					"有"
#define STR_OFF					"無"
#define STR_ENABLE				"○"
#define STR_DISABLE				"×"
#define	STR_SAVE_SET_SUCCESS	"設定値を保存しました。"
#define	STR_SAVE_SET_ERROR		"設定値保存に失敗しました。"

#define SET_FILE_NAME			"\\BackUpDiff.dat"
#define BAT_FILE_NAME			"\\BackUpDiff.bat"

typedef CMap <int,int,int,int>	CMap_IdToIndex;

typedef struct tagBACKUP
{
	BOOL	bBkEnable;
	CString	strSrcPath;
	CString	strDstPath;
	DWORD	opt;

} BACKUP;

// CBackUpDiffDlg ダイアログ
class CBackUpDiffDlg : public CDialog
{
// コンストラクション
public:
	CBackUpDiffDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_BACKUPDIFF_DIALOG };

private:
	CMap_IdToIndex mapPaperSize;
	CMap_IdToIndex mapPaperType;
	CListCtrl m_listctl;
	int m_idx;
	int m_end;
	int m_FileOut;
	BACKUP m_bkStruct[MAXNUM_IDX];
	BACKUP m_bkDefault;


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBrowseSrc();
	afx_msg void OnBrowseDest();
	afx_msg void OnDiff();
	afx_msg void OnBackupStart();
	afx_msg void OnBnClickedSaveSetting();
	afx_msg void OnBnClickedSubdir();
	afx_msg void OnBnClickedOverWrite();
	afx_msg void OnBnClickedEnableBK();
	afx_msg void OnEnChangeEditSrc();
	afx_msg void OnEnChangeEditDst();
	afx_msg void OnBnClickedAllClear();
	afx_msg void OnBnClickedEndNone();
	afx_msg void OnBnClickedEndReboot();
	afx_msg void OnBnClickedEndShutdown();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	BOOL GetDirectory(TCHAR * dir);
	BOOL BackUpProc();
	void LCSetData();
	void InsertListCtrl(LV_COLUMN lvCol, int idx, CString name);
	BOOL ReadSetting();
	BOOL WriteSetting();
	void WriteBatchFile(CString cmd);
	void UpdateEnableBK(BOOL bChk, int idx=CURRENT_IDX);
	void UpdateSubDirectory(DWORD opt, int idx=CURRENT_IDX);
	void UpdateDiffFile(DWORD opt, int idx=CURRENT_IDX);
	void UpdateOverWrite(DWORD opt, int idx=CURRENT_IDX);
	void UpdateSrcPath(char *str, int idx=CURRENT_IDX);
	void UpdateDstPath(char *str, int idx=CURRENT_IDX);
	void Refresh();
	int GetIdx(int idx);
	BOOL IsBlankData(BACKUP *BkStruct);

public:
	afx_msg void OnBnClickedEndApp();
};
