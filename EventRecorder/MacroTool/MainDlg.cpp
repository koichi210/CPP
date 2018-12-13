// MainDlg.cpp : 実装ファイル
//

#include <process.h>
#include "stdafx.h"
#include "MainDlg.h"
#include "MacroTool.h"
#include "MacroToolDlg.h"

#define WM_ENDPROC	WM_USER+100

typedef struct
{
	HWND			hWnd;
	CMacroToolDlg	*pParent;
	BOOL			*bpProcFlg;
	UINT			nRepeat;
	UINT			nRepeatTime;
	char			strTitle[MAX_PATH];
	EVENT			strEvent[MAXNUM_CLOUMN];
} sPROCSETTINGS;

UINT ProcThread(LPVOID pParam);
unsigned __stdcall fnProc(PVOID param);
void SleepCnt(int sleep, BOOL *bpProcFlg);
void fnProcMouse(EVENTMOUSE evMouse);
void fnProcKey(EVENTKEY	evKey);



// CMainDlg ダイアログ
IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_nRepeatNum = DEF_REPEATNUM;
	m_nRepeatDelayMsec = DEF_REPEATTIME;
	m_hThread = NULL;
	m_bProcFlg = FALSE;
	memset(&m_strEvent, 0, sizeof(m_strEvent));
	memset(&m_strTitle, 0, sizeof(m_strTitle));
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDBT_SETTING, OnSetting)
	ON_BN_CLICKED(IDBT_EXEC, OnExec)
    ON_MESSAGE(WM_ENDPROC, OnEndProc)
	ON_BN_CLICKED(IDBT_STOP, OnStop)
	ON_BN_CLICKED(IDCLOSE, OnClose)
END_MESSAGE_MAP()


// CMainDlg メッセージ ハンドラ
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初期表示
	m_ver.LoadString(IDS_VERSION);
	SetWindowText(m_ver);

	return TRUE;
}


void CMainDlg::OnSetting()
{
	CString str;
	GetDlgItem(IDET_SAMPLE)->GetWindowText(str);

	//カレントディレクトリを設定
#if USER_DIRECTORY
	char path[MAX_PATH];
	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		m_fName.SetString(path);
	}
#else
	m_fName.SetString(".");
#endif

	//カレントファイル名を設定
	m_fName.Append("\\");
	if ( str.IsEmpty() )
	{
		m_fName.Append(STR_SAVE_FNAME);
	}
	else
	{
		m_fName.Append(str);
	}

	CMacroToolDlg gmpDlg(this, m_fName);
	INT_PTR nResponse = gmpDlg.DoModal();
	if ( nResponse == IDOK )
	{
		gmpDlg.GetSettings(m_strEvent, sizeof(m_strEvent));

		char fname[MAX_PATH];
		_splitpath_s(gmpDlg.GetSettingFileName(),
			NULL, 0,
			NULL, 0,
			fname, sizeof(fname),
			NULL, 0);
		strcpy(m_strTitle, (LPCTSTR)fname);
		GetDlgItem(IDET_SAMPLE)->SetWindowText(m_strTitle);
	}
}

void CMainDlg::OnExec()
{
	GetDlgItem(IDBT_EXEC)->EnableWindow(FALSE);
	GetDlgItem(IDBT_SETTING)->EnableWindow(FALSE);

	sPROCSETTINGS	*pProcSettings;
//	pProcSettings = (sPROCSETTINGS *) new sPROCSETTINGS;

	sPROCSETTINGS	tmp;
	pProcSettings = (sPROCSETTINGS *) &tmp;

	CString sTitle;
	sTitle.Format("%s %s", m_ver, STR_START_PROC);
	SetWindowText(sTitle);

	Sleep(START_WAIT_TIME);

	if ( pProcSettings )
	{
		pProcSettings->hWnd = m_hWnd;
		pProcSettings->pParent = (CMacroToolDlg *)GetParent();
		pProcSettings->bpProcFlg = &m_bProcFlg;
		pProcSettings->nRepeat = m_nRepeatNum;
		pProcSettings->nRepeatTime = m_nRepeatDelayMsec;
		strcpy(pProcSettings->strTitle, m_strTitle);

		memcpy(pProcSettings->strEvent, m_strEvent, sizeof(pProcSettings->strEvent));

		m_bProcFlg = TRUE;
#ifdef BEGINTHREADEX
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, fnProc, pProcSettings, 0, NULL);
#else
		AfxBeginThread(ProcThread, pProcSettings);
#endif
	}
}

