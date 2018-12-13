
// ProgressBar_SingleThreadDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"

#define PROGRESS_MAX	(100000)

// CProgressBar_SingleThreadDlg �_�C�A���O
class CProgressBar_SingleThreadDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CProgressBar_SingleThreadDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

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
	CProgressCtrl m_ddx_progress;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
};
