// MacroToolDlg.cpp : 実装ファイル
//
#include "stdafx.h"
#include "MainDlg.h"
#include "MacroTool.h"
#include "MacroToolDlg.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ /////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
//	Create(CAboutDlg::IDD);
//	Create(CAboutDlg::IDD, pParent);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMainDlg メッセージ ハンドラ
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString ver;
	ver.LoadString(IDS_VERSION);
	GetDlgItem(IDC_VERSION)->SetWindowText(ver);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


// CMacroToolDlg ダイアログ ////////////////////////////////////////////////////////////////////////////////
CMacroToolDlg::CMacroToolDlg(CWnd* pParent /*=NULL*/, CString fName) : CDialog(CMacroToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_p = (CMainDlg*)pParent;
	m_idx = 0;
	m_Record = FALSE;
	m_fName = fName;
	m_idx = 0;
	m_IsDebug = FALSE;

	m_hDLLInst = LoadLibrary(DLL_NAME);
	StartKeyHook = NULL;
	StopKeyHook = NULL;
	StartMouseHook = NULL;
	StopMouseHook = NULL;
	DebugMode = NULL;
	if ( m_hDLLInst )
	{
#if USE_KEY_HOOK
		StartKeyHook = (StartKeyHookFUNC *)GetProcAddress(m_hDLLInst, "StartKeyHook");
		StopKeyHook = (StopKeyHookFUNC *)GetProcAddress(m_hDLLInst, "StopKeyHook");
#endif
		StartMouseHook = (StartMouseHookFUNC *)GetProcAddress(m_hDLLInst, "StartMouseHook");
		StopMouseHook = (StopMouseHookFUNC *)GetProcAddress(m_hDLLInst, "StopMouseHook");
		DebugMode = (DebugModeFUNC *)GetProcAddress(m_hDLLInst, "DebugMode");
	}

	//親から設定を継承
	RestoreDefault();
	RestoreDefault(&m_CopyPaste);
	for ( int i=0; i<MAXNUM_CLOUMN; i++ )
	{
		if ( m_p->m_strEvent[i].nEvent == EVENT_UNDEFINE )
		{
			break ;
		}
		memcpy(&m_tmpEvent[i], &m_p->m_strEvent[i], sizeof(m_tmpEvent[i]));
	}
}

CMacroToolDlg::~CMacroToolDlg()
{
	if ( m_Record )
	{
		StopRecord();
	}
}

void CMacroToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL, m_listctl);
	DDX_Control(pDX, IDCB_MOUSE, m_MouseArray);
	DDX_Control(pDX, IDET_KEY, m_etKey);
	DDX_Control(pDX, IDET_COMMENT, m_etComment);
	DDX_Control(pDX, IDCB_KEY, m_KeyArray);
}

BEGIN_MESSAGE_MAP(CMacroToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, OnLvnItemchangedListctrl)
	ON_BN_CLICKED(IDBT_LIST_INSERT, OnListInsert)
	ON_BN_CLICKED(IDBT_LIST_DELETE, OnListDelete)
	ON_BN_CLICKED(IDBT_LIST_COPY, OnListCopy)
	ON_BN_CLICKED(IDBT_LIST_PASTE, OnListPaste)
	ON_EN_CHANGE(IDET_COMMENT, OnEnChangeComment)
	ON_BN_CLICKED(IDRB_MOUSE, OnMouse)
	ON_CBN_SELCHANGE(IDCB_MOUSE, OnCbnlchangeMouse)
	ON_EN_CHANGE(IDET_MOUSE_X, OnEnChangeMouseX)
	ON_EN_CHANGE(IDET_MOUSE_Y, OnEnChangeMouseY)
	ON_BN_CLICKED(IDRB_KEY, OnKey)
	ON_CBN_SELCHANGE(IDCB_KEY, OnCbnlchangeKey)
	ON_EN_CHANGE(IDET_KEY, OnEnChangeKey)
	ON_BN_CLICKED(IDCH_KEY_SHIFT, OnKeyShift)
	ON_BN_CLICKED(IDCH_KEY_CTRL, OnKeyCtrl)
	ON_BN_CLICKED(IDCH_KEY_ALT, OnKeyAlt)
	ON_EN_CHANGE(IDET_EXECUTE, OnEnChangeExecute)
	ON_EN_CHANGE(IDET_SLEEP, OnEnChangeSleep)
	ON_EN_CHANGE(IDET_REPEAT_NUM, OnEnChangeRepeatNum)
	ON_EN_CHANGE(IDET_REPEAT_TIME, OnEnChangeRepeatTime)
	ON_BN_CLICKED(IDBT_ALLCLEAR, OnAllclear)
	ON_BN_CLICKED(IDBT_READ, OnRead)
	ON_BN_CLICKED(IDBT_WRITE, OnWrite)
	ON_BN_CLICKED(IDBT_HELP, OnHelp)
	ON_BN_CLICKED(IDBT_RECORD, OnRecord)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CMacroToolDlg メッセージ ハンドラ
void CMacroToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMacroToolDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMacroToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMacroToolDlg::OnLvnItemchangedListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ( pNMLV )
	{
		if ( pNMLV->iItem != m_idx )
		{
			m_idx = pNMLV->iItem;
			UpdateControl();
		}
	}
	*pResult = 0;
}

BOOL CMacroToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	Initialize();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


void CMacroToolDlg::Initialize()
{
	m_listctl.SetMaxColumnNum(MAXNUM_CLOUMN);
	m_listctl.EnableSelectRowAll();
	m_listctl.SetLabel(LABELIDX_NO, LABEL_WIDTH_NO, "No");
	m_listctl.SetLabel(LABELIDX_EXECUTE, LABEL_WIDTH_EXECUTE, "実行");
	m_listctl.SetLabel(LABELIDX_SLEEP, LABEL_WIDTH_SLEEP, "遅延時間");
	m_listctl.SetLabel(LABELIDX_EVENT, LABEL_WIDTH_EVENT, "イベント");
	m_listctl.SetLabel(LABELIDX_DETAIL, LABEL_WIDTH_DETAIL, "詳細設定");
	m_listctl.SetLabel(LABELIDX_COMMENT, LABEL_WIDTH_COMMENT, "コメント");

	m_listctl.FillRect(LABELIDX_NO, "");
	//m_listctl.FillRect(LABELIDX_EXECUTE, "");
	//m_listctl.FillRect(LABELIDX_EVENT, "");
	//m_listctl.FillRect(LABELIDX_DETAIL, "");
	//m_listctl.FillRect(LABELIDX_SLEEP, "");
#ifdef TODO //ListBoxの初期選択状態を作りたいが、選択されない（表示がおかしくなる）
	//m_listctl.SelectIdx(m_idx);
#endif
	m_etKey.DisableCopyAndPaste(TRUE);
	//m_etKey.SetLimitText(sizeof(m_tmpEvent[m_idx].evKey.key) - 1);

	CheckRadioButton(DEF_EVENT_START, DEF_EVENT_END, DEF_EVENT);
//	CheckRadioButton(DEF_MOUSE_START, DEF_MOUSE_END, DEF_MOUSE_CAP);
	CheckDlgButton(IDCH_EXECUTE, DEF_EXECUTE);

	SetDlgItemInt(IDET_REPEAT_NUM, m_p->m_nRepeatNum);
	SetDlgItemInt(IDET_REPEAT_TIME, m_p->m_nRepeatDelayMsec);
//	SetDlgItemInt(IDET_SLEEP, DEF_SLEEP_VALUE);
//	SetDlgItemInt(IDET_MOUSE_X, DEF_MOUSE_X_VALUE);
//	SetDlgItemInt(IDET_MOUSE_Y, DEF_MOUSE_Y_VALUE);

	//コンボボックス
	SendDlgItemMessage(IDCB_MOUSE, CB_RESETCONTENT, 0L, 0L);
	for( int i=0; i<MOUSE_ARRAY_MAX; i++ )
	{
		m_MouseArray.InsertString(i, arMouse[i]);
		m_MouseArray.SetItemData(i, i);
	}
	m_MouseArray.SetCurSel(0);

	SendDlgItemMessage(IDCB_KEY, CB_RESETCONTENT, 0L, 0L);
	for( int i=0; i<KEY_ARRAY_MAX; i++ )
	{
		m_KeyArray.InsertString(i, arKey[i]);
		m_KeyArray.SetItemData(i, i);
	}
	m_KeyArray.SetCurSel(0);

	if ( ! m_hDLLInst )
	{
		GetDlgItem(IDBT_RECORD)->EnableWindow(FALSE);
	}

	UpdateListControl(TRUE);
	UpdateControl();
//	SetTitleBar();
}

void CMacroToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}

void CMacroToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CMacroToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CString str;
	POINT pt;

	GetCursorPos(&pt);
	str.Format("(%4d:%4d)" ,pt.x, pt.y);
	GetDlgItem(IDLB_STATES)->SetWindowText(str);

	CDialog::OnMouseMove(nFlags, point);
}

void CMacroToolDlg::OnListInsert()
{
	int nLastIdx = MAXNUM_CLOUMN-1;
	
	if ( m_idx == nLastIdx )
	{
		MessageBox(STR_SELECT_LAST_CLOUMN);
		return ;
	}

	for (int i=nLastIdx; i>m_idx; i--)
	{
		memcpy(&m_tmpEvent[i], &m_tmpEvent[i-1],sizeof(m_tmpEvent[i]));
		
	}
	RestoreDefault(&m_tmpEvent[m_idx]);
	UpdateListControl(TRUE);
}

void CMacroToolDlg::OnListDelete()
{
	int nLastIdx = MAXNUM_CLOUMN-1;

	for (int i=m_idx; i<nLastIdx; i++)
	{
		memcpy(&m_tmpEvent[i], &m_tmpEvent[i+1],sizeof(m_tmpEvent[i]));
	}
	RestoreDefault(&m_tmpEvent[nLastIdx]);
	UpdateListControl(TRUE);
}

