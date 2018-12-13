
// LauncherDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char glCommand[MAX_PATH];	// 実際に叩くコマンド
int  glbNotExplorer = 0;

// CLauncherDlg ダイアログ -------------------------------------------------------------------------------------------------
CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_CurrentHeight = 0;
	m_PeforceBottom = 0;
	m_nRadioId = 0;
	m_RegistResourceMax = 0;
	m_RegistLabelMax = 0;

	for ( UINT i=0; i<RESOURCE_MAX; i++ )
	{
		m_perforce.m_Radio.m_Resource[i].Id = 0;
		m_perforce.m_Radio.m_Resource[i].bNotExplorer = 0;
		m_perforce.m_Radio.m_Resource[i].name = "";
		m_perforce.m_Radio.m_Resource[i].path = "";
		m_perforce.m_Radio.m_nRegistNum = 0;
	}

	for ( UINT i=0; i<LABEL_MAX; i++ )
	{
		m_perforce.m_Button[i].m_LabelName = "";
		for ( UINT j=0; j<RESOURCE_MAX; j++ )
		{
			m_perforce.m_Button[i].m_Resource[j].Id = 0;
			m_perforce.m_Button[i].m_Resource[j].bNotExplorer = 0;
			m_perforce.m_Button[i].m_Resource[j].name = "";
			m_perforce.m_Button[i].m_Resource[j].path = "";
			m_perforce.m_Button[i].m_nRegistNum = 0;
		}
	}

	for ( UINT i=0; i<LABEL_MAX; i++ )
	{
		m_Button[i].m_LabelName = "";
		for ( UINT j=0; j<RESOURCE_MAX; j++ )
		{
			m_Button[i].m_Resource[j].Id = 0;
			m_Button[i].m_Resource[j].bNotExplorer = 0;
			m_Button[i].m_Resource[j].name = "";
			m_Button[i].m_Resource[j].path = "";
			m_Button[i].m_nRegistNum = 0;
		}
	}

	for (UINT i=0; i<RESOURCE_MAX_OF_DLG; i++)
	{
		m_pButton[i] = NULL;
	}
}

