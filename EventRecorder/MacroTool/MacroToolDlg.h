// MacroToolDlg.h : ヘッダー ファイル
#pragma once

#include "StandardTemplate.h"
#include "afxcmn.h"
#include "afxwin.h"


#define USE_KEY_HOOK	0
#define DLL_LOG_FNAME	"MacroLog.txt"

// Ini項目名
#define INI_FILE_SECTION_TABLE "TABLE"
#define INI_FILE_SECTION_COMMON "COMMON"

#define INI_FILE_KEY_TABLE_PARAM "Param"
#define INI_FILE_KEY_REPLEAT_NUM "RepeatNum"
#define INI_FILE_KEY_REPEAT_DELAY_MSEC "DelayMsec"

#define DEF_EXECUTE		BST_CHECKED
#define DEF_EVENT		IDRB_MOUSE
#define DEF_MOUSE_CAP	0 //IDRB_MOUSE_LCLICK

#define DEF_EVENT_START	IDRB_MOUSE
#define DEF_EVENT_END	IDRB_KEY

#define DEF_MOUSE_START	IDRB_MOUSE_POINT_MOVE
#define DEF_MOUSE_END	IDRB_MOUSE_RCLICK

#define EVENT_UNDEFINE	0
#define EVENT_MOUSE		1
#define EVENT_KEY		2

#define CURRENT_IDX		-1
#define NO_PROC			-1
#define NO_ENTRY		-1

#define MAX_MOUSE_POINT_VALUE	9999
#define MAX_SLEEP_VALUE			99999999
#define EXECUTE_MAX		32767
#define EXECUTE_NONE		0

#define SUBITEM_EXECUTE	1
#define SUBITEM_SLEEP	SUBITEM_EXECUTE+1
#define SUBITEM_EVENT	SUBITEM_SLEEP+1
#define SUBITEM_DETAIL	SUBITEM_EVENT+1
#define SUBITEM_COMMENT	SUBITEM_DETAIL+1

#define LABELIDX_NO			0
#define LABELIDX_EXECUTE	LABELIDX_NO+1
#define LABELIDX_SLEEP		LABELIDX_EXECUTE+1
#define LABELIDX_EVENT		LABELIDX_SLEEP+1
#define LABELIDX_DETAIL	LABELIDX_EVENT+1
#define LABELIDX_COMMENT	LABELIDX_DETAIL+1

#define LABEL_WIDTH_NO			25
#define LABEL_WIDTH_EXECUTE		45
#define LABEL_WIDTH_SLEEP		45
#define LABEL_WIDTH_EVENT		40
#define LABEL_WIDTH_DETAIL		140
#define LABEL_WIDTH_COMMENT		30

#define STR_TRUE			"○"
#define STR_FALSE			"×"
#define STR_MOUSE			"マウス"
#define STR_KEY				"キー"
#define STR_MOUSE_MOVE		"移動"
#define STR_MOUSE_LCLICK	"左クリック"
#define STR_MOUSE_RCLICK	"右クリック"
#define STR_MOUSE_POINT		"%s X[%4d] Y[%4d]"
#define STR_KEY_FMT			"[%s]"
#define STR_KEY_SHIFT		"　Shift"
#define STR_KEY_CTRL		"　Ctrl"
#define STR_KEY_ALT			"　Alt"

#define CONT_NUM			10
#define STR_LOG_FMT			"%d,%d,%d,%d,%d,%d,%d,%d,%s,%s\n"
//#define STR_LOG_FMT		"%d,%d,%d,%d,%d,%d,%d,%d,%s %s\n"
#define STR_SAVE_FNAME		"save.txt"
#define STR_DLG_FILTER		"Files (*.txt)|*.txt|All Files (*.*)|*.*|"
#define STR_DLG_TITLE		"ファイルを選択"
#define STR_ERR_FILE_NOT_FOUND	"ファイルがみつかりません m(_ _)m"
#define STR_ERR_OPEN_FILE	"ファイルの読み込みに失敗しました(err=%d)"
#define STR_ERR_WRITE		"ファイルの保存に失敗しました"
#define STR_NULL			"(null)"
#define STR_EXT				".txt"
#define STR_SELECT_LAST_CLOUMN	"最終行を選択しました。処理を中断します"

#define LOGGING_FNAME	"MacroLog.txt"

#define MOUSE_ARRAY_MAX		(7)
#define MOUSE_ARRAY_LCLICK	(0)
#define MOUSE_ARRAY_LDOWN	(1)
#define MOUSE_ARRAY_LUP		(2)
#define MOUSE_ARRAY_RCLICK	(3)
#define MOUSE_ARRAY_RDOWN	(4)
#define MOUSE_ARRAY_RUP		(5)
#define MOUSE_ARRAY_MOVE	(6)
const char arMouse[MOUSE_ARRAY_MAX][MAX_PATH] = {"左クリック", "左DN", "左UP", "右クリック", "右DN", "右UP", "移動"};

