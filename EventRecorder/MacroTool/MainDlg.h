#pragma once

#include "resource.h"		// メイン シンボル

#define DEF_REPEATNUM		1
#define DEF_REPEATTIME		100
#define DEF_SLEEP_VALUE		0
#define DEF_MOUSE_X_VALUE	0
#define DEF_MOUSE_Y_VALUE	0

#define MAXNUM_CLOUMN		700
#define START_WAIT_TIME		1000
#define STR_MAIN_TITLE		"ﾏｸﾛ"
#define STR_SET_TITLE		"設定"
#define STR_SLEEP_TITLE		"　（総計： %02dh %02dm %02ds %03dmsec）"
#define STR_RUNNING_PROC		"処理中です"
#define STR_START_PROC		"[開始]"
#define STR_STOP_PROC		"[停止]"

#define BF_CAPS_KEY_SHIFT	0x0001
#define BF_CAPS_KEY_CTRL	0x0002
#define BF_CAPS_KEY_ALT		0x0004

#define CLOSE_WAIT_TIME		5	// 閉じる前に最大5秒まで待つ

//#define FLICKA_KEY	VK_CAPITAL
#define FLICKA_KEY	VK_NONE


typedef struct tagEVENTKEY
{
	DWORD	dwCap;
	int		keyEx;
	char	key[MAX_PATH];
} EVENTKEY;

typedef struct tagEVENTMOUSE
{
	CPoint	pt;
	int		nOpe;
} EVENTMOUSE;

typedef struct tagEVENT
{
	int			nExe;
	DWORD		sleep;
	int			nEvent;
	EVENTKEY	evKey;
	EVENTMOUSE	evMouse;
	char		comment[MAX_PATH];
} EVENT;


CString GetControlDetail(EVENT evt);

// CMainDlg ダイアログ
class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMainDlg();

	char		m_strTitle[MAX_PATH];
	CString		m_fName;
	UINT		m_nRepeatNum;
	UINT		m_nRepeatDelayMsec;
	BOOL		m_bProcFlg;
	HANDLE		m_hThread;
	EVENT		m_strEvent[MAXNUM_CLOUMN];
	CString		m_ver;


// ダイアログ データ
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetting();
	afx_msg void OnExec();
	afx_msg LRESULT OnEndProc(UINT wParam, LONG lParam);
	afx_msg void OnStop();
	afx_msg void OnClose();
};