CLauncherDlg::~CLauncherDlg()
{
	for (UINT i=0; i<RESOURCE_MAX_OF_DLG; i++)
	{
		if ( m_pButton[i] )
		{
			delete m_pButton[i];
			m_pButton[i] = NULL;
		}
	}
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// CLauncherDlg メッセージ ハンドラー
BEGIN_MESSAGE_MAP(CLauncherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, IDBT_PERFORCE_RADIO, IDBT_PERFORCE_RADIO+RESOURCE_ID_MAX, PushRadioButton)
	ON_CONTROL_RANGE(BN_CLICKED, IDBT_PERFORCE_BUTTON, IDBT_PERFORCE_BUTTON+RESOURCE_ID_MAX, PushPefroceButton)
	ON_CONTROL_RANGE(BN_CLICKED, IDBT_BUTTON, IDBT_BUTTON+RESOURCE_ID_MAX, PushButton)
END_MESSAGE_MAP()

BOOL CLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	SetTopLeft();		// 左上に表示
	if ( ReadIniFile() )		// iniファイルから設定値を読み込む
	{
		CreateResource();	// Resource作成

		// ラジオボタンの初期値
		PushRadioButton(m_perforce.m_Radio.m_Resource[0].Id);
	}
	else
	{
		MessageBox("設定ファイルを読めませんでした");
	}

	// Backgroundで起動するアプリ
	for ( UINT i=0; i<RESOURCE_MAX; i++ )
	{
		if ( m_Background[i].Compare("") == 0 )
		{
			// これ以上定義が無い
			break;
		}
		WinExec(m_Background[i], SW_HIDE);
	}
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CPen pen(PS_SOLID, 2, RGB(0,0,255));
		CPen* oldpen=dc.SelectObject(&pen);
		POINT pt;

		pt.x=0;
		pt.y=m_PeforceBottom;
		dc.MoveTo(pt);

		pt.x=BUTTON_WIDTH;
		dc.LineTo(pt);
		dc.SelectObject(&oldpen);

		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLauncherDlg::SetTopLeft()
{
	CRect rect;
	GetWindowRect(&rect);
	SetWindowPos(&wndTop, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
}

BOOL CLauncherDlg::ReadIniFile()
{
	CLauncherReadFile read(INI_FILE_NAME);

	if ( ! read.IsExitFile() )
	{
		return FALSE;
	}

	// Perforce(Radio)
	read.ReadResource(KEY_RADIO_LABEL, SECTION_PERFORCE, KEY_RADIO, &m_perforce.m_Radio, RESOURCE_MAX, IDBT_PERFORCE_RADIO);

	// Perforce(Data)
	for ( UINT i=0; i<LABEL_MAX; i++ )
	{
		read.ReadResource(KEY_LABEL, SECTION_PERFORCE, KEY_BUTTON, &m_perforce.m_Button[i], RESOURCE_MAX, IDBT_PERFORCE_BUTTON, UNDEFINE_IDX, i);
	}

	// Common(Data)
	for ( UINT i=0; i<LABEL_MAX; i++ )
	{
		read.ReadResource(KEY_LABEL, SECTION_SETTING, KEY_BUTTON, &m_Button[i], RESOURCE_MAX, IDBT_BUTTON, i);
	}

	// Background
	for ( UINT i=0; i<RESOURCE_MAX; i++ )
	{
		if ( ! read.ReadBackgroundResource(SECTION_BACKGROUND, KEY_VALUE, &m_Background[i], i) )
		{
			// これ以上定義がない
			break;
		}
	}

	return TRUE;
}

void CLauncherDlg::CreateResource()
{
	// m_perforce.m_Radio
	if ( m_perforce.m_Radio.m_LabelName.Compare("") )
	{
		CreateLabel(m_perforce.m_Radio.m_LabelName);
		for ( UINT32 i=0; i<m_perforce.m_Radio.m_nRegistNum; i++ )
		{
			CreateResource(m_perforce.m_Radio.m_Resource[i], BS_RADIOBUTTON);
		}
	}

	// m_perforce.m_Button
	for ( UINT32 i=0; i<LABEL_MAX; i++ )
	{
		if ( m_perforce.m_Button[i].m_LabelName.Compare("") )
		{
			CreateLabel(m_perforce.m_Button[i].m_LabelName);
			for ( UINT32 j=0; j<m_perforce.m_Button[i].m_nRegistNum; j++ )
			{
				CreateResource(m_perforce.m_Button[i].m_Resource[j], BS_PUSHBUTTON);
			}
		}
	}
	m_CurrentHeight += LABEL_OFFSET;
	m_PeforceBottom = m_CurrentHeight;

	// m_Button
	for ( UINT32 i=0; i<LABEL_MAX; i++ )
	{
		if ( m_Button[i].m_LabelName.Compare("") )
		{
			CreateLabel(m_Button[i].m_LabelName);
			for ( UINT32 j=0; j<m_Button[i].m_nRegistNum; j++ )
			{
				CreateResource(m_Button[i].m_Resource[j], BS_PUSHBUTTON);
			}
		}
	}
}

void CLauncherDlg::CreateLabel(CString LabelName)
{
	RECT rect;
	ScreenToClient(&rect);
	GetRect(&rect, LABEL_OFFSET);

	m_pStatic[m_RegistLabelMax] = new CStatic;
    m_pStatic[m_RegistLabelMax]->Create(LabelName, WS_VISIBLE, rect, this, 0);
	m_RegistLabelMax++;
}

void CLauncherDlg::CreateResource(Resource res, DWORD dwStyle)
{
	RECT rect;
	ScreenToClient(&rect);
	GetRect(&rect);

	m_pButton[m_RegistResourceMax] = new CButton();	
	m_pButton[m_RegistResourceMax]->Create(res.name, dwStyle | WS_VISIBLE, rect, this, res.Id);
	m_RegistResourceMax++;
}

void CLauncherDlg::GetRect(RECT *rect, UINT32 offset)
{
	rect->top = m_CurrentHeight + offset;
	rect->bottom = rect->top + BUTTON_HEIGHT;
	rect->left = 0;
	rect->right = BUTTON_WIDTH;

	m_CurrentHeight = rect->bottom;
}

void CLauncherDlg::PushRadioButton(UINT Id)
{
	UpdateRadioButton(Id);
	m_nRadioId = Id;
}

void CLauncherDlg::UpdateRadioButton(UINT Id)
{
	for(UINT32 i=0; i<m_perforce.m_Radio.m_nRegistNum;i++)
	{
		m_pButton[i]->SetCheck(BST_UNCHECKED);
	}

	UINT32 idx = GetIdx(Id, &m_perforce.m_Radio);
	m_pButton[idx]->SetCheck(BST_CHECKED);
}

void CLauncherDlg::PushPefroceButton(UINT Id)
{
	CString workspace = GetPath(m_nRadioId, &m_perforce.m_Radio);
	CString path = GetPath(Id, &m_perforce.m_Button[0]);
	workspace += path;
	Execute(workspace);
}

void CLauncherDlg::PushButton(UINT Id)
{
	Resource res = GetResource(Id, &m_Button[0]);
	Execute(res.path, res.bNotExplorer);
}

UINT32 CLauncherDlg::GetIdx(UINT32 Id, CCtrl *res)
{
	for ( UINT32 j=0; j<res->m_nRegistNum; j++ )
	{
		if ( Id == res->m_Resource[j].Id )
		{
			return j;
		}
	}

	MessageBox("not found Comomn Id");
	return 0;
}

CString CLauncherDlg::GetPath(UINT32 Id, CCtrl *res)
{
	for ( UINT32 i=0; i<LABEL_MAX; i++,res++ )
	{
		for ( UINT32 j=0; j<res->m_nRegistNum; j++ )
		{
			if ( Id == res->m_Resource[j].Id )
			{
				return res->m_Resource[j].path;
			}
		}
	}

	MessageBox("not found Comomn Id");
	return NULL;
}

Resource CLauncherDlg::GetResource(UINT32 Id, CCtrl *res)
{
	for ( UINT32 i=0; i<LABEL_MAX; i++,res++ )
	{
		for ( UINT32 j=0; j<res->m_nRegistNum; j++ )
		{
			if ( Id == res->m_Resource[j].Id )
			{
				return res->m_Resource[j];
			}
		}
	}

	MessageBox("not found Comomn Id");
	return res->m_Resource[0];
}

void CLauncherDlg::Execute(const char *path, BOOL bNotExplorer)
{
	memset(&glCommand[0], 0, sizeof(glCommand));
#if 0 // org
	if ( ! bNotExplorer )
	{
		sprintf_s(glCommand, "%s %s", EXE_EXPLORER, path);
	}
	else
	{
		sprintf_s(glCommand, "%s", path);
	}

	//WinExec(glCommand, SW_HIDE);
	//ShellExecute(NULL, "open", "sakura", glCommand, NULL, SW_SHOWNORMAL);

	//スレッドにしてもダメみたい
	DWORD Id;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, &Id);
	//_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))ThreadProc, 0, 0, (unsigned int*)&Id);
#else

	glbNotExplorer = bNotExplorer;
	switch(glbNotExplorer)
	{
	case 0 :
//		sprintf_s(glCommand, "%s %s", EXE_EXPLORER, path);
		sprintf_s(glCommand, "%s", path);
		break;

	case 1 :
		sprintf_s(glCommand, "%s", path);
		break;

	case 2 :
		sprintf_s(glCommand, "%s", path);
		break;

	default :
		break;
	}

	//スレッドにしてもダメみたい
	DWORD Id;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, &Id);
#endif
}