#define KEY_ARRAY_USER			0
#define KEY_ARRAY_FUNC_START	(KEY_ARRAY_USER + 1)			//  1
#define KEY_ARRAY_FUNC_END		(KEY_ARRAY_FUNC_START + 11)		// 12
#ifdef TODO //カーソルが動かない
#define KEY_ARRAY_CURSOR_START	(KEY_ARRAY_FUNC_END + 1)		// 13
#define KEY_ARRAY_CURSOR_END	(KEY_ARRAY_CURSOR_START + 3)	// 16
#define KEY_ARRAY_MAX			(KEY_ARRAY_CURSOR_END + 1)		// 17
const char arKey[KEY_ARRAY_MAX][MAX_PATH] = {"右記", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "←", "↑", "→", "↓"};
#else
#define KEY_ARRAY_MAX			(KEY_ARRAY_FUNC_END + 1)		// 13
const char arKey[KEY_ARRAY_MAX][MAX_PATH] = {"上記", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};
#endif

#define HOUR_MAX	24
#define MIN_MAX		60
#define SEC_MAX		60
#define MSEC_MAX	1000

typedef BOOL _stdcall StartKeyHookFUNC();
typedef BOOL _stdcall StartMouseHookFUNC();
typedef BOOL _stdcall StopKeyHookFUNC();
typedef BOOL _stdcall StopMouseHookFUNC();

#ifdef _DEBUG
 #define DLL_NAME	"../Release/EventHookd.dll"
#else
 #define DLL_NAME	"EventHookd.dll"
#endif

typedef struct{
	int nHour;
	int nMin;
	int nSec;
	int nMsec;
} TIME;

// CMacroToolDlg ダイアログ
class CMacroToolDlg : public CDialog
{
// コンストラクタ
public:
	CMacroToolDlg(CWnd* pParent = NULL, CString fName = "");	// 標準コンストラクタ
	virtual ~CMacroToolDlg();									// デストラクタ
	enum { IDD = IDD_MACROTOOL_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// メンバ変数
protected:
	HICON		m_hIcon;
	CMainDlg	*m_p;
	EVENT		m_tmpEvent[MAXNUM_CLOUMN];
	EVENT		m_CopyPaste;
	int			m_idx;
	CCustomCtrl m_listctl;
	CString		m_fName;
	CMyRistrictedSBCS	m_etKey;
	CEdit		m_etComment;
	CComboBox	m_MouseArray;
	CComboBox	m_KeyArray;
	BOOL		m_Record;

	StartKeyHookFUNC	* StartKeyHook;
	StartMouseHookFUNC	* StartMouseHook;
	StopKeyHookFUNC		* StopKeyHook;
	StopMouseHookFUNC	* StopMouseHook;
	HINSTANCE m_hDLLInst;

//既存メンバ関数
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRecord();
	afx_msg void OnLvnItemchangedListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

//メンバ関数
	void StartRecord();
	void StopRecord();

//メンバ関数
public:
	virtual BOOL OnInitDialog();
	void Initialize();
	void OnListInsert();
	void OnListDelete();
	void OnListCopy();
	void OnListPaste();
	void OnEnChangeComment();
	void OnMouse();
	void OnCbnlchangeMouse();
	void OnMousePointMove();
	void OnMouseLclick();
	void OnMouseRclick();
	void OnEnChangeMouseX();
	void OnEnChangeMouseY();
	void OnCbnlchangeKey();
	void OnKey();
	void OnEnChangeKey();
	void OnKeyShift();
	void OnKeyCtrl();
	void OnKeyAlt();
	void OnEnChangeExecute();
	void OnEnChangeSleep();
	void OnEnChangeRepeatNum();
	void OnEnChangeRepeatTime();
	void OnAllclear();
	void RestoreDefault(EVENT *strEvent = NULL);
	void OnRead();
	void ReadFile(CString FileName);
	void OnWrite();
	void SplitLogString(EVENT *pEvt, char *str, int sz);
	void MakeLogString(EVENT *pEvt, char *str, int sz);
	BOOL GetFileName(BOOL bRead = FALSE);
	BOOL CheckLogFormat(EVENT evt);
	void OnHelp();

	// 設定値コントロールのUpdate
	void UpdateControl();
	void UpdateControlExecute();
	void UpdateControlEvent();
	void UpdateControlDetail();
	void UpdateControlSleep();

	// リストコントロールのUpdate
	void UpdateListControl(BOOL bAll = FALSE);
	void UpdateListControlExecute(int idx = CURRENT_IDX);
	void UpdateListControlSleep(int idx = CURRENT_IDX);
	void UpdateListControlEvent(int idx = CURRENT_IDX);
	void UpdateListControlDetail(int idx = CURRENT_IDX);
	void UpdateListControlComment(int idx = CURRENT_IDX);
	void GetSettings(EVENT *pevt, size_t sz);
	void SetTitleBar();
	BOOL MakeTime(int nMsec, TIME *pTm);
	CString GetSettingFileName();

};
