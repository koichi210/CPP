#define EXPORTS extern "C" __declspec(dllexport)

// Utility(commonÇ¡ÇƒñºëOÇ…Ç∑ÇÈÅH)ÇÃã§í ÉwÉbÉ_Ç…à⁄ìÆ +
#ifndef MAX_PATH
#define MAX_PATH	(256)
#endif

#define BKEY_PUSH	(0x80)		//Byteå^
#define SKEY_PUSH	(0x8000)	//Shortå^

BOOL IsShiftKeyDown();
BOOL IsControlKeyDown();
BOOL IsKeyDown(int nKey);
BOOL IsKeyDown(short sKey);
BOOL IsKeyDown(BYTE bKey);
void MyTolower(int *nKey);

// UtilityÇÃã§í ÉwÉbÉ_Ç…à⁄ìÆ -


#define DLL_LOG_FNAME	"MacroLog.txt"
#define STR_LOG_FMT		"%3d,%8d,%2d,%4d,%4d,%2d,%2d,%2d,%s,%s\n"

#define MAX_HOUR	24
#define MAX_MINUTES	60
#define MAX_SECONDS	60
#define MAX_MSEONDS	1000

#define DEF_EXE_NUM	1
#define EVENT_MOUSE	1
#define EVENT_KEY	2


//m_tmpEvent[i].nExe
//m_tmpEvent[i].sleep
//m_tmpEvent[i].nEvent
//m_tmpEvent[i].evMouse.pt.x
//m_tmpEvent[i].evMouse.pt.y
//m_tmpEvent[i].evMouse.nOpe
//m_tmpEvent[i].evKey.dwCap
//m_tmpEvent[i].evKey.keyEx
//m_tmpEvent[i].evKey.key
//m_tmpEvent[i].comment);

//m_tmpEvent[i].evKey.dwCap,
#define BF_CAPS_KEY_SHIFT	0x0001
#define BF_CAPS_KEY_CTRL	0x0002
#define BF_CAPS_KEY_ALT		0x0004

//m_tmpEvent[i].evMouse.nCap,
#define MOUSE_OPERATION_LCLICK	(0)
#define MOUSE_OPERATION_LDOWN	(1)
#define MOUSE_OPERATION_LUP		(2)
#define MOUSE_OPERATION_RCLICK	(3)
#define MOUSE_OPERATION_RDOWN	(4)
#define MOUSE_OPERATION_RUP		(5)
#define MOUSE_OPERATION_MOVE	(6)

EXPORTS BOOL StartKeyHook();
EXPORTS BOOL StartMouseHook();
EXPORTS BOOL StopKeyHook();
EXPORTS BOOL StopMouseHook();

LRESULT CALLBACK KeyHookProc(int, WPARAM, LPARAM);
LRESULT CALLBACK MouseHookProc(int, WPARAM, LPARAM);
BOOL GetKeyParameter(WPARAM wParam, DWORD *dwCap, TCHAR *key);
BOOL GetMouseParameter(WPARAM wParam, int *nCap);
void WriteLog(char *stirng);
DWORD ElapsedTime();

