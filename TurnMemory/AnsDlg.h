#if !defined(AFX_ANSDLG_H__10E74F65_7B5F_4506_A68B_6EB9A9E4D00E__INCLUDED_)
#define AFX_ANSDLG_H__10E74F65_7B5F_4506_A68B_6EB9A9E4D00E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnsDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// AnsDlg �_�C�A���O

class AnsDlg : public CDialog
{
// �R���X�g���N�V����
public:
	AnsDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void CheckProc();
	CTurnMemoryDlg *pParent;
	int judge[CELL_MAX*CELL_MAX];

// �_�C�A���O �f�[�^
	//{{AFX_DATA(AnsDlg)
	enum { IDD = IDD_ANS };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(AnsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(AnsDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ANSDLG_H__10E74F65_7B5F_4506_A68B_6EB9A9E4D00E__INCLUDED_)
