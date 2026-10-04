// event_hookd.cc : マウス・キーボードのグローバルフック DLL

#include "stdafx.h"
#include "event_hookd.h"

#include <cstdio>

// フックの登録状態と、前回イベントの時刻
// SetWindowsHookEx(WH_KEYBOARD / WH_MOUSE, ..., 0) のグローバルフックは、フックされた各プロセスに
// この DLL が読み込まれて動く。前回イベントの時刻などをプロセス間で共有するため .shared を共有セクションにする。
// （EventHookd.def の SECTIONS はリンクに使っていないので効かない。共有はこの指定で有効にしている。
//   共有セクションの変数には初期化子が必須）
#pragma comment(linker, "/SECTION:.shared,RWS")
#pragma data_seg(".shared")
HHOOK		g_hKeyHook = NULL;
HHOOK		g_hMouseHook = NULL;
SYSTEMTIME	g_lastEventTime = {0,0,0,0,0,0,0,0};
#pragma data_seg()

namespace
{
	HINSTANCE	instance = nullptr;
	BOOL		debug_enabled = FALSE;

	constexpr char kLogFileName[] = "MacroLog.txt";

	// MacroTool の設定ファイル1行と同じ並び
	// 実行回数, 遅延(ms), イベント種別, X, Y, マウス操作, 修飾キー, キー種別, キー文字列, コメント
	constexpr char kLogFormat[] = "%3d,%8d,%2d,%4d,%4d,%2d,%2d,%2d,%s,%s\n";

	constexpr int kDefaultExecuteCount = 1;
	constexpr int kEventMouse = 1;
	constexpr int kEventKey = 2;

	constexpr DWORD kModifierShift = 0x0001;
	constexpr DWORD kModifierCtrl = 0x0002;

	// MacroTool のマウス操作の番号
	constexpr int kMouseOpLDown = 1;
	constexpr int kMouseOpLUp = 2;
	constexpr int kMouseOpRDown = 4;
	constexpr int kMouseOpRUp = 5;
	constexpr int kMouseOpNone = -1;		// 記録しない

	bool IsPressed(int virtual_key)
	{
		return (GetAsyncKeyState(virtual_key) & 0x8000) != 0;
	}

	bool IsShiftPressed()	{ return IsPressed(VK_LSHIFT) || IsPressed(VK_RSHIFT); }
	bool IsControlPressed()	{ return IsPressed(VK_LCONTROL) || IsPressed(VK_RCONTROL); }

	// SYSTEMTIME を 100ns 単位の通し番号にする（変換できなければ 0）
	ULONGLONG ToFileTimeValue(const SYSTEMTIME& time)
	{
		FILETIME file_time;
		if (!SystemTimeToFileTime(&time, &file_time))
		{
			return 0;
		}
		ULARGE_INTEGER value;
		value.LowPart = file_time.dwLowDateTime;
		value.HighPart = file_time.dwHighDateTime;
		return value.QuadPart;
	}

	// 前回のイベントからの経過時間(ms)
	// （日・時・分…の差を積み上げる計算では、月をまたぐと日の差が負になって値が壊れていた）
	DWORD ElapsedTime()
	{
		SYSTEMTIME now;
		GetSystemTime(&now);

		const ULONGLONG now_value = ToFileTimeValue(now);
		if (now_value == 0)	// まれに取得できないことがあった
		{
			return 0;
		}
		const ULONGLONG last_value = ToFileTimeValue(g_lastEventTime);
		g_lastEventTime = now;

		constexpr ULONGLONG kFileTimePerMsec = 10000;
		return (last_value != 0 && now_value > last_value)
			? static_cast<DWORD>((now_value - last_value) / kFileTimePerMsec) : 0;
	}

	void WriteLog(const char* text)
	{
		if (!debug_enabled)
		{
			return;
		}

		FILE* fp = fopen(kLogFileName, "a");
		if (fp)
		{
			fputs(text, fp);
			fclose(fp);
		}
	}

	// 押された英数字キーと修飾キーを取り出す（英数字以外・キーを離したときは false）
	bool GetKeyParameter(WPARAM w_param, DWORD& modifiers, char& key)
	{
		int virtual_key = static_cast<int>(w_param);

		// ファンクションキー等には未対応
		// （仮想キーコードの英字は 'A'〜'Z' だけ。isalpha だと 'a'〜'z' と同じ値の
		//   テンキー・F1〜F11 まで英字として通ってしまう）
		const bool is_letter = ('A' <= virtual_key && virtual_key <= 'Z');
		const bool is_digit = ('0' <= virtual_key && virtual_key <= '9');
		if (!is_letter && !is_digit)
		{
			return false;
		}

		// w_param だけではキーを押したときと離したとき両方が来るので、押されているかを確かめる
		if (!IsPressed(virtual_key))
		{
			return false;
		}

		if (IsControlPressed())
		{
			modifiers |= kModifierCtrl;
		}
		if (IsShiftPressed())
		{
			modifiers |= kModifierShift;
		}
		else if (is_letter)
		{
			// Shift が押されていなければ小文字にする
			virtual_key += 'a' - 'A';
		}
		key = static_cast<char>(virtual_key);
		return true;
	}

	// マウスのメッセージを MacroTool のマウス操作に変換する（移動などは記録しない）
	int ToMouseOperation(WPARAM message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:	return kMouseOpLDown;
		case WM_LBUTTONUP:		return kMouseOpLUp;
		case WM_RBUTTONDOWN:	return kMouseOpRDown;
		case WM_RBUTTONUP:		return kMouseOpRUp;
		default:				return kMouseOpNone;
		}
	}

	LRESULT CALLBACK KeyHookProc(int code, WPARAM w_param, LPARAM l_param)
	{
		if (code == HC_ACTION)
		{
			DWORD modifiers = 0;
			char key[2] = {};
			if (GetKeyParameter(w_param, modifiers, key[0]))
			{
				char text[MAX_PATH];
				sprintf_s(text, kLogFormat,
					kDefaultExecuteCount, ElapsedTime(), kEventKey,
					0, 0, 0,
					modifiers, 0, key, "");
				WriteLog(text);
			}
		}
		return CallNextHookEx(g_hKeyHook, code, w_param, l_param);
	}

	LRESULT CALLBACK MouseHookProc(int code, WPARAM w_param, LPARAM l_param)
	{
		if (code == HC_ACTION)
		{
			const int operation = ToMouseOperation(w_param);
			if (operation != kMouseOpNone)
			{
				POINT pt;
				GetCursorPos(&pt);

				char text[MAX_PATH];
				sprintf_s(text, kLogFormat,
					kDefaultExecuteCount, ElapsedTime(), kEventMouse,
					pt.x, pt.y, operation,
					0, 0, "", "");
				WriteLog(text);
			}
		}
		return CallNextHookEx(g_hMouseHook, code, w_param, l_param);
	}
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		instance = module;
	}
	return TRUE;
}

HOOKD_API BOOL StartKeyHook()
{
	g_hKeyHook = SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, instance, 0);
	if (!g_hKeyHook)
	{
		return FALSE;
	}
	GetSystemTime(&g_lastEventTime);
	return TRUE;
}

HOOKD_API BOOL StartMouseHook()
{
	g_hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, instance, 0);
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

HOOKD_API void DebugMode(BOOL is_debug)
{
	debug_enabled = is_debug;
}
