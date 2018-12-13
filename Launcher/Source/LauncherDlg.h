
// LauncherDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#define UNDEFINE_IDX	-1	// IDX���w�肵�Ȃ�
#define LABEL_MAX		10	// Perforce�ȊO�̃O���[�v�o�^�\��
#define RESOURCE_MAX	20	// ���\�[�X�̓o�^�\��
#define RESOURCE_MAX_OF_DLG	RESOURCE_MAX+LABEL_MAX*RESOURCE_MAX+LABEL_MAX*RESOURCE_MAX	
#define LABEL_MAX_OF_DLG	LABEL_MAX*RESOURCE_MAX	

#define BUTTON_WIDTH	100	// �{�^���̕�
#define BUTTON_HEIGHT	20	// �{�^���̍���
#define LABEL_OFFSET	20	// ���x����\�����鎞�A��ɔz�u����Ă���Item�Ƃ̃I�t�Z�b�g�l

#ifdef DEBUG
 #define INI_FILE_NAME	"../Release/Launcher.ini"
#else
 #define INI_FILE_NAME	"./Launcher.ini"
#endif

#define SECTION_PERFORCE "Perforce"		// �Z�N�V�������FPerforce
#define SECTION_SETTING "Setting"		// �Z�N�V�������FItem
#define SECTION_BACKGROUND "Background"	// �Z�N�V�������FBackGround�N��
#define KEY_RADIO_LABEL "RadioLabel"	// �L�[���FLabel
#define KEY_LABEL "Label"				// �L�[���FLabel
#define KEY_BUTTON "Button"				// �L�[���FButton
#define KEY_RADIO "Radio"				// �L�[���FRadioButton
#define KEY_VALUE "Value"				// �L�[���F�Ώۂ�Item�̃p�X
#define KEY_NOT_EXPLORER "NotExplorer"	// �L�[���F�N���A�v����Explorer�ł͖���

// ResourceID
#define IDBT_PERFORCE_BUTTON	2000
#define IDBT_PERFORCE_RADIO		3000
#define IDBT_BUTTON				4000
#define RESOURCE_ID_MAX			999

#define EXE_EXPLORER	"explorer"

// �{�^�����ƃp�X�̊֌W
typedef struct{
	UINT32 Id;
	UINT32 bNotExplorer;
	CString name;
	CString path;
}Resource;

// �{�^���̒�`
class CCtrl
{
public :
	CString	m_LabelName;
	Resource m_Resource[RESOURCE_MAX];
	UINT32 m_nRegistNum;			// ���\�[�X�ő吔
};

// Perforce�̒�`
class CPerforce
{
public :
	CCtrl m_Radio;
	CCtrl m_Button[LABEL_MAX];
};


// CLauncherDlg �_�C�A���O -------------------------------------------------------------------------------------
class CLauncherDlg : public CDialogEx
{
public:
	CLauncherDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[
	virtual ~CLauncherDlg();
	enum { IDD = IDD_LAUNCHER_DIALOG };	// �_�C�A���O �f�[�^

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

private :
	HICON m_hIcon;
	CPerforce m_perforce;
	CCtrl m_Button[LABEL_MAX];
	CString m_Background[RESOURCE_MAX];
	UINT32 m_CurrentHeight;		// �_�C�A���O�̍���
	UINT32 m_PeforceBottom;		// Perforce�̈�ԉ��̍��W�iy�j
	UINT32 m_nRadioId;			// Perforce�̃��W�I�{�^����`
	CButton *m_pButton[RESOURCE_MAX_OF_DLG];
	CStatic *m_pStatic[LABEL_MAX_OF_DLG];
	UINT32 m_RegistResourceMax;
	UINT32 m_RegistLabelMax;

	void SetTopLeft();
	BOOL ReadIniFile();
	void CreateResource();
	void CreateLabel(CString LabelName);
	void CreateResource(Resource res, DWORD dwStyle);
	void GetRect(RECT *rect, UINT32 offset=0);

	void PushRadioButton(UINT Id);
	void UpdateRadioButton(UINT Id);
	void PushPefroceButton(UINT Id);
	void PushButton(UINT Id);
	UINT32 CLauncherDlg::GetIdx(UINT32 Id, CCtrl *res);
	CString GetPath(UINT32 Id, CCtrl *res);
	Resource GetResource(UINT32 Id, CCtrl *res);
	void Execute(const char *path, BOOL bNotExplorer = FALSE);

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};


// CLauncherReadFile Ini�t�@�C������f�[�^��ǂݍ��ރq�g ---------------------------------------------------------
class CLauncherReadFile
{
public:
	CLauncherReadFile(CString iniFileName);
	virtual ~CLauncherReadFile();
	BOOL ReadBackgroundResource(CString section, CString key, CString *pPath, UINT LabelIdx);
	void ReadResource(CString Label, CString section, CString key, CCtrl *pCtrl, UINT CtrlMax, UINT ResourceId, UINT SectionIdx=UNDEFINE_IDX, UINT KeyIdx=UNDEFINE_IDX);
	BOOL IsExitFile();

private :
	CString m_iniFileName;
	BOOL GetLabelDefine(CString *Label, CString Section,  CString key);
	BOOL GetDataDefine(Resource *ButtonData, CString Section, CString key, CString keyValue, CString KeyNotExplorer, UINT DataIdx);
	BOOL GetDigitForIniFile(DWORD *Value, CString Section, CString Key, UINT DataIdx);
	BOOL GetStringForIniFile(CString *str, CString Section, CString key, UINT DataIdx);
	BOOL GetStringForIniFile(CString *str, CString Section, CString Key);
	ULONG StrToHex(const char * str);
};

void ThreadProc();
BOOL IsExistFile(CString filename);
