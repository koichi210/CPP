
// DivisionCouplingDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"

#define UNIT_VALUE (1024)	//	KB

#define POS_END				(-1)
#define POS_INIT			(0)

// CDivisionCouplingDlg �_�C�A���O
class CDivisionCouplingDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CDivisionCouplingDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[
	int			m_fDivSize;		// �����T�C�Y
	int			m_fRestSize;	// �����c��T�C�Y
	int			m_nIdx;			// �������̃C���f�b�N�X
	CString		m_fNameOrg;		// �������t�@�C����
	CString		m_fNameNew;		// ������t�@�C����
	CStdioFile	m_cstOrgFile;	// �������t�@�C���|�C���^
	CStdioFile	m_cstNewFile;	// ������t�@�C���|�C���^
	int			m_Err;			// �������̃G���[
	int			m_MaxNum;		// ���������t�@�C���̐��i�v���O���X�o�[�p�j
	BOOL		m_bProc;		// �������t���O
	enum {				// Err
		//NO_ERROR		 = 0,	// WinError.h�Œ�`�ς�
		ERR_OPEN_ORGFILE = 1,
		ERR_OPEN_NEWFILE = 2,
		ERR_CALLOC		 = 3,
		ERR_DIV_SIZE	 = 4
	};

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIVISION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
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

// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
