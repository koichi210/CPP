// TurnMemoryDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_TURNMEMORYDLG_H__DBAA0EB2_1E2F_47F7_88D1_F83B473736BE__INCLUDED_)
#define AFX_TURNMEMORYDLG_H__DBAA0EB2_1E2F_47F7_88D1_F83B473736BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// ��`
#define INIT		0
#define START		1
#define STOP		2
#define END			3
#define EXIT		3

#define STR_BUFF	256

#define CELL_MAX	10
#define CELL_MIN	3

#define DEF_PRB		4

#define EVENT_SHOW	1
#define EVENT_WAIT	2

#define SHOW_INVAL	1000
#define WAIT_INVAL	1000
#define WAIT_MAX	5

#define KEEP_MIND_STR		"���Ԃ��L�����ĉ������B"
#define CNT_REMEMBER_STR	"�b�Ԋo���ĉ������B"
#define START_STR			"�X�^�[�g�������ĉ������B"
#define END_STR				"�𓚂���͂��ĉ������B"
#define START_BUTTON		"�X�^�[�g"
#define STOP_BUTTON			"�X�g�b�v"

#define CurCellMax(a)	(a * a)
/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryDlg �_�C�A���O

class CTurnMemoryDlg : public CDialog
{
// �\�z
public:
	CTurnMemoryDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTurnMemoryDlg)
	enum { IDD = IDD_TURNMEMORY_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTurnMemoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

public:
	int	anstbl[CELL_MAX*CELL_MAX];
	int	intbl[CELL_MAX*CELL_MAX];
	int prb_num;
	int states;
	int m_cnt;

	void EndProc();
	void ExitProc();
	void InitProc();
	void BuildNumber();
	void Refresh();
	void ShowProc();
	void WaitProc();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTurnMemoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnAns();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TURNMEMORYDLG_H__DBAA0EB2_1E2F_47F7_88D1_F83B473736BE__INCLUDED_)
