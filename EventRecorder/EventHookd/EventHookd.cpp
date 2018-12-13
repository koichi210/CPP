// EventHookd.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include <stdio.h>
#include "stdafx.h"
#include "EventHookd.h"

// ���L������
#pragma data_seg(".shared")
HHOOK		ghKeyHook = NULL;
HHOOK		ghMouseHook = NULL;
SYSTEMTIME	gOld_tm = {0,0,0,0,0,0,0,0};
#pragma data_seg()
HINSTANCE	ghInst = NULL;

BOOL APIENTRY DllMain( HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			ghInst = (HINSTANCE)hinstDLL;
			break;
		default :
			break;
	}
	return TRUE;
}

EXPORTS BOOL StartKeyHook()
{
	ghKeyHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyHookProc, ghInst, 0);
	if ( ! ghKeyHook )
	{
		return FALSE;
	}
	GetSystemTime(&gOld_tm);
	return TRUE;
}

EXPORTS BOOL StartMouseHook()
{
	ghMouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseHookProc, ghInst, 0);
	if ( ! ghMouseHook )
	{
		return FALSE;
	}
	GetSystemTime(&gOld_tm);
	return TRUE;
}

EXPORTS BOOL StopKeyHook()
{
	if ( ! UnhookWindowsHookEx(ghKeyHook) )
	{
		return FALSE;
	}
	return TRUE;
}