LRESULT CMainDlg::OnEndProc(UINT wParam, LONG lParam)
{
	if ( m_hThread != NULL )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	OnStop();
	ShowWindow(SW_RESTORE);

	return TRUE;
}

void CMainDlg::OnStop()
{
	if ( m_bProcFlg == TRUE )
	{
		SetWindowText(m_ver);
		m_bProcFlg = FALSE;
	}

	GetDlgItem(IDBT_EXEC)->EnableWindow(TRUE);
	GetDlgItem(IDBT_SETTING)->EnableWindow(TRUE);
}

void CMainDlg::OnClose()
{
	if ( m_bProcFlg == TRUE )
	{
		//CString sTitle;
		//sTitle.Format("%s %s", m_strTitle, STR_STOP_PROC);
		//SetWindowText(sTitle);

		m_bProcFlg = FALSE;
		for( int i=0; i<CLOSE_WAIT_TIME;i++ )
		{
			if ( m_hThread == NULL )
			{
				break;
			}
			Sleep(1);
		}
	}
	OnOK();
}

UINT ProcThread(LPVOID pParam)
{
	sPROCSETTINGS* p=(sPROCSETTINGS*)pParam;
	CString			tmp;
#ifdef TODO	//メモリリーク；；
	CString			str;
#else
	char			str[MAX_PATH];
#endif

	if ( p == NULL )
	{
		return FALSE;
	}

	for (UINT i=0; i < p->nRepeat && *p->bpProcFlg; i++)
	{
		for (int j=0; j < MAXNUM_CLOUMN && *p->bpProcFlg; j++)
		{
			// これ以上定義がないので終了
			if ( p->strEvent[j].nEvent == EVENT_UNDEFINE )
			{
				break;
			}

			for (int k=0; k < p->strEvent[j].nExe; k++)
			{
#ifdef TODO	//メモリリーク；；
				str.Format(
#else
				sprintf(str,
#endif
					"[(%d:%d:%d)]",
//					p->m_ver,
					i+1,
//					p->nRepeat,
					j+1,
					k+1);
//					p->strEvent[j].nExe);
				SetWindowText(p->hWnd, str);

				// sleep後、動作する前にフラグをチェック
				SleepCnt(p->strEvent[j].sleep, p->bpProcFlg);

				if ( *p->bpProcFlg == FALSE )
				{
					break;
				}

				if ( p->strEvent[j].nEvent == EVENT_MOUSE )
				{
					fnProcMouse(p->strEvent[j].evMouse);
				}
				else // if ( p->strEvent[j].nEvent == EVENT_KEY )
				{
					fnProcKey(p->strEvent[j].evKey);
				}
			}
		}
		SleepCnt(p->nRepeatTime, p->bpProcFlg);	// 全体を対象としたsleep時間
	}
	PostMessage(p->hWnd, WM_ENDPROC, NULL, NULL);

	return TRUE;
}

unsigned __stdcall fnProc(PVOID param)
{
	sPROCSETTINGS	*p = (sPROCSETTINGS *)param;

	PostMessage(p->hWnd, WM_ENDPROC, NULL, NULL);
	_endthreadex(0);

	return TRUE;
}

void SleepCnt(int sleep, BOOL *bpProcFlg)
{
	int sec = sleep / 1000;
	int msec = sleep % 1000;
	CMyEvent evt;

	int cnt = 0;

	// 1秒未満のsleepをここで補填
	Sleep(msec);

	//1秒ずつsleepして、bpProcfFlgを見る
	for(int i=0; i<sec && *bpProcFlg; i++)
	{
		evt.FunctionKeyAction(FLICKA_KEY);
		cnt++;
		Sleep(1000);
	}

	if ( cnt % 2 )
	{
		evt.FunctionKeyAction(FLICKA_KEY);
	}
}

void fnProcMouse(EVENTMOUSE evMouse)
{
	CMyEvent evt;

	evt.MouseMove(evMouse.pt);
	switch ( evMouse.nOpe )
	{
	default : // no break
	case MOUSE_ARRAY_LCLICK :	evt.MouseLButtonClick();break;
	case MOUSE_ARRAY_LDOWN :	evt.MouseLButtonDown();	break;
	case MOUSE_ARRAY_LUP :		evt.MouseLButtonUp();	break;
	case MOUSE_ARRAY_RCLICK :	evt.MouseRButtonClick();break;
	case MOUSE_ARRAY_RDOWN :	evt.MouseRButtonDown();	break;
	case MOUSE_ARRAY_RUP :		evt.MouseRButtonUp();	break;
	case MOUSE_ARRAY_MOVE :		/*evt.MouseMove();*/	break;
	}
}

void fnProcKey(EVENTKEY	evKey)
{
	#define VK_FUNCTIONKEY(k)	(VK_F1   + k - KEY_ARRAY_FUNC_START)
	#define VK_CURSOL(k)		(VK_LEFT + k - KEY_ARRAY_CURSOR_START+1)

	DWORD dwCap = evKey.dwCap;
	CMyEvent evt;

	if ( dwCap & BF_CAPS_KEY_SHIFT )
	{
		evt.KeyAction(VK_SHIFT, TRUE);
	}
	if ( dwCap & BF_CAPS_KEY_CTRL )
	{
		evt.KeyAction(VK_CONTROL, TRUE);
	}
	if ( dwCap & BF_CAPS_KEY_ALT )
	{
		evt.KeyAction(VK_MENU, TRUE);
	}

	if ( KEY_ARRAY_FUNC_START <= evKey.keyEx && evKey.keyEx <= KEY_ARRAY_FUNC_END )
	{
		// function
		evt.FunctionKeyAction(VK_FUNCTIONKEY(evKey.keyEx));
	}
#ifdef TODO //カーソルが動かない
	else if ( KEY_ARRAY_CURSOR_START <= evKey.keyEx && evKey.keyEx <= KEY_ARRAY_CURSOR_END )
	{
		// cursol
		evt.FunctionKeyAction(VK_CURSOL(evKey.keyEx));
	}
#endif
	else // if ( evKey.keyEx == KEY_ARRAY_USER )
	{
		// キー入力(a to z)
		char *key = evKey.key;
		size_t len = strlen(evKey.key);
		for(size_t i=0; i<len; i++)
		{
			evt.KeyAction(key[i]);
		}
	}

	if ( dwCap & BF_CAPS_KEY_SHIFT )
	{
		evt.KeyAction(VK_SHIFT, FALSE);
	}
	if ( dwCap & BF_CAPS_KEY_CTRL )
	{
		evt.KeyAction(VK_CONTROL, FALSE);
	}
	if ( dwCap & BF_CAPS_KEY_ALT )
	{
		evt.KeyAction(VK_MENU, FALSE);
	}
}

CString GetControlDetail(EVENT evt)
{
	CString str;

	//設定文字列
	if ( evt.nEvent == EVENT_KEY )
	{
		if ( evt.evKey.keyEx == KEY_ARRAY_USER )
		{
			if ( strcmp(evt.evKey.key, "") != 0 )
			{
				str.Format(STR_KEY_FMT, evt.evKey.key);
			}
		}
		else
		{
			str.Format(arKey[evt.evKey.keyEx]);
		}

		if ( evt.evKey.dwCap & BF_CAPS_KEY_SHIFT )
		{
			str += STR_KEY_SHIFT;
		}

		if ( evt.evKey.dwCap & BF_CAPS_KEY_CTRL )
		{
			str += STR_KEY_CTRL;
		}

		if ( evt.evKey.dwCap & BF_CAPS_KEY_ALT )
		{
			str += STR_KEY_ALT;
		}
	}
	else if ( evt.nEvent == EVENT_MOUSE )
	{
		str.Format(STR_MOUSE_POINT,arMouse[evt.evMouse.nOpe], evt.evMouse.pt.x, evt.evMouse.pt.y);

		//switch (evt.evMouse.nOpe)
		//{
		//case IDRB_MOUSE_POINT_MOVE :
		//default :
		//	str.Format(STR_MOUSE_MOVE);
		//	break ;
		//case IDRB_MOUSE_LCLICK :
		//	str.SetString(STR_MOUSE_LCLICK);
		//	break ;
		//case IDRB_MOUSE_RCLICK :
		//	str.SetString(STR_MOUSE_RCLICK);
		//	break ;
		//}
		//CString tmp;
		//tmp.Format(STR_MOUSE_POINT, evt.evMouse.pt.x, evt.evMouse.pt.y);
		//str += tmp;
	}
	else
	{
		str.SetString("");
	}

	return str;
}
