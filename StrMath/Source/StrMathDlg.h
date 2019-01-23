// StrMathDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_STRMATHDLG_H__4BEDFCF0_EC9E_47F9_968D_AC1AA9E3ED68__INCLUDED_)
#define AFX_STRMATHDLG_H__4BEDFCF0_EC9E_47F9_968D_AC1AA9E3ED68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// ��`
#define DEF1	"����"
#define DEF2	"��"
#define DEF3	"����"
#define DEF4	"���"
#define DEF5	"��"
#define DEF6A	"�낭"
#define DEF6B	"���"
#define DEF7	"�Ȃ�"
#define DEF8A	"�͂�"
#define DEF8B	"�͂�"
#define DEF9	"���イ"
#define DEF10	"���イ"
#define DEF100A	"�ЂႭ"
#define DEF100B	"�тႭ"
#define DEF100C	"�҂Ⴍ"

#define SUM		1
#define SUB		2
#define STR_BUFF	256
#define SHOW_WEIGHT	16				//�\��������̕�
#define SHOW_HEIGHT	(SHOW_WEIGHT)	//�\��������̍���

void GetString(int num, LPSTR str);
void GetString2(int num, LPSTR str, BOOL flg);

/////////////////////////////////////////////////////////////////////////////
// CStrMathDlg �_�C�A���O

class CStrMathDlg : public CDialog
{
// �\�z
public:
	CStrMathDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CStrMathDlg)
	enum { IDD = IDD_STRMATH_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CStrMathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	void BuiltNumber(int * num);

	int num1;
	int num2;
	int keta;
	int mark;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CStrMathDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnStart();
	afx_msg void OnSum();
	afx_msg void OnSub();
	afx_msg void On2keta();
	afx_msg void On3keta();
	afx_msg void OnAns();
	afx_msg void OnHlp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STRMATHDLG_H__4BEDFCF0_EC9E_47F9_968D_AC1AA9E3ED68__INCLUDED_)
