// BackUpDiffDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define CURRENT_IDX				(-1)
#define MAXNUM_IDX				(50)
#define LC_VAL_WIDTH			(25)	//���X�g�R���g���[���̕�(No)
#define LC_STR_WIDTH			(170)	//���X�g�R���g���[���̕�(path)

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

#define STR_ENABLE_BK			"�o�b�N�A�b�v�L��"
#define STR_NUMBER				"No"
#define STR_ORG_PATH			"���t�H���_"
#define STR_NEW_PATH			"��t�H���_"
#define STR_SUBDIRECTORY		"�T�u�f�B���N�g�����Ώ�"
#define STR_DIFF_FILE			"�����t�@�C���̂ݑΏ�"
#define STR_OVERWRITE			"�㏑���̊m�F��\��"
#define STR_ON					"�L"
#define STR_OFF					"��"
#define STR_ENABLE				"��"
#define STR_DISABLE				"�~"
#define	STR_SAVE_SET_SUCCESS	"�ݒ�l��ۑ����܂����B"
#define	STR_SAVE_SET_ERROR		"�ݒ�l�ۑ��Ɏ��s���܂����B"

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

// CBackUpDiffDlg �_�C�A���O
class CBackUpDiffDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CBackUpDiffDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
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


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
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
