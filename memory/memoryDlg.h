// memoryDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_MEMORYDLG_H__B30A7A81_A157_4530_9E1F_C0528EF9B54E__INCLUDED_)
#define AFX_MEMORYDLG_H__B30A7A81_A157_4530_9E1F_C0528EF9B54E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "memoryDef.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoryDlg �_�C�A���O

class CMemoryDlg : public CDialog
{
// �\�z
public:
	CMemoryDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMemoryDlg)
	enum { IDD = IDD_MEMORY_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMemoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMemoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnAns();
	virtual void OnOK();
	afx_msg void OnKeisan();
	afx_msg void OnAnki();
	afx_msg void OnEngSmall();
	afx_msg void OnEngLarge();
	afx_msg void OnNumber();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHelp();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// ��`
public:
	char record[PR_NUM_MAX][KETA_ANIKI_MAX+2] ;	//�\�������l���o���Ă���('\0'��'\n'���l��)
	int timer_cyc ;		//�^�C�}�N������
	int count ;			//���݂̌�
	int states ;		//��
	int num ;			//�\����(�o�萔)
	int keta ;			//����
	int type ;			//���� and �A���t�@�x�b�g
	int old_mode ;		//�ȑO�̃��[�h
	int new_mode ;		//�V�������[�h
	CScrollBar cyc_bar ;	// �\�����x��ݒ�
	void ViewText() ;	//�\���֐�
	void InitProc() ;	//����������
	void StartProc() ;	//�J�n����
	void EndProc() ;	//�I������
	void ItemSts(BOOL flg) ;	// UI�֑̋�
	void KeyGen(char str[KETA_ANIKI_MAX]) ;	// �\�������쐬
	int GetKeyGenType(int val) ;		// KeyGen�̃^�C�v�擾
	void GetKeyGenVal(char * wk, int str_type, int number, int val) ;	// KeyGen�̒l�擾

	void MatchProc(int org_val,int * new_val) ;	// ���ꕶ������菈��
	int StartCheck() ;	//�J�n�ł��邩�ݒ�l���`�F�b�N
	void SetType(int type, BOOL flg) ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMORYDLG_H__B30A7A81_A157_4530_9E1F_C0528EF9B54E__INCLUDED_)
