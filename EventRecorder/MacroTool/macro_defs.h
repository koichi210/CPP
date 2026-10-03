// macro_defs.h : マクロ1行分のデータと、設定画面・実行で共通の定数

#pragma once

constexpr int MAX_EVENT_COUNT = 700;	// 設定できる行数

// イベント種別（設定ファイルに数値で保存する）
enum class EventKind : int
{
	None	= 0,	// 未設定の行
	Mouse	= 1,
	Key		= 2,
};

// マウス操作（設定ファイルに数値で保存する。コンボボックスの並びと同じ）
enum MouseOperation
{
	MOUSEOP_LCLICK,
	MOUSEOP_LDOWN,
	MOUSEOP_LUP,
	MOUSEOP_RCLICK,
	MOUSEOP_RDOWN,
	MOUSEOP_RUP,
	MOUSEOP_MOVE,
	MOUSEOP_COUNT,
};

// キー種別（設定ファイルに数値で保存する。コンボボックスの並びと同じ）
enum KeyKind
{
	KEYKIND_USER	= 0,	// text の文字を順に入力する
	KEYKIND_F1		= 1,
	KEYKIND_F12		= 12,
	KEYKIND_COUNT,
};

// 修飾キー（ビットの組み合わせ）
constexpr DWORD MODIFIER_SHIFT	= 0x0001;
constexpr DWORD MODIFIER_CTRL	= 0x0002;
constexpr DWORD MODIFIER_ALT	= 0x0004;

inline constexpr LPCTSTR MOUSE_OPERATION_NAMES[MOUSEOP_COUNT] =
{
	_T("左クリック"), _T("左DN"), _T("左UP"), _T("右クリック"), _T("右DN"), _T("右UP"), _T("移動"),
};

inline constexpr LPCTSTR KEY_KIND_NAMES[KEYKIND_COUNT] =
{
	_T("上記"), _T("F1"), _T("F2"), _T("F3"), _T("F4"), _T("F5"), _T("F6"),
	_T("F7"), _T("F8"), _T("F9"), _T("F10"), _T("F11"), _T("F12"),
};

struct MACROKEY
{
	DWORD	modifiers;			// MODIFIER_* の組み合わせ
	int		keyKind;			// KeyKind
	char	text[MAX_PATH];		// KEYKIND_USER のとき入力する文字列
};

struct MACROMOUSE
{
	CPoint	pt;					// スクリーン座標
	int		operation;			// MouseOperation
};

struct MACROEVENT
{
	int			execCount;			// 実行回数
	DWORD		sleepMsec;			// 実行前の待ち時間(ms)
	EventKind	kind;
	MACROKEY	key;
	MACROMOUSE	mouse;
	char		comment[MAX_PATH];
};
