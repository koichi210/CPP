
// LauncherDlg.h : ヘッダー ファイル
//

#pragma once

#define UNDEFINE_IDX	-1	// IDXを指定しない
#define LABEL_MAX		10	// Perforce以外のグループ登録可能数
#define RESOURCE_MAX	20	// リソースの登録可能数
#define RESOURCE_MAX_OF_DLG	RESOURCE_MAX+LABEL_MAX*RESOURCE_MAX+LABEL_MAX*RESOURCE_MAX	
#define LABEL_MAX_OF_DLG	LABEL_MAX*RESOURCE_MAX	

#define BUTTON_WIDTH	100	// ボタンの幅
#define BUTTON_HEIGHT	20	// ボタンの高さ
#define LABEL_OFFSET	20	// ラベルを表示する時、上に配置されているItemとのオフセット値

#ifdef DEBUG
 #define INI_FILE_NAME	"../Release/Launcher.ini"
#else
 #define INI_FILE_NAME	"./Launcher.ini"
#endif

#define SECTION_PERFORCE "Perforce"		// セクション名：Perforce
#define SECTION_SETTING "Setting"		// セクション名：Item
#define SECTION_BACKGROUND "Background"	// セクション名：BackGround起動
#define KEY_RADIO_LABEL "RadioLabel"	// キー名：Label
#define KEY_LABEL "Label"				// キー名：Label
#define KEY_BUTTON "Button"				// キー名：Button
#define KEY_RADIO "Radio"				// キー名：RadioButton
#define KEY_VALUE "Value"				// キー名：対象のItemのパス
#define KEY_NOT_EXPLORER "NotExplorer"	// キー名：起動アプリはExplorerでは無い

// ResourceID
#define IDBT_PERFORCE_BUTTON	2000
#define IDBT_PERFORCE_RADIO		3000
#define IDBT_BUTTON				4000
#define RESOURCE_ID_MAX			999

#define EXE_EXPLORER	"explorer"

// ボタン名とパスの関係
typedef struct{
	UINT32 Id;
	UINT32 bNotExplorer;
	CString name;
	CString path;
}Resource;

// ボタンの定義
class CCtrl
{
public :
	CString	m_LabelName;
	Resource m_Resource[RESOURCE_MAX];
	UINT32 m_nRegistNum;			// リソース最大数
};

// Perforceの定義
class CPerforce
{
public :
	CCtrl m_Radio;
	CCtrl m_Button[LABEL_MAX];
};


// CLauncherDlg ダイアログ -------------------------------------------------------------------------------------
class CLauncherDlg : public CDialogEx
{
public:
	CLauncherDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	virtual ~CLauncherDlg();
	enum { IDD = IDD_LAUNCHER_DIALOG };	// ダイアログ データ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

private :
	HICON m_hIcon;
	CPerforce m_perforce;
	CCtrl m_Button[LABEL_MAX];
	CString m_Background[RESOURCE_MAX];
	UINT32 m_CurrentHeight;		// ダイアログの高さ
	UINT32 m_PeforceBottom;		// Perforceの一番下の座標（y）
	UINT32 m_nRadioId;			// Perforceのラジオボタン定義
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

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};


// CLauncherReadFile Iniファイルからデータを読み込むヒト ---------------------------------------------------------
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
