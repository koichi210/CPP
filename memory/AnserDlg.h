#if !defined(AFX_ANSERDLG_H__F0A0A981_0BDB_4378_BC33_08C104CD441F__INCLUDED_)
#define AFX_ANSERDLG_H__F0A0A981_0BDB_4378_BC33_08C104CD441F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnserDlg.h : �w�b�_�[ �t�@�C��
//
#include "memoryDef.h"

//typedef struct{
//	char text[STR_BUFF] ;
//}ANSER;

/////////////////////////////////////////////////////////////////////////////
// CAnserDlg �_�C�A���O

class CAnserDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CAnserDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	BOOL cheat ;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAnserDlg)
	enum { IDD = IDD_MEM_ANSER };
	CAnserDlg*	anser;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAnserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CAnserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAnserCheck();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnAnsok();
	afx_msg void OnAnsShow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ANSERDLG_H__F0A0A981_0BDB_4378_BC33_08C104CD441F__INCLUDED_)
