
// TabControlDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"

#include "Child1.h"
#include "Child2.h"


// CTabControlDlg �_�C�A���O
class CTabControlDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CTabControlDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TABCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_tab1;
	CTabCtrl m_tab2;
	CChild1 m_Child1;
	CChild2 m_Child2;

};
