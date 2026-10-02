// EventHookd.cpp : マウス・キーボードのグローバルフック DLL

#include "stdafx.h"
#include "EventHookd.h"

#include <cctype>
#include <cstdio>

// フックの登録状態と、前回イベントの時刻
// （.shared セクションに置いているが、.def をリンクしていないため現状はプロセス間で共有されない）
#pragma data_seg(".shared")
HHOOK		g_hKeyHook = NULL;
HHOOK		g_hMouseHook = NULL;
SYSTEMTIME	g_lastEventTime = {0,0,0,0,0,0,0,0};
#pragma data_seg()

namespace
{
	HINSTANCE	g_hInstance = nullptr;
	BOOL		g_isDebug = FALSE;

	constexpr char LOG_FILE_NAME[] = "MacroLog.txt";

	// MacroTool の設定ファイル1行と同じ並び
	// 実行回数, 遅延(ms), イベント種別, X, Y, マウス操作, 修飾キー, キー種別, キー文字列, コメント
	constexpr char LOG_FORMAT[] = "%3d,%8d,%2d,%4d,%4d,%2d,%2d,%2d,%s,%s\n";

	constexpr int DEFAULT_EXECUTE_COUNT = 1;
	constexpr int EVENT_MOUSE = 1;
	constexpr int EVENT_KEY = 2;

	constexpr DWORD MODIFIER_SHIFT = 0x0001;
	constexpr DWORD MODIFIER_CTRL = 0x0002;

	// MacroTool のマウス操作の番号
	constexpr int MOUSEOP_LDOWN = 1;
	constexpr int MOUSEOP_LUP = 2;
	constexpr int MOUSEOP_RDOWN = 4;
	constexpr int MOUSEOP_RUP = 5;
	constexpr int MOUSEOP_NONE = -1;		// 記録しない

	constexpr int HOURS_PER_DAY = 24;
	constexpr int MINUTES_PER_HOUR = 60;
	constexpr int SECONDS_PER_MINUTE = 60;
	constexpr int MSEC_PER_SECOND = 1000;

	bool IsPressed(int virtualKey)
	{
		return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
	}

	bool IsShiftPressed()	{ return IsPressed(VK_LSHIFT) || IsPressed(VK_RSHIFT); }
	bool IsControlPressed()	{ return IsPressed(VK_LCONTROL) || IsPressed(VK_RCONTROL); }

	// 前回のイベントからの経過時間(ms)
	DWORD ElapsedTime()
	{
		DWORD msec = 0;
		SYSTEMTIME now;

		GetSystemTime(&now);
		if (now.wYear != 0)	// まれに取得できないことがあった
		{
			msec += now.wDay - g_lastEventTime.wDay;
			msec *= HOURS_PER_DAY;
			msec += now.wHour - g_lastEventTime.wHour;
			msec *= MINUTES_PER_HOUR;
			msec += now.wMinute - g_lastEventTime.wMinute;
			msec *= SECONDS_PER_MINUTE;
			msec += now.wSecond - g_lastEventTime.wSecond;
			msec *= MSEC_PER_SECOND;
			msec += now.wMilliseconds - g_lastEventTime.wMilliseconds;

			g_lastEventTime = now;
		}
		return msec;
	}

	void WriteLog(const char* text)
	{
		if (!g_isDebug)
		{
			return;
		}

		FILE* fp = fopen(LOG_FILE_NAME, "a");
		if (fp)
		{
			fputs(text, fp);
			fclose(fp);
		}
	}

	// 押された英数字キーと修飾キーを取り出す（英数字以外・キーを離したときは false）
	bool GetKeyParameter(WPARAM wParam, DWORD& modifiers, char& key)
	{
		int virtualKey = static_cast<int>(wParam);

		// ファンクションキー等には未対応
		if (!isalpha(virtualKey) && !isdigit(virtualKey))
		{
			return false;
		}

		// wParam だけではキーを押したときと離したとき両方が来るので、押されているかを確かめる
		if (!IsPressed(virtualKey))
		{
			return false;
		}

		if (IsControlPressed())
		{
			modifiers |= MODIFIER_CTRL;
		}
		if (IsShiftPressed())
		{
			modifiers |= MODIFIER_SHIFT;
		}
		else if ('A' <= virtualKey && virtualKey <= 'Z')
		{
			// Shift が押されていなければ小文字にする
			virtualKey += 'a' - 'A';
		}
		key = static_cast<char>(virtualKey);
		return true;
	}

	// マウスのメッセージを MacroTool のマウス操作に変換する（移動などは記録しない）
	int ToMouseOperation(WPARAM message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:	return MOUSEOP_LDOWN;
		case WM_LBUTTONUP:		return MOUSEOP_LUP;
		case WM_RBUTTONDOWN:	return MOUSEOP_RDOWN;
		case WM_RBUTTONUP:		return MOUSEOP_RUP;
		default:				return MOUSEOP_NONE;
		}
	}

	LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == HC_ACTION)
		{
			DWORD modifiers = 0;
			char key[2] = {};
			if (GetKeyParameter(wParam, modifiers, key[0]))
			{
				char text[MAX_PATH];
				sprintf_s(text, LOG_FORMAT,
					DEFAULT_EXECUTE_COUNT, ElapsedTime(), EVENT_KEY,
					0, 0, 0,
					modifiers, 0, key, "");
				WriteLog(text);
			}
		}
		return CallNextHookEx(g_hKeyHook, nCode, wParam, lParam);
	}

	LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == HC_ACTION)
		{
			const int operation = ToMouseOperation(wParam);
			if (operation != MOUSEOP_NONE)
			{
				POINT pt;
				GetCursorPos(&pt);

				char text[MAX_PATH];
				sprintf_s(text, LOG_FORMAT,
					DEFAULT_EXECUTE_COUNT, ElapsedTime(), EVENT_MOUSE,
					pt.x, pt.y, operation,
					0, 0, "", "");
				WriteLog(text);
			}
		}
		return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hModule;
	}
	return TRUE;
}

HOOKD_API BOOL StartKeyHook()
{
	g_hKeyHook = SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, g_hInstance, 0);
	if (!g_hKeyHook)
	{
		return FALSE;
	}
	GetSystemTime(&g_lastEventTime);
	return TRUE;
}

HOOKD_API BOOL StartMouseHook()
{
	g_hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, g_hInstance, 0);
	if (!g_hMouseHook)
	{
		return FALSE;
	}
	GetSystemTime(&g_lastEventTime);
	return TRUE;
}

HOOKD_API BOOL StopKeyHook()
{
	return UnhookWindowsHookEx(g_hKeyHook) ? TRUE : FALSE;
}

HOOKD_API BOOL StopMouseHook()
{
	return UnhookWindowsHookEx(g_hMouseHook) ? TRUE : FALSE;
}

HOOKD_API void DebugMode(BOOL IsDebug)
{
	g_isDebug = IsDebug;
}
