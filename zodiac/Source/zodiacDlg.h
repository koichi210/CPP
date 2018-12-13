// zodiacDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_ZODIACDLG_H__6E535628_9651_4FD1_AE54_155DD1AE66CD__INCLUDED_)
#define AFX_ZODIACDLG_H__6E535628_9651_4FD1_AE54_155DD1AE66CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MIN_YEAR		1900
#define MAX_YEAR		2100
#define MIN_AGE			0
#define MAX_AGE			130
#define CHINEZODIAC_NUM	12

#define ZOD_NAME_BUFF	14
#define STR_BUFF		256
#define VIEW_BUFF		512
#define VIEW_ALL_BUFF	3072

#define DEFAULT_AGE		20
#define DEFAULT_BIRTH	1980

#define FIX_BIRTH	1
#define FIX_AGE		2
#define FIX_ZODIAC	3

#define AGE_FORMAT		"��%d��"
#define BIRTH_FORMAT	"%d�N"


static const char zod_name[CHINEZODIAC_NUM][MAX_PATH] = { "�q(�˂���)", "�N(����)", "��(�Ƃ�)", "�K(������)", "�C(����)", "��(��)",
												"��(����)", "��(�Ђ�)", "�\(����)", "��(�Ƃ�)", "��(����)", "��(���̂���)"};

/////////////////////////////////////////////////////////////////////////////
// CZodiacDlg �_�C�A���O

class CZodiacDlg : public CDialog
{
// �\�z
public:
	CZodiacDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
	int GetZodiac(int year, char * zod);
	void Refresh();
	void SetListData(char *str, int idx);
	int chk_states;
	int m_year;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CZodiacDlg)
	enum { IDD = IDD_ZODIAC_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CZodiacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CZodiacDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAge();
	afx_msg void OnChineZodiac();
	afx_msg void OnView();
	afx_msg void OnBirth();
	afx_msg void OnSelchangeYear();
	afx_msg void OnAllView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ZODIACDLG_H__6E535628_9651_4FD1_AE54_155DD1AE66CD__INCLUDED_)
