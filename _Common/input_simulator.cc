// input_simulator.cc : キーボード・マウスの入力をエミュレートする

#include "input_simulator.h"

namespace
{
	// スクリーン座標を SendInput の絶対座標（0〜65535）に変換
	LONG ToAbsolute(LONG pos, int screen_size)
	{
		return pos * 65535 / (screen_size - 1);
	}
}

void InputSimulator::SendMouse(DWORD flags, LONG dx, LONG dy)
{
	INPUT input = {};
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = flags;
	::SendInput(1, &input, sizeof(INPUT));
}

void InputSimulator::MouseMove(CPoint pt)
{
	SendMouse(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,
		ToAbsolute(pt.x, ::GetSystemMetrics(SM_CXSCREEN)),
		ToAbsolute(pt.y, ::GetSystemMetrics(SM_CYSCREEN)));
}

void InputSimulator::MouseLButtonDown()	{ SendMouse(MOUSEEVENTF_LEFTDOWN); }
void InputSimulator::MouseLButtonUp()		{ SendMouse(MOUSEEVENTF_LEFTUP); }
void InputSimulator::MouseRButtonDown()	{ SendMouse(MOUSEEVENTF_RIGHTDOWN); }
void InputSimulator::MouseRButtonUp()		{ SendMouse(MOUSEEVENTF_RIGHTUP); }

void InputSimulator::MouseLButtonClick()
{
	MouseLButtonDown();
	MouseLButtonUp();
}

void InputSimulator::MouseRButtonClick()
{
	MouseRButtonDown();
	MouseRButtonUp();
}

void InputSimulator::KeyAction(WORD virtual_key, BOOL hold)
{
	// 仮想キーコードの 'A'〜'Z' は大文字側なので、小文字の文字コードを変換する
	if (_T('a') <= virtual_key && virtual_key <= _T('z'))
	{
		virtual_key = static_cast<WORD>(virtual_key - (_T('a') - _T('A')));
	}
	SendInputKey(virtual_key, hold);
}

void InputSimulator::FunctionKeyAction(BYTE virtual_key, BOOL hold)
{
	if (virtual_key != static_cast<BYTE>(kVkNone))
	{
		SendInputKey(virtual_key, hold);
	}
}

void InputSimulator::SendInputKey(WORD virtual_key, BOOL hold)
{
	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = virtual_key;
	input.ki.wScan = static_cast<WORD>(::MapVirtualKey(virtual_key, MAPVK_VK_TO_VSC));
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	input.ki.dwExtraInfo = ::GetMessageExtraInfo();
	::SendInput(1, &input, sizeof(INPUT));

	if (!hold)
	{
		input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
		::SendInput(1, &input, sizeof(INPUT));
	}
}