// CLauncherReadFile ----------------------------------------------------------------------------------------------------------
CLauncherReadFile::CLauncherReadFile(CString iniFileName)
{
	m_iniFileName = iniFileName;
}

CLauncherReadFile::~CLauncherReadFile()
{
}

BOOL CLauncherReadFile::IsExitFile()
{
	return IsExistFile(m_iniFileName);
}

BOOL CLauncherReadFile::ReadBackgroundResource(CString section, CString key, CString *pPath, UINT LabelIdx)
{
	BOOL bFound = FALSE;
	CString SerchSection = section;
	CString SerchKey = key;
	Resource rcData;

	if ( ! GetDataDefine(&rcData, section, NULL, SerchKey, NULL, LabelIdx) )
	{
		if ( rcData.path.Compare("") != 0 )
		{
			*pPath = rcData.path;
			bFound = TRUE;
		}
	}

	return bFound;
}

void CLauncherReadFile::ReadResource(CString Label, CString section, CString key, CCtrl *pCtrl, UINT CtrlMax, UINT ResourceId, UINT SectionIdx, UINT LabelIdx)
{
	UINT32 coef = 0;
	CString SerchSection = section;
	if ( SectionIdx != UNDEFINE_IDX )
	{
		coef += SectionIdx * 100;
		SerchSection.Format("%s%d", section, SectionIdx);
	}

	CString SerchLabel = Label;
	CString SerchKey = key;
	CString SerchKeyValue = KEY_VALUE;
	CString SerchKeyNotExplorer = KEY_NOT_EXPLORER;
	if ( LabelIdx != UNDEFINE_IDX )
	{
		coef += LabelIdx * 100;
		SerchLabel.Format("%s[%d]", Label, LabelIdx);
		SerchKey.Format("%s[%d]", key, LabelIdx);
		SerchKeyValue.Format("%s[%d]", KEY_VALUE, LabelIdx);
		SerchKeyNotExplorer.Format("%s[%d]", KEY_NOT_EXPLORER, LabelIdx);
	}

	// まずはラベルを取ってくる
	CString LabelName;
	if ( ! GetLabelDefine(&LabelName, SerchSection, SerchLabel) )
	{
		// 定義が無い
		return;
	}
	pCtrl->m_LabelName = LabelName;

	// ラベルに紐づくボタン定義を取ってくる
	Resource rcData;
	for ( UINT j=0; j<CtrlMax; j++ )
	{
		if ( ! GetDataDefine(&rcData, SerchSection, SerchKey, SerchKeyValue, SerchKeyNotExplorer, j) )
		{
			// 定義がない
			break;
		}
		pCtrl->m_Resource[j].name = rcData.name;
		pCtrl->m_Resource[j].path = rcData.path;
		pCtrl->m_Resource[j].bNotExplorer = rcData.bNotExplorer;
		pCtrl->m_Resource[j].Id = ResourceId + pCtrl->m_nRegistNum + coef;

		pCtrl->m_nRegistNum = pCtrl->m_nRegistNum + 1;
	}
}