EXPORTS BOOL StopMouseHook()
{
	if ( ! UnhookWindowsHookEx(ghMouseHook) )
	{
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ( nCode == HC_ACTION )
	{
		DWORD dwCap = 0;
		TCHAR key[2];

		memset(&key, 0 , sizeof(key));
		if ( GetKeyParameter(wParam, &dwCap, key) )
		{
			TCHAR str[MAX_PATH];

			memset(&str, 0, sizeof(str));
			sprintf(str, STR_LOG_FMT,
				DEF_EXE_NUM,	//m_tmpEvent[i].nExe
				ElapsedTime(),	//m_tmpEvent[i].sleep
				EVENT_KEY,		//m_tmpEvent[i].nEvent
				0,				//m_tmpEvent[i].evMouse.pt.x
				0,				//m_tmpEvent[i].evMouse.pt.y
				0,				//m_tmpEvent[i].evMouse.nOpe
				dwCap,			//m_tmpEvent[i].evKey.dwCap
				0,				//m_tmpEvent[i].evKey.keyEx
				key,			//m_tmpEvent[i].evKey.key
				""				//m_tmpEvent[i].evKey.comment
				);
			WriteLog(str);
		}
	}
	return CallNextHookEx(ghKeyHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ( nCode == HC_ACTION )
	{
		int nCap = 0;

		// Mouse Move���̓��M���O���Ȃ��B
		if ( GetMouseParameter(wParam, &nCap) )
		{
			POINT pt;
			TCHAR str[MAX_PATH];

			GetCursorPos(&pt);
			sprintf(str,STR_LOG_FMT,
						DEF_EXE_NUM,	//m_tmpEvent[i].nExe
						ElapsedTime(),	//m_tmpEvent[i].sleep
						EVENT_MOUSE,	//m_tmpEvent[i].nEvent
						pt.x,			//m_tmpEvent[i].evMouse.pt.x
						pt.y,			//m_tmpEvent[i].evMouse.pt.y
						nCap,			//m_tmpEvent[i].evMouse.nOpe
						0,				//m_tmpEvent[i].evKey.dwCap
						0,				//m_tmpEvent[i].evKey.keyEx
						"",				//m_tmpEvent[i].evKey.key
						""				//m_tmpEvent[i].evKey.comment
						);
			WriteLog(str);
		}
	}
	return CallNextHookEx(ghKeyHook, nCode, wParam, lParam);
}

BOOL GetKeyParameter(WPARAM wParam, DWORD *dwCap, TCHAR *key)
{
	int nKey = (int)wParam;
	BOOL bProc = TRUE;

	if ( ! dwCap || ! key )
	{
		bProc = FALSE;
	}

	// ���l or �A���t�@�x�b�g�łȂ���Ή������Ȃ�
	// Function�L�[���̏����ɑΉ����Ă��Ȃ�����
	if ( bProc )
	{
		if ( ! isalpha(nKey) && ! isdigit(nKey) )
		{
			bProc = FALSE;
		}
	}

	// �Ώۂ̃L�[��������Ă��邩����
	if ( bProc )
	{
		// ����̃L�[�𔻕ʂ��邾���Ȃ� GetAsyncKeyState(vk) �ł��ǂ�
		//BYTE bKey[MAX_PATH];
		//if ( GetKeyboardState(bKey) ) 
		//{
		//	bProc = IsKeyDown(bKey[nKey]);
		//}

		// wParam�����ł́AKEY_UP��KEY_DOWN�����̃C�x���g�����ł����Ⴄ�̂ŁA������Ă邩�ǂ����������ƃ`�F�b�N
		short sKey = GetAsyncKeyState(nKey);
		bProc = IsKeyDown(sKey);
	}

	// ����
	if ( bProc )
	{
		if ( IsControlKeyDown() )
		{
			*dwCap |= BF_CAPS_KEY_CTRL;
		}

		if ( IsShiftKeyDown() )
		{
			*dwCap |= BF_CAPS_KEY_SHIFT;
		}
		else
		{
			// ShiftKey��������Ă��Ȃ�������A�啶�����������ɕϊ�
			MyTolower(&nKey);
		}
		*key = (TCHAR)nKey;
	}

	return bProc;
}

BOOL GetMouseParameter(WPARAM wParam, int *nCap)
{
	BOOL bProc = TRUE;

	if ( ! nCap )
	{
		bProc = FALSE;
	}

	// wParam = �C�x���g��
	switch(wParam)
	{
		case WM_LBUTTONDOWN :		*nCap = MOUSE_OPERATION_LDOWN;	break;
		case WM_LBUTTONUP :			*nCap = MOUSE_OPERATION_LUP;	break;
		//case WM_LBUTTONDBLCLK :	*nCap = 0;						break;
		case WM_RBUTTONDOWN :		*nCap = MOUSE_OPERATION_RDOWN;	break;
		case WM_RBUTTONUP :			*nCap = MOUSE_OPERATION_RUP;	break;
		case WM_MOUSEMOVE :			*nCap = MOUSE_OPERATION_MOVE;	// no break
		default :					bProc = FALSE;					break;
	}

	return bProc;
}

void WriteLog(char *string)
{
	FILE *fp = NULL;
	char TempDir[MAX_PATH];
	char fname[MAX_PATH];

	GetTempPath( sizeof(TempDir), TempDir);
	sprintf(fname, "%s\\%s",TempDir, DLL_LOG_FNAME);
	fp = fopen(fname, "a");
	if ( fp )
	{
		fputs(string, fp);
		fclose(fp);
	}
}

DWORD ElapsedTime()
{
	DWORD msec = 0;
	SYSTEMTIME	new_tm;

	GetSystemTime(&new_tm);
	if ( new_tm.wYear != 0 ) //TODO�F���܂Ɏ��Ȃ��Ƃ�������	
	{
		int temp ;

//		temp = new_tm.wYear - gOld_tm.wYear;
//		temp = new_tm.wMonth - gOld_tm.wMonth;

		// ��
		temp = new_tm.wDay - gOld_tm.wDay;
		msec += temp;
		msec *= MAX_HOUR;

		// ����
		temp = new_tm.wHour - gOld_tm.wHour;
		msec += temp;
		msec *= MAX_MINUTES;

		// ��
		temp = new_tm.wMinute - gOld_tm.wMinute;
		msec += temp;
		msec *= MAX_MINUTES;

		// �b
		temp = new_tm.wSecond - gOld_tm.wSecond;
		msec += temp;
		msec *= MAX_MSEONDS;

		// �~���b
		temp = new_tm.wMilliseconds - gOld_tm.wMilliseconds;
		msec += temp;

		gOld_tm = new_tm;
	}

	return msec;
}

// utility �Ɉړ� +
BOOL IsKeyDown(int nKey)
{
	BOOL bReturn = FALSE;

#if 0 // �����̃L�[���ꊇ�Ŋm�F����ꍇ
	BYTE bKey[MAX_PATH];
	if ( GetKeyboardState(bKey) ) // ����̃L�[�𔻕ʂ��邾���Ȃ� GetAsyncKeyState(vk) �ł��ǂ�
	{
		// KEY_UP�i�L�[�𗣂����j�C�x���g�ł���Ζ���
		if ( IsKeyDown(bKey[nKey] )
		{
			bReturn = TRUE;
		}
	}
#else
	short sKey = GetAsyncKeyState(nKey);

	if ( IsKeyDown(sKey) )
	{
		bReturn = TRUE;
	}
#endif

	return bReturn;
}

BOOL IsShiftKeyDown()
{
	BOOL bReturn = FALSE;
	short sLkey = GetAsyncKeyState(VK_LSHIFT);
	short sRkey = GetAsyncKeyState(VK_RSHIFT);

	if ( IsKeyDown(sLkey) ||
		 IsKeyDown(sRkey) )
	{
		bReturn = TRUE;
	}

	return bReturn;
}

BOOL IsControlKeyDown()
{
	BOOL bReturn = FALSE;
	short sLkey = GetAsyncKeyState(VK_LCONTROL);
	short sRkey = GetAsyncKeyState(VK_RCONTROL);

	if ( IsKeyDown(sLkey) ||
		 IsKeyDown(sRkey) )
	{
		bReturn = TRUE;
	}

	return bReturn;
}

BOOL IsKeyDown(short sKey)
{
	BOOL bReturn = FALSE;

	if ( sKey & 0x8000 )
	{
		bReturn = TRUE;
	}
	return bReturn;
}

BOOL IsKeyDown(BYTE bKey)
{
	BOOL bReturn = FALSE;

	if ( bKey & 0x80 )
	{
		bReturn = TRUE;
	}
	return bReturn;
}

void MyTolower(int *nKey)
{
	if ( ! nKey )
	{
		return;
	}

	if ( 'A' <= *nKey && *nKey <= 'Z' )
	{
		//tolower(*nKey);
		*nKey += 'a' - 'A';
	}
}

// utility �Ɉړ� -
 