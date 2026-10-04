// event_hookd.cc : マウス・キーボードの操作を記録する低レベルフック DLL

#include "stdafx.h"
#include "event_hookd.h"

#include <cstdio>

namespace
{
	HINSTANCE	instance = nullptr;
	HHOOK		key_hook = nullptr;
	HHOOK		mouse_hook = nullptr;
	char		log_path[MAX_PATH] = {};
	DWORD		last_event_tick = 0;	// 前回の操作の時刻（GetTickCount の値）

	// MacroTool の設定ファイル1行と同じ並び
	// 実行回数, 遅延(ms), イベント種別, X, Y, マウス操作, 修飾キー, キー種別, キー文字列, コメント
	constexpr char kLogFormat[] = "%d,%lu,%d,%ld,%ld,%d,%lu,%d,%s,%s\n";

	constexpr int kDefaultExecuteCount = 1;
	constexpr int kEventMouse = 1;
	constexpr int kEventKey = 2;

	// MacroTool の修飾キー（ビットの組み合わせ）
	constexpr DWORD kModifierShift = 0x0001;
	constexpr DWORD kModifierCtrl = 0x0002;
	constexpr DWORD kModifierAlt = 0x0004;

	// MacroTool のキー種別（0 はキー文字列を入力、1〜12 は F1〜F12）
	constexpr int kKeyKindUser = 0;
	constexpr int kKeyKindF1 = 1;

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

	// 前回の操作からの経過時間(ms)。event_tick は低レベルフックが渡す操作の時刻
	DWORD ElapsedTime(DWORD event_tick)
	{
		const DWORD elapsed = event_tick - last_event_tick;	// 49日で一周しても差は正しく求まる
		last_event_tick = event_tick;
		return elapsed;
	}

	// MacroTool 自身の画面への操作（「記録終了」ボタンを押すなど）は記録しない
	bool IsOwnWindow(HWND wnd)
	{
		DWORD process_id = 0;
		return wnd != nullptr
			&& GetWindowThreadProcessId(wnd, &process_id) != 0
			&& process_id == GetCurrentProcessId();
	}

	void WriteLog(const char* text)
	{
		FILE* fp = nullptr;
		if (log_path[0] != '\0' && fopen_s(&fp, log_path, "a") == 0)
		{
			fputs(text, fp);
			fclose(fp);
		}
	}

	// 押されたキーを MacroTool のキー種別とキー文字列にする（英数字と F1〜F12 以外は false）
	bool GetKeyParameter(DWORD virtual_key, int& key_kind, char& key)
	{
		if (VK_F1 <= virtual_key && virtual_key <= VK_F12)
		{
			key_kind = kKeyKindF1 + static_cast<int>(virtual_key - VK_F1);
			return true;
		}

		// 仮想キーコードの英字は 'A'〜'Z' だけ。'a'〜'z' と同じ値はテンキーや F1〜F11 なので使わない
		const bool is_letter = ('A' <= virtual_key && virtual_key <= 'Z');
		const bool is_digit = ('0' <= virtual_key && virtual_key <= '9');
		if (!is_letter && !is_digit)
		{
			return false;
		}

		// 再生では、英字は小文字で書いておき Shift を修飾キーとして押す
		key_kind = kKeyKindUser;
		key = static_cast<char>(is_letter ? virtual_key + ('a' - 'A') : virtual_key);
		return true;
	}

	DWORD GetModifiers(bool alt_down)
	{
		DWORD modifiers = 0;
		if (IsPressed(VK_SHIFT))	modifiers |= kModifierShift;
		if (IsPressed(VK_CONTROL))	modifiers |= kModifierCtrl;
		if (alt_down)				modifiers |= kModifierAlt;
		return modifiers;
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
		// 押したときだけ記録する（Alt と一緒に押すと WM_SYSKEYDOWN になる）
		if (code == HC_ACTION && (w_param == WM_KEYDOWN || w_param == WM_SYSKEYDOWN)
			&& !IsOwnWindow(GetForegroundWindow()))
		{
			const KBDLLHOOKSTRUCT* info = reinterpret_cast<const KBDLLHOOKSTRUCT*>(l_param);
			int key_kind = kKeyKindUser;
			char key[2] = {};
			if (GetKeyParameter(info->vkCode, key_kind, key[0]))
			{
				char text[MAX_PATH];
				sprintf_s(text, kLogFormat,
					kDefaultExecuteCount, ElapsedTime(info->time), kEventKey,
					0L, 0L, 0,
					GetModifiers((info->flags & LLKHF_ALTDOWN) != 0), key_kind, key, "");
				WriteLog(text);
			}
		}
		return CallNextHookEx(key_hook, code, w_param, l_param);
	}

	LRESULT CALLBACK MouseHookProc(int code, WPARAM w_param, LPARAM l_param)
	{
		if (code == HC_ACTION)
		{
			const MSLLHOOKSTRUCT* info = reinterpret_cast<const MSLLHOOKSTRUCT*>(l_param);
			const int operation = ToMouseOperation(w_param);

			// 座標は再生と同じ物差しにするため、この DLL を使うプロセスから見た位置を使う
			POINT pt;
			if (operation != kMouseOpNone && GetCursorPos(&pt) && !IsOwnWindow(WindowFromPoint(pt)))
			{
				char text[MAX_PATH];
				sprintf_s(text, kLogFormat,
					kDefaultExecuteCount, ElapsedTime(info->time), kEventMouse,
					pt.x, pt.y, operation,
					0UL, kKeyKindUser, "", "");
				WriteLog(text);
			}
		}
		return CallNextHookEx(mouse_hook, code, w_param, l_param);
	}

	// フックを登録する。すでに登録してあれば何もしない
	BOOL StartHook(HHOOK& hook, int hook_id, HOOKPROC proc)
	{
		if (hook == nullptr)
		{
			hook = SetWindowsHookEx(hook_id, proc, instance, 0);
			last_event_tick = GetTickCount();
		}
		return hook != nullptr ? TRUE : FALSE;
	}

	BOOL StopHook(HHOOK& hook)
	{
		if (hook == nullptr)
		{
			return TRUE;
		}
		const BOOL result = UnhookWindowsHookEx(hook);
		hook = nullptr;
		return result;
	}
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*reserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		instance = module;
	}
	return TRUE;
}

HOOKD_API void SetLogFile(const char* path)
{
	strcpy_s(log_path, path != nullptr ? path : "");
}

HOOKD_API BOOL StartKeyHook()	{ return StartHook(key_hook, WH_KEYBOARD_LL, KeyHookProc); }
HOOKD_API BOOL StartMouseHook()	{ return StartHook(mouse_hook, WH_MOUSE_LL, MouseHookProc); }
HOOKD_API BOOL StopKeyHook()	{ return StopHook(key_hook); }
HOOKD_API BOOL StopMouseHook()	{ return StopHook(mouse_hook); }
