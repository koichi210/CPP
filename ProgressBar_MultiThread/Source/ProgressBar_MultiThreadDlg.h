
// ProgressBar_MultiThreadDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"

#define PROGRESS_MAX	(100000)

// CProgressBarDlg �_�C�A���O
class CProgressBarDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CProgressBarDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROGRESSBAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_stop ;
	CProgressCtrl m_ddx_progress;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	CString m_StatusBar;
};

UINT ProcThread(LPVOID pParam);