BOOL CLauncherReadFile::GetLabelDefine(CString *Label, CString Section,  CString key)
{
	return GetStringForIniFile(Label, Section, key);
}

BOOL CLauncherReadFile::GetDataDefine(Resource *ButtonData, CString Section, CString key, CString keyValue, CString keyExplorer, UINT DataIdx)
{
	BOOL bReturn = FALSE;
	BOOL bReturnName = FALSE;
	BOOL bReturnPath = FALSE;
	CString temp;

	bReturnName = GetStringForIniFile(&temp, Section, key, DataIdx);
	ButtonData->name = temp;

	bReturnPath = GetStringForIniFile(&temp, Section, keyValue, DataIdx);
	ButtonData->path = temp;

	DWORD Value;
	GetDigitForIniFile(&Value, Section, keyExplorer, DataIdx);
	ButtonData->bNotExplorer = Value;

	if ( bReturnName && bReturnPath )
	{
		bReturn = TRUE;
		
	}
	return bReturn;
}

BOOL CLauncherReadFile::GetDigitForIniFile(DWORD *Value, CString Section, CString Key, UINT DataIdx)
{
	CString Data;
	BOOL bReturn = GetStringForIniFile(&Data, Section, Key, DataIdx);
	if ( Value )
	{
		*Value = StrToHex(Data.GetString());
	}

	return bReturn;
}

BOOL CLauncherReadFile::GetStringForIniFile(CString *str, CString Section, CString key, UINT DataIdx)
{
	CString SerchKey = key;
	if( DataIdx != UNDEFINE_IDX )
	{
		SerchKey.Format("%s[%d]", key, DataIdx);
	}

	return GetStringForIniFile(str, Section, SerchKey);
}

BOOL CLauncherReadFile::GetStringForIniFile(CString *str, CString Section, CString Key)
{
	char buff[MAX_PATH];
	BOOL bReturn = GetPrivateProfileString(Section, Key, NULL, buff, sizeof(buff), m_iniFileName);

	if ( str )
	{
		str->SetString(buff);
	}

	return bReturn;
}

ULONG CLauncherReadFile::StrToHex(const char * str)
{
	ULONG ulValue=0;
	sscanf_s(str,"%x", &ulValue);

	return ulValue;
}

void ThreadProc()
{
#if 0
	system(glCommand);
	//WinExec(glCommand, SW_HIDE);
	//ShellExecute(NULL, "open", "sakura", glCommand, NULL, SW_SHOWNORMAL);
#else
	switch(glbNotExplorer)
	{
	case 0 :
		ShellExecute(NULL, "open", EXE_EXPLORER, glCommand, NULL, SW_SHOWNORMAL);
//		system(glCommand);
		break;

	case 1 :
		ShellExecute(NULL, "open", "sakura", glCommand, NULL, SW_SHOWNORMAL);
		break;

	case 2 :
		system(glCommand);
		break;

	default :
		break;
	}
#endif
	ExitThread(0); 
}

BOOL IsExistFile(CString filename)
{
	BOOL bReturn = FALSE;
	CStdioFile	file;

	// IsExistみたいなAPIがあったような。。
	if ( file.Open( filename, CFile::modeRead | CFile::typeText) )
	{
		bReturn = TRUE;
		file.Close();
	}

	return bReturn;
}
