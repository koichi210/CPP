// event_hookd.h : マウス・キーボードのグローバルフック DLL の公開関数
//   MacroTool は LoadLibrary / GetProcAddress で名前を指定して呼ぶ。
//   関数名と呼び出し規約（extern "C" の __cdecl）は MacroTool 側の型と合わせてあるので変えないこと

#pragma once

#define HOOKD_API extern "C" __declspec(dllexport)

HOOKD_API BOOL StartKeyHook();
HOOKD_API BOOL StartMouseHook();
HOOKD_API BOOL StopKeyHook();
HOOKD_API BOOL StopMouseHook();
HOOKD_API void DebugMode(BOOL is_debug);	// TRUE のときだけ操作をログファイルに書く
