
// FileCompareDlg.h : �w�b�_�[ �t�@�C��
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

#define STR_START			"��r"
#define STR_STOP			"��~"

typedef struct FileList{
	int		nFindSame;
	CString strFileName;
}FileList;

UINT ProcThread(LPVOID pParam);

// CFileCompareDlg �_�C�A���O
class CFileCompareDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	HICON m_hIcon;
	int m_SameGroup;						// �����t�@�C���̉�iidx�j
	CProgressCtrl mdx_progress;				// �v���O���X�o�[
	BOOL m_bProc;							// �����t���O�iFALSE=��~�v���j
// �_�C�A���O �f�[�^
	enum { IDD = IDD_FILECOMPARE_DIALOG };
	FileList m_FileList[MAX_FILE_NUM];		// �S�t�@�C���̃t���p�X
	int		 m_FileNum;						// �Ώۃt�@�C���̑S�̐�
	CString m_FolderList[MAX_FOLDER_NUM];	// �Ώۃt�H���_�̃��X�g


// ����
public:
	CFileCompareDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
	void StopProc();	// ��~����
	void InitProc();	// ����������
	void GetFolder();	// �t�H���_�ꗗ���擾
	void GetFile();		// �t�@�C���ꗗ���擾
	void Compare();		// �t�@�C����r�̐e
	int GetProgressBarEnd();	// �v���O���X�o�[�̏I���ʒu
	void UpdateProgressBar(int nPos);		// �v���O���X�o�[�̐i��
	void OutputResult(BOOL bClear = FALSE);	// ���ʂ̉�ʕ\��
	void Result();		// �I������
	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedExecute();
	DECLARE_MESSAGE_MAP()
};
