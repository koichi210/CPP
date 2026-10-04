// event_hookd.h : マウス・キーボードの操作を記録する低レベルフック DLL の公開関数
//   MacroTool は LoadLibrary / GetProcAddress で名前を指定して呼ぶ。
//   関数名と呼び出し規約（extern "C" の __cdecl）は MacroTool 側の型と合わせてあるので変えないこと
//
//   低レベルフック（WH_MOUSE_LL / WH_KEYBOARD_LL）は他のプロセスに DLL を読み込ませず、
//   フックを登録したスレッド（MacroTool の画面のスレッド）で呼ばれる。
//   そのため Start*Hook / Stop*Hook は、メッセージループを回しているスレッドから呼ぶこと

#ifndef EVENTRECORDER_EVENTHOOKD_EVENT_HOOKD_H_
#define EVENTRECORDER_EVENTHOOKD_EVENT_HOOKD_H_

#define HOOKD_API extern "C" __declspec(dllexport)

// 記録の書き込み先（1操作1行、MacroTool の設定ファイルの1行と同じ並び）。Start*Hook の前に呼ぶ
HOOKD_API void SetLogFile(const char* path);

HOOKD_API BOOL StartKeyHook();
HOOKD_API BOOL StartMouseHook();
HOOKD_API BOOL StopKeyHook();
HOOKD_API BOOL StopMouseHook();

#endif  // EVENTRECORDER_EVENTHOOKD_EVENT_HOOKD_H_