void CMacroToolDlg::OnListCopy()
{
	memcpy(&m_CopyPaste, &m_tmpEvent[m_idx], sizeof(m_CopyPaste));
}

void CMacroToolDlg::OnListPaste()
{
	memcpy(&m_tmpEvent[m_idx], &m_CopyPaste, sizeof(m_CopyPaste));
	UpdateListControl();
	UpdateControl();
}

void CMacroToolDlg::OnEnChangeComment()
{
	CString str;

	GetDlgItem(IDET_COMMENT)->GetWindowText(str);
	strcpy_s(m_tmpEvent[m_idx].comment, sizeof(m_tmpEvent[m_idx].comment), str);
	UpdateListControl();
}

void CMacroToolDlg::OnMouse()
{
	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nExe = 1;
	}
	m_tmpEvent[m_idx].nEvent = EVENT_MOUSE;

	UpdateControl();
	UpdateListControl();
}

void CMacroToolDlg::OnCbnlchangeMouse()
{
	m_tmpEvent[m_idx].evMouse.nOpe = m_MouseArray.GetCurSel();
	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_MOUSE;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeMouseX()
{
	BOOL bValid;
	int value;

	value = (int)GetDlgItemInt(IDET_MOUSE_X, &bValid, TRUE);
	if ( value > MAX_MOUSE_POINT_VALUE )
	{
		value = MAX_MOUSE_POINT_VALUE;

		CString tmp;
		tmp.Format("%d",value);
		GetDlgItem(IDET_MOUSE_X)->SetWindowText(tmp);
	}
	m_tmpEvent[m_idx].evMouse.pt.x = value;

	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_MOUSE;
//		m_tmpEvent[m_idx].evMouse.nOpe = DEF_MOUSE_CAP;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeMouseY()
{
	BOOL bValid;
	int value;

	value = (int)GetDlgItemInt(IDET_MOUSE_Y, &bValid, TRUE);
	if ( value > MAX_MOUSE_POINT_VALUE )
	{
		value = MAX_MOUSE_POINT_VALUE;

		CString tmp;
		tmp.Format("%d",value);
		GetDlgItem(IDET_MOUSE_Y)->SetWindowText(tmp);
	}
	m_tmpEvent[m_idx].evMouse.pt.y = value;

	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_MOUSE;
//		m_tmpEvent[m_idx].evMouse.nOpe = DEF_MOUSE_CAP;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnKey()
{
	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nExe = 1;
	}
	m_tmpEvent[m_idx].nEvent = EVENT_KEY;

	UpdateControl();
	UpdateListControl();
}

void CMacroToolDlg::OnCbnlchangeKey()
{
	m_tmpEvent[m_idx].evKey.keyEx = m_KeyArray.GetCurSel();
	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_KEY;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeKey()
{
	CString str;

	GetDlgItem(IDET_KEY)->GetWindowText(str);
	strcpy_s(m_tmpEvent[m_idx].evKey.key, sizeof(m_tmpEvent[m_idx].evKey.key), str);
	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_KEY;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnKeyShift()
{
	BOOL bOn = FALSE;

	if ( IsDlgButtonChecked(IDCH_KEY_SHIFT) == BST_CHECKED )
	{
		bOn = TRUE;
	}
	if ( bOn )
	{
		m_tmpEvent[m_idx].evKey.dwCap |= BF_CAPS_KEY_SHIFT;
	}
	else
	{
		m_tmpEvent[m_idx].evKey.dwCap &= ~BF_CAPS_KEY_SHIFT;
	}

	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_KEY;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnKeyCtrl()
{
	BOOL bOn = FALSE;

	if ( IsDlgButtonChecked(IDCH_KEY_CTRL) == BST_CHECKED )
	{
		bOn = TRUE;
	}
	if ( bOn )
	{
		m_tmpEvent[m_idx].evKey.dwCap |= BF_CAPS_KEY_CTRL;
	}
	else
	{
		m_tmpEvent[m_idx].evKey.dwCap &= ~BF_CAPS_KEY_CTRL;
	}

	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_KEY;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnKeyAlt()
{
	BOOL bOn = FALSE;

	if ( IsDlgButtonChecked(IDCH_KEY_ALT) == BST_CHECKED )
	{
		bOn = TRUE;
	}
	if ( bOn )
	{
		m_tmpEvent[m_idx].evKey.dwCap |= BF_CAPS_KEY_ALT;
	}
	else
	{
		m_tmpEvent[m_idx].evKey.dwCap &= ~BF_CAPS_KEY_ALT;
	}

	if ( m_tmpEvent[m_idx].nEvent == EVENT_UNDEFINE )
	{
		m_tmpEvent[m_idx].nEvent = EVENT_KEY;
		UpdateControl();
	}
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeExecute()
{
	BOOL bValid;
	int value;

	value = (int)GetDlgItemInt(IDET_EXECUTE, &bValid, FALSE);
	if ( value > EXECUTE_MAX )
	{
		value = EXECUTE_MAX;

		CString tmp;
		tmp.Format("%d",value);
		GetDlgItem(IDET_EXECUTE)->SetWindowText(tmp);
	}
	m_tmpEvent[m_idx].nExe = value;
	SetTitleBar();
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeSleep()
{
	BOOL bValid;
	int value;

	value = (int)GetDlgItemInt(IDET_SLEEP, &bValid, FALSE);
	if ( value > MAX_SLEEP_VALUE )
	{
		value = MAX_SLEEP_VALUE;

		CString tmp;
		tmp.Format("%d",value);
		GetDlgItem(IDET_SLEEP)->SetWindowText(tmp);
	}
	m_tmpEvent[m_idx].sleep = value;

	SetTitleBar();
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeRepeatNum()
{
	BOOL bValid;

	m_p->m_nRepeatNum = (int)GetDlgItemInt(IDET_REPEAT_NUM, &bValid, FALSE);
}

void CMacroToolDlg::OnEnChangeRepeatTime()
{
	BOOL bValid;

	m_p->m_nRepeatDelayMsec = (int)GetDlgItemInt(IDET_REPEAT_TIME, &bValid, FALSE);
	SetTitleBar();
}

void CMacroToolDlg::OnAllclear()
{
	RestoreDefault();
//	RestoreDefault();//予備
	UpdateListControl(TRUE);
	UpdateControl();
}

void CMacroToolDlg::RestoreDefault(EVENT *strEvent)
{
	EVENT strDefEvent;

	memset(&strDefEvent, 0, sizeof(strDefEvent));

	if ( strEvent == NULL )
	{
		memset(&m_tmpEvent, 0, sizeof(m_tmpEvent));
		for ( int i=0; i < MAXNUM_CLOUMN; i++)
		{
			memcpy(&m_tmpEvent[i], &strDefEvent, sizeof(m_tmpEvent[i]));
		}
	}
	else
	{
		memcpy(strEvent, &strDefEvent, sizeof(strDefEvent));
	}
}

void CMacroToolDlg::OnRead()
{
	if ( GetFileName(TRUE) )
	{
		ReadFile(m_fName);
	}
}


void CMacroToolDlg::ReadFile(CString FileName)
{
	char str[MAX_PATH];
	EVENT evt;
	int cnt=0;

	RestoreDefault();

	CString value = GetIniFileParam(FileName, INI_FILE_SECTION_COMMON, INI_FILE_KEY_REPLEAT_NUM);
	m_p->m_nRepeatNum = atoi (value);

	value = GetIniFileParam(FileName, INI_FILE_SECTION_COMMON, INI_FILE_KEY_REPEAT_DELAY_MSEC);
	m_p->m_nRepeatDelayMsec = atoi (value);

	for(int i=0; i < MAXNUM_CLOUMN; i++)
	{
		CString KeyName;
		KeyName.Format("%s%d",INI_FILE_KEY_TABLE_PARAM, i);
		CString value = GetIniFileParam(FileName, INI_FILE_SECTION_TABLE, KeyName);
		if ( value == "" )
		{
			break;
		}

		memset(&evt, 0, sizeof(evt));
		strcpy(str, value);
		SplitLogString(&evt, str, sizeof(str));

		if ( CheckLogFormat(evt) == TRUE )
		{
			switch(evt.evMouse.nOpe)
			{
			case MOUSE_ARRAY_MOVE :
				break;

			case MOUSE_ARRAY_LUP :
				if ( cnt > 0 )
				{
					// 前回がLDOWNだったら、前回の命令をLCLICKに変更
					if ( m_tmpEvent[cnt-1].evMouse.nOpe == MOUSE_ARRAY_LDOWN &&
							m_tmpEvent[cnt-1].evMouse.pt.x == evt.evMouse.pt.x &&
							m_tmpEvent[cnt-1].evMouse.pt.y == evt.evMouse.pt.y)
					{
						m_tmpEvent[cnt-1].evMouse.nOpe = MOUSE_ARRAY_LCLICK;
						continue;
					}
				}
				break;

			case MOUSE_ARRAY_RUP :
				if ( cnt > 0 )
				{
					// 前回がRDOWNだったら、前回の命令をRCLICKに変更
					if ( m_tmpEvent[cnt-1].evMouse.nOpe == MOUSE_ARRAY_RDOWN &&
							m_tmpEvent[cnt-1].evMouse.pt.x == evt.evMouse.pt.x &&
							m_tmpEvent[cnt-1].evMouse.pt.y == evt.evMouse.pt.y)
					{
						m_tmpEvent[cnt-1].evMouse.nOpe = MOUSE_ARRAY_RCLICK;
						continue;
					}
				}
				break;

			default :
				break;
			}

			memcpy(&m_tmpEvent[cnt], &evt, sizeof(m_tmpEvent[cnt]));
			cnt++;
		}
	}
	
	UpdateListControl(TRUE);
	UpdateControl();

	SetDlgItemInt(IDET_REPEAT_NUM, m_p->m_nRepeatNum);
	SetDlgItemInt(IDET_REPEAT_TIME, m_p->m_nRepeatDelayMsec);
}

void CMacroToolDlg::OnWrite()
{
	if ( ! GetFileName() )
	{
		return ;
	}
	
	SetIniFileParam(m_fName, INI_FILE_SECTION_COMMON, INI_FILE_KEY_REPLEAT_NUM, m_p->m_nRepeatNum);
	SetIniFileParam(m_fName, INI_FILE_SECTION_COMMON, INI_FILE_KEY_REPEAT_DELAY_MSEC, m_p->m_nRepeatDelayMsec);

	char str[MAX_PATH];
	int idx = 0;
	for(int i=0; i < MAXNUM_CLOUMN; i++)
	{
		if ( m_tmpEvent[i].nEvent == EVENT_UNDEFINE )
		{
			continue;
		}
		MakeLogString(&m_tmpEvent[i], str, sizeof(str));

		CString KeyName;
		KeyName.Format("%s%d",INI_FILE_KEY_TABLE_PARAM, idx);
		SetIniFileParam(m_fName, INI_FILE_SECTION_TABLE, KeyName, str);
		idx++;
	}
}


void CMacroToolDlg::SplitLogString(EVENT *pEvt, char *str, int sz)
{
	char *pstr;

	pstr = strtok(str, ",;");
	if ( pstr )
	{
		pEvt->nExe = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->sleep = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->nEvent = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->evMouse.pt.x = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->evMouse.pt.y = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->evMouse.nOpe = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->evKey.dwCap = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		pEvt->evKey.keyEx = atoi(pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		strcpy(pEvt->evKey.key, pstr);
	}

	pstr = strtok(NULL, ",;");
	if ( pstr )
	{
		strcpy(pEvt->comment, pstr);
	}
}


void CMacroToolDlg::MakeLogString(EVENT *pEvt, char *str, int sz)
{
	sprintf_s(str, sz, STR_LOG_FMT,
		pEvt->nExe,
		pEvt->sleep,
		pEvt->nEvent,
		pEvt->evMouse.pt.x,
		pEvt->evMouse.pt.y,
		pEvt->evMouse.nOpe,
		pEvt->evKey.dwCap,
		pEvt->evKey.keyEx,
		pEvt->evKey.key,
		pEvt->comment);
}


BOOL CMacroToolDlg::GetFileName(BOOL bRead)
{
	BOOL bReturn = FALSE;
	CStdioFile	file;

	CFileDialog dlg(bRead, NULL, m_fName, OFN_HIDEREADONLY, STR_DLG_FILTER);
	dlg.m_ofn.lpstrTitle = STR_DLG_TITLE;
	
	if ( dlg.DoModal() == IDOK )
	{
		m_fName = dlg.GetPathName();

		if ( ! bRead )
		{
			char drv[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

			_splitpath_s(m_fName, drv, sizeof(drv), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
			if ( strcmp(ext, "") == 0 )
			{
				m_fName += STR_EXT;
			}
		}
		bReturn = TRUE;
	}
	return bReturn;
}


BOOL CMacroToolDlg::CheckLogFormat(EVENT evt)
{
	BOOL bReturn = TRUE;

	// 実行
	if ( ! (EXECUTE_NONE <= evt.nExe && evt.nExe <= EXECUTE_MAX) )
	{
		bReturn = FALSE;
	}

	// Sleep
	if ( ! (evt.sleep >= 0) )
	{
		bReturn = FALSE;
	}

	// Event（Mouse/Key）
	if ( ! (EVENT_MOUSE <= evt.nEvent && evt.nEvent <= EVENT_KEY) )
	{
		bReturn = FALSE;
	}

	// Mouse - X
	//if ( ! (evt.evMouse.pt.x >= 0) )
	//{
	//	bReturn = FALSE;
	//}

	// Mouse - Y
	//if ( ! (evt.evMouse.pt.y >= 0) )
	//{
	//	bReturn = FALSE;
	//}

	// MouseCaps
	if ( evt.nEvent == EVENT_MOUSE )
	{
		if ( ! ( 0 <= evt.evMouse.nOpe && evt.evMouse.nOpe <= MOUSE_ARRAY_MAX) )
		{
			bReturn = FALSE;
		}
	}

	// KeyCaps
	if ( evt.nEvent == EVENT_KEY )
	{
		if ( ! ( evt.evKey.dwCap >= 0) )
		{
			bReturn = FALSE;
		}
	}

	if ( ! (strcmp(evt.evKey.key, STR_NULL) != 0) )
	{
		bReturn = FALSE;
	}

	return bReturn;
}

void CMacroToolDlg::OnHelp()
{
#if 0
	MessageBox("ヘルプはありませんm(_ _)m");
#endif

#if 0
	CMyEvent evt;

	evt.MouseRButtonDown();
	SetCapture();
#endif

#if 0
	CMyEvent evt;
	CPoint pt;

	pt.x = 200;
	pt.y = 200;
	evt.MouseMove(pt);
	evt.MouseLButtonClick();
//	evt.FunctionKeyAction(VK_LEFT, FALSE);
	evt.KeyAction(VK_LEFT, FALSE);
#endif

#if 1
	CMyEvent evt;
	for ( int i=0; i<10; i++)
	{
		evt.FunctionKeyAction(VK_NUMLOCK, TRUE);
		Sleep(100);
		evt.FunctionKeyAction(VK_NUMLOCK, FALSE);
		Sleep(100);
		evt.FunctionKeyAction(VK_CAPITAL, TRUE);
		Sleep(100);
		evt.FunctionKeyAction(VK_CAPITAL, FALSE);
		Sleep(100);
		evt.FunctionKeyAction(VK_SCROLL, TRUE);
		Sleep(100);
		evt.FunctionKeyAction(VK_SCROLL, FALSE);
		Sleep(100);
	}
#endif
}

void CMacroToolDlg::UpdateControl()
{
	//Execute
	UpdateControlExecute();

	//Event
	UpdateControlEvent();

	//Detail
	UpdateControlDetail();

	//sleep
	UpdateControlSleep();

	//タイトルバーにスリープ時間を表示
	SetTitleBar();
}

void CMacroToolDlg::UpdateControlExecute()
{
	CString tmp,tmp2;

	tmp.Format("%d",m_tmpEvent[m_idx].nExe);
	GetDlgItem(IDET_EXECUTE)->GetWindowText(tmp2);
	if ( atoi(tmp) != atoi(tmp2) )
	{
		GetDlgItem(IDET_EXECUTE)->SetWindowText(tmp);
	}
}

void CMacroToolDlg::UpdateControlEvent()
{
	int nID = DEF_EVENT;

	if ( m_tmpEvent[m_idx].nEvent != EVENT_UNDEFINE )
	{
		if ( m_tmpEvent[m_idx].nEvent == EVENT_MOUSE )
		{
			nID = IDRB_MOUSE;
		}
		else if ( m_tmpEvent[m_idx].nEvent == EVENT_KEY )
		{
			nID = IDRB_KEY;
		}
	}

	if ( GetCheckedRadioButton(DEF_EVENT_START, DEF_EVENT_END) != nID )
	{
		CheckRadioButton(DEF_EVENT_START, DEF_EVENT_END, nID);
	}
}

void CMacroToolDlg::UpdateControlDetail()
{
	CString tmp,tmp2;
	UINT uCheck;

	{ //EditBox
		//Comment
		GetDlgItem(IDET_COMMENT)->GetWindowText(tmp2);
		if ( tmp2.Compare(m_tmpEvent[m_idx].comment) != 0 )
		{
			GetDlgItem(IDET_COMMENT)->SetWindowText(m_tmpEvent[m_idx].comment);
		}

		//Setting MouseX
		tmp.Format("%d",m_tmpEvent[m_idx].evMouse.pt.x);
		GetDlgItem(IDET_MOUSE_X)->GetWindowText(tmp2);
		if ( atoi(tmp) != atoi(tmp2) )
		{
			GetDlgItem(IDET_MOUSE_X)->SetWindowText(tmp);
		}

		//Setting MouseY
		tmp.Format("%d",m_tmpEvent[m_idx].evMouse.pt.y);
		GetDlgItem(IDET_MOUSE_Y)->GetWindowText(tmp2);
		if ( atoi(tmp) != atoi(tmp2) )
		{
			GetDlgItem(IDET_MOUSE_Y)->SetWindowText(tmp);
		}

		//Setting Key
		GetDlgItem(IDET_KEY)->GetWindowText(tmp2);
		if ( tmp2.Compare(m_tmpEvent[m_idx].evKey.key) != 0 )
		{
			GetDlgItem(IDET_KEY)->SetWindowText(m_tmpEvent[m_idx].evKey.key);
		}
	}

	//{ //RadioButton
	//	int nID;
	//	nID = DEF_MOUSE_CAP;
	//	if ( m_tmpEvent[m_idx].evMouse.nOpe != EVENT_UNDEFINE )
	//	{
	//		nID = m_tmpEvent[m_idx].evMouse.nOpe;
	//	}
	//	if ( GetCheckedRadioButton(DEF_MOUSE_START, DEF_MOUSE_END) != nID )
	//	{
	//		CheckRadioButton(DEF_MOUSE_START, DEF_MOUSE_END, nID);
	//	}
	//}

	{ //CheckButton
		uCheck = BST_UNCHECKED;
		if ( m_tmpEvent[m_idx].evKey.dwCap & BF_CAPS_KEY_SHIFT )
		{
			uCheck = BST_CHECKED;
		}
		if ( IsDlgButtonChecked(IDCH_KEY_SHIFT) != uCheck )
		{
			CheckDlgButton(IDCH_KEY_SHIFT, uCheck);
		}

		uCheck = BST_UNCHECKED;
		if ( m_tmpEvent[m_idx].evKey.dwCap & BF_CAPS_KEY_CTRL )
		{
			uCheck = BST_CHECKED;
		}
		if ( IsDlgButtonChecked(IDCH_KEY_CTRL) != uCheck )
		{
			CheckDlgButton(IDCH_KEY_CTRL, uCheck);
		}

		uCheck = BST_UNCHECKED;
		if ( m_tmpEvent[m_idx].evKey.dwCap & BF_CAPS_KEY_ALT )
		{
			uCheck = BST_CHECKED;
		}
		if ( IsDlgButtonChecked(IDCH_KEY_ALT) != uCheck )
		{
			CheckDlgButton(IDCH_KEY_ALT, uCheck);
		}
	}

	{ //ComboBox
		m_MouseArray.SetCurSel(m_tmpEvent[m_idx].evMouse.nOpe);
		m_KeyArray.SetCurSel(m_tmpEvent[m_idx].evKey.keyEx);
	}
}

void CMacroToolDlg::UpdateControlSleep()
{
	CString tmp,tmp2;

	tmp.Format("%d",m_tmpEvent[m_idx].sleep);
	GetDlgItem(IDET_SLEEP)->GetWindowText(tmp2);
	if ( atoi(tmp) != atoi(tmp2) )
	{
		GetDlgItem(IDET_SLEEP)->SetWindowText(tmp);
	}
}

void CMacroToolDlg::UpdateListControl(BOOL bAll)
{
	if ( bAll )
	{
		for ( int i=0; i < MAXNUM_CLOUMN; i++)
		{
			UpdateListControlExecute(i);
			UpdateListControlSleep(i);
			UpdateListControlEvent(i);
			UpdateListControlDetail(i);
			UpdateListControlComment(i);
		}
	}
	else
	{
		UpdateListControlExecute();
		UpdateListControlSleep();
		UpdateListControlEvent();
		UpdateListControlDetail();
		UpdateListControlComment();
	}
}

void CMacroToolDlg::UpdateListControlExecute(int idx)
{
	CString str = "";
	int nIdx = idx;

	if ( nIdx == CURRENT_IDX )
	{
		nIdx = m_idx;
	}

	if (  m_tmpEvent[nIdx].nEvent != EVENT_UNDEFINE )
	{
		//実行可否
		str.Format("%d", m_tmpEvent[nIdx].nExe);
	}
	m_listctl.SetItemText(nIdx, SUBITEM_EXECUTE, str);
}

void CMacroToolDlg::UpdateListControlSleep(int idx)
{
	CString str = "";
	int nIdx = idx;

	if ( nIdx == CURRENT_IDX )
	{
		nIdx = m_idx;
	}

	if (  m_tmpEvent[nIdx].nEvent != EVENT_UNDEFINE )
	{
		//sleep
		str.Format("%d", m_tmpEvent[nIdx].sleep);
	}
	m_listctl.SetItemText(nIdx, SUBITEM_SLEEP, str);
}

void CMacroToolDlg::UpdateListControlEvent(int idx)
{
	CString str = "";
	int nIdx = idx;

	if ( nIdx == CURRENT_IDX )
	{
		nIdx = m_idx;
	}

	if (  m_tmpEvent[nIdx].nEvent != EVENT_UNDEFINE )
	{
		//event文字列
		switch(m_tmpEvent[nIdx].nEvent)
		{
		case EVENT_KEY :
			str.SetString(STR_KEY);
			break;

		case EVENT_MOUSE :
			str.SetString(STR_MOUSE);
			break;

		default :
			break;
		}
	}
	m_listctl.SetItemText(nIdx, SUBITEM_EVENT, str);
}

void CMacroToolDlg::UpdateListControlDetail(int idx)
{
	CString str = "";
	int nIdx = idx;

	if ( nIdx == CURRENT_IDX )
	{
		nIdx = m_idx;
	}

	if (  m_tmpEvent[nIdx].nEvent != EVENT_UNDEFINE )
	{
		str = GetControlDetail(m_tmpEvent[nIdx]);
	}
	m_listctl.SetItemText(nIdx, SUBITEM_DETAIL, str);
}

void CMacroToolDlg::UpdateListControlComment(int idx)
{
	CString str = "";
	int nIdx = idx;

	if ( nIdx == CURRENT_IDX )
	{
		nIdx = m_idx;
	}

	if (  m_tmpEvent[nIdx].nEvent != EVENT_UNDEFINE )
	{
		str.SetString(m_tmpEvent[nIdx].comment);
	}
	m_listctl.SetItemText(nIdx, SUBITEM_COMMENT, str);
}

void CMacroToolDlg::GetSettings(EVENT *pevt, size_t sz)
{
	if ( pevt != NULL && sz > 0 )
	{
		size_t szTotal = 0;
		size_t szStr = sizeof(m_tmpEvent[0]);

		memset(pevt, 0, sz);
		for ( int i=0; i<MAXNUM_CLOUMN; i++ )
		{
			if ( m_tmpEvent[i].nEvent == EVENT_UNDEFINE )
			{
				continue ;
			}

			if ( (szTotal + szStr) > sz )
			{
				break;
			}

			memcpy(pevt++, &m_tmpEvent[i], szStr);
			szTotal+=szStr;
		}
	}
}

int CMacroToolDlg::GetTotalTime()
{
	int nMsec = m_p->m_nRepeatDelayMsec;
	for (int i=0; i < MAXNUM_CLOUMN; i++)
	{
		if ( m_tmpEvent[i].nEvent != EVENT_UNDEFINE )
		{
			nMsec += m_tmpEvent[i].sleep * m_tmpEvent[i].nExe;
		}
	}

	return nMsec;
}

void CMacroToolDlg::SetTitleBar()
{
	int nMsec = GetTotalTime();

	TIME tm;
	MakeTime(nMsec, &tm);

	CString sTitle, sTmp;
	sTitle.SetString(STR_SET_TITLE);
	sTmp.Format(STR_SLEEP_TITLE,
		tm.nHour,
		tm.nMin,
		tm.nSec,
		tm.nMsec);
	sTitle += sTmp;
	SetWindowText(sTitle);
}


BOOL CMacroToolDlg::MakeTime(int nMsec, TIME *pTm)
{
	int tmp	= nMsec;

	if ( pTm == NULL )
	{
		return FALSE;
	}

	pTm->nMsec = tmp % MSEC_MAX;	tmp /= MSEC_MAX;
	pTm->nSec = tmp % SEC_MAX; 		tmp	/= SEC_MAX;
	pTm->nMin = tmp % MIN_MAX;		tmp /= MIN_MAX;
	pTm->nHour = tmp % HOUR_MAX;	tmp /= HOUR_MAX;

	return TRUE;
}


void CMacroToolDlg::OnRecord()
{
	char TempDir[MAX_PATH];
	char TempFile[MAX_PATH];

	GetTempPath( sizeof(TempDir), TempDir);
	sprintf(TempFile, "%s\\%s",TempDir, DLL_LOG_FNAME);

	if ( ! m_Record )
	{
		CString title;

		title.Format("%s(Record中)",STR_SET_TITLE);
		SetWindowText(title);
		GetDlgItem(IDBT_RECORD)->SetWindowText("記録終了");

		remove(TempFile);	// ファイルが存在していたら削除
		StartRecord();
	}
	else
	{
		SetTitleBar();
		GetDlgItem(IDBT_RECORD)->SetWindowText("記録");
		StopRecord();

		ReadFile(TempFile);
	}
}

void CMacroToolDlg::StartRecord()
{
#if USE_KEY_HOOK // キーボードは未完成
	if ( StartKeyHook && StartKeyHook() &&
		 StartMouseHook && StartMouseHook() )
#else
	if ( StartMouseHook && StartMouseHook() )
#endif
	{
		m_Record = TRUE;
	}
	else
	{
		MessageBox("StartMouseHook() fail");
	}
}


void CMacroToolDlg::StopRecord()
{
#if USE_KEY_HOOK // キーボードは未完成
	if ( StopKeyHook && StopKeyHook()
		 StopMouseHook && StopMouseHook() )
#else
	if ( StopMouseHook && StopMouseHook() )
#endif
	{
		m_Record = FALSE;
	}
	else
	{
		MessageBox("StopMouseHook() fail");
	}
}

CString CMacroToolDlg::GetSettingFileName()
{
	return m_fName;
}

void CMacroToolDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_IsDebug = ! m_IsDebug;

	CString str;
	str.Format("DebugMode=%d", m_IsDebug);
	MessageBox(str);

	if ( DebugMode )
	{
		DebugMode(m_IsDebug);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CMacroToolDlg::IsSuccessSetting()
{
	if  ( m_p->m_nRepeatNum <= 1 )
	{
		/* 実行回数が1回以下は警告対象外 */
		return TRUE;
	}

	int nMsec = GetTotalTime();
	if ( nMsec > CHECK_CRISIS_MSEC )
	{
		/* 1回の実行時間が規定時間未満は警告対象外 */
		return TRUE;
	}

	CString str;
	str.Format("下記が設定されました。設定を見直しますか？\n　・2回以上の実行回数\n　・１回の実行時間が2秒未満");
	int ret = MessageBox(str, "Warning", MB_YESNO);
	if ( ret == IDNO )
	{
		/* ユーザーが問題無いと判断 */
		return TRUE;
	}

	return FALSE;
}

void CMacroToolDlg::OnBnClickedOk()
{
	if( ! IsSuccessSetting() )
	{
		// 設定し直し
		return;
	}
		
	CDialog::OnOK();
}
