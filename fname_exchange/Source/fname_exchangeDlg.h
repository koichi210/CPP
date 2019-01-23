// fname_exchangeDlg.h : �w�b�_�[ �t�@�C��
//

#include "utils.h"

#if !defined(AFX_FNAME_EXCHANGEDLG_H__DCB04DFD_CE1F_4735_947F_BDFD20365F38__INCLUDED_)
#define AFX_FNAME_EXCHANGEDLG_H__DCB04DFD_CE1F_4735_947F_BDFD20365F38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// ��`
#define DEFAULT_ALERT BST_CHECKED

#define ENUM	1
#define ALLSBCS 2
#define ALLDBCS	3
#define DELDIST	4
#define DELNUM	5
#define ADD		6
#define DEL		7
#define REP		8

#define BEF		1
#define AFT		2
#define KEEP	1

#define STR_BUFF 256
#define DIST_MAX 5
#define UNDO_MAX 100
#define FILE_MAX 250
#define KETA_MAX 5

#define TARGET_FILE		1
#define TARGET_DIRECTOY	2

#define HORIZONTAIL_OFFSET 6

typedef struct _EXCH{
	CString ofname[FILE_MAX];	// org file name
	CString nfname[FILE_MAX];	// new file name
	int		num;				// ��x�ɕϊ������t�@�C���̐��i���s�{�^���������Ƃ��Ɏ��s�����t�@�C�����j
}EXCH;

typedef struct _UNDO{
	int  maxnum;		// ���s�{�^����������
	int  curnum;	// ���݂̕�����
	EXCH exch[UNDO_MAX];
}UNDO;

const TABLE tbl[] =
{
	{ IDST_DIR,            IDSTR_DIR},
	{ IDBT_BROWSE,         IDSTR_BROWSE},
	{ IDST_FILE_LIST,      IDSTR_FILE_LIST},
	{ IDBT_GET_FILE,       IDSTR_GET_FILE},
	{ IDBT_ALL_CHECK,      IDSTR_ALL_CHECK},
	{ IDBT_ALL_UNCHECK,    IDSTR_ALL_UNCHECK},
	{ IDGR_SYSTEM_SET,     IDSTR_SYSTEM_SET},
	{ IDCH_IGNORE_ALERT,   IDSTR_IGNORE_ALERT},
	{ IDCH_COMP_BIG_SMALL, IDSTR_COMP_BIG_SMALL},
	{ IDGR_HOWTO_CHANGE,   IDSTR_HOWTO_CHANGE},
	{ IDRB_ENUM,           IDSTR_ENUM},
	{ IDST_FIRST_NUM,      IDSTR_FIRST_NUM},
	{ IDCH_KEEP_NAME,      IDSTR_KEEP_NAME},
	{ IDRB_DEL_NUM,        IDSTR_DEL_NUM},
	{ IDST_DEL_BEF_NUM,    IDSTR_DEL_BEF_NUM},
	{ IDST_DEL_AFT_NUM,    IDSTR_DEL_AFT_NUM},
	{ IDRB_DEL_DIST,       IDSTR_DEL_DIST},
	{ IDRB_ADD,            IDSTR_ADD},
	{ IDCH_ADD_BEF,        IDSTR_ADD_BEF},
	{ IDCH_ADD_AFT,        IDSTR_ADD_AFT},
	{ IDRB_DEL,            IDSTR_DEL},
	{ IDRB_REP,            IDSTR_REP},
	{ IDST_NAME1,          IDSTR_NAME_ADD},
	{ IDST_NAME2,          IDSTR_NAME_REP_AFT},
	{ IDBT_EXE,            IDSTR_EXE},
	{ IDBT_UNDO,           IDSTR_UNDO},
	{ IDBT_END,            IDSTR_END}
};

const char sbcs[] =	("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!\"#$%&'()-+/*=?_<>,.;:[]{}");
const char dbcs[] = _T("�����������������������������������������������������`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�P�Q�R�S�T�U�V�W�X�O�I���h���������f�i�j�|�{�^�����H�Q�����C�D�G�F�m�n�o�p");
//{
//	("a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
//	"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
//	"1","2","3","4","5","6","7","8","9","0",
//	"!","\"","#","$","%","&","'","(",")","-","+","/","*","=","?","_","<",">",",",".",";",":","[","]","{","}"
//};

/////////////////////////////////////////////////////////////////////////////
// CFname_exchangeDlg �_�C�A���O

class CFname_exchangeDlg : public CDialog
{
// �\�z
public:
	CFname_exchangeDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CFname_exchangeDlg)
	enum { IDD = IDD_FNAME_EXCHANGE_DIALOG };
	CListBox	m_list;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFname_exchangeDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL
	void Refresh();
	void GetSetting();
	void ExChangeProc();
	void EnumProc();
	void SbcsProc();
	void DbcsProc();
	void DelNumProc();
	void DelDistProc();
	void AddProc();
	void DelProc();
	void RepProc();
	void UndoProc();
	void GetFileList();
	int GetKeta(int number);	// ���l�̌����擾
	void MoveFileProc();
	void ReplaceString2(CString oname, CString *nname, BOOL bDB2SB);


	CString m_oname;	// old name
	CString m_nname;	// new name
	int		m_Target;	// �Ώہi�t�@�C�� or �t�H���_�j
	int		m_type;		// ���W�I�{�^���̑I��l
	DWORD	m_opt;		// �����I�v�V����
	UINT	m_first_num;	//�y�ʂ��ԍ��t���z�ŏ��̐��l
	UINT	m_cur_num;		//�y�ʂ��ԍ��t���z�J�����g�̐��l
	int		m_keta;
	UINT 	m_Bef_Del_num;	//�y�w�蕶�����폜�z�擪����̒l
	UINT 	m_Aft_Del_num;	//�y�w�蕶�����폜�z�㕔����̒l
	INT_PTR	m_list_cnt;		// �����Ώۂ̃t�@�C����
	BOOL	m_ignore_alert;	// �x���𖳎�����
	BOOL	m_comp;			// �啶������������ʂ���
	CString m_file_name[FILE_MAX];
	char	m_dir[STR_BUFF];
	char	m_name1[STR_BUFF];
	char	m_name2[STR_BUFF];
	UNDO	m_undo;

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFname_exchangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetfocus();
	afx_msg void OnBrowse();
	afx_msg void OnGetFile();
	afx_msg void OnFile();
	afx_msg void OnFolder();
	afx_msg void OnIgnoreAlert();
	afx_msg void OnEnd();
	afx_msg void OnRadioAdd();
	afx_msg void OnRadioDel();
	afx_msg void OnRadioRep();
	afx_msg void OnExecute();
	afx_msg void OnRADIOEnum();
	afx_msg void OnRADIOAllDbcs();
	afx_msg void OnRADIOAllSbcs();
	afx_msg void OnRADIODelEnum();
	afx_msg void OnRADIODelDist();
	afx_msg void OnUndo();
	afx_msg void OnAllCheck();
	afx_msg void OnAllUncheck();
	afx_msg void OnCompBigSmall();
	void InitUndo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FNAME_EXCHANGEDLG_H__DCB04DFD_CE1F_4735_947F_BDFD20365F38__INCLUDED_)
