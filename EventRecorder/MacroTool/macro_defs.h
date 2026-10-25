// macro_defs.h : マクロ1行分のデータと、設定画面・実行で共通の定数

#ifndef EVENTRECORDER_MACROTOOL_MACRO_DEFS_H_
#define EVENTRECORDER_MACROTOOL_MACRO_DEFS_H_

constexpr int kMaxEventCount = 700;	// 設定できる行数

// イベント種別（設定ファイルに数値で保存する）
enum class EventKind : int
{
	kNone	= 0,	// 未設定の行
	kMouse	= 1,
	kKey	= 2,
};

// マウス操作（設定ファイルに数値で保存する。コンボボックスの並びと同じ）
enum MouseOperation
{
	kMouseOpLClick,
	kMouseOpLDown,
	kMouseOpLUp,
	kMouseOpRClick,
	kMouseOpRDown,
	kMouseOpRUp,
	kMouseOpMove,
	kMouseOpCount,
};

// キー種別（設定ファイルに数値で保存する。コンボボックスの並びと同じ）
enum KeyKind
{
	kKeyKindUser	= 0,	// text の文字を順に入力する
	kKeyKindF1		= 1,
	kKeyKindF12		= 12,
	kKeyKindCount,
};

// 修飾キー（ビットの組み合わせ）
constexpr DWORD kModifierShift	= 0x0001;
constexpr DWORD kModifierCtrl	= 0x0002;
constexpr DWORD kModifierAlt	= 0x0004;

inline constexpr LPCTSTR kMouseOperationNames[kMouseOpCount] =
{
	_T("左クリック"), _T("左DN"), _T("左UP"), _T("右クリック"), _T("右DN"), _T("右UP"), _T("移動"),
};

inline constexpr LPCTSTR kKeyKindNames[kKeyKindCount] =
{
	_T("上記"), _T("F1"), _T("F2"), _T("F3"), _T("F4"), _T("F5"), _T("F6"),
	_T("F7"), _T("F8"), _T("F9"), _T("F10"), _T("F11"), _T("F12"),
};

struct MacroKey
{
	DWORD	modifiers;			// kModifier* の組み合わせ
	int		key_kind;			// KeyKind
	char	text[MAX_PATH];		// kKeyKindUser のとき入力する文字列
};

struct MacroMouse
{
	CPoint	pt;					// スクリーン座標
	int		operation;			// MouseOperation
};

struct MacroEvent
{
	int			exec_count;			// 実行回数
	DWORD		sleep_msec;			// 実行前の待ち時間(ms)
	EventKind	kind;
	MacroKey	key;
	MacroMouse	mouse;
	char		comment[MAX_PATH];
};

// 再生では1文字をそのまま仮想キーコードとして送るので、送れるのは英字・数字・空白だけ
// 英字は小文字にする（記録と同じ。大文字は Shift を修飾キーで指定する）。送れない文字は '\0'
// 設定画面の入力欄と、再生（古いファイルや手で書いたファイルの文字も通る）の両方で使う
inline char ToPlayableKeyChar(char c)
{
	if ('A' <= c && c <= 'Z')
	{
		return static_cast<char>(c + ('a' - 'A'));
	}
	const bool playable = ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') || c == ' ';
	return playable ? c : '\0';
}

#endif  // EVENTRECORDER_MACROTOOL_MACRO_DEFS_H_
