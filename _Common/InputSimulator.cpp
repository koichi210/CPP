// InputSimulator.cpp : キーボード・マウスの入力をエミュレートする

#include "InputSimulator.h"

namespace
{
	// スクリーン座標を SendInput の絶対座標（0〜65535）に変換
	LONG ToAbsolute(LONG pos, int screenSize)
	{
		return pos * 65535 / (screenSize - 1);
	}
}

void CInputSimulator::SendMouse(DWORD flags, LONG dx, LONG dy)
{
	INPUT input = {};
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = flags;
	::SendInput(1, &input, sizeof(INPUT));
}

void CInputSimulator::MouseMove(CPoint pt)
{
	SendMouse(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,
		ToAbsolute(pt.x, ::GetSystemMetrics(SM_CXSCREEN)),
		ToAbsolute(pt.y, ::GetSystemMetrics(SM_CYSCREEN)));
}

void CInputSimulator::MouseLButtonDown()	{ SendMouse(MOUSEEVENTF_LEFTDOWN); }
void CInputSimulator::MouseLButtonUp()		{ SendMouse(MOUSEEVENTF_LEFTUP); }
void CInputSimulator::MouseRButtonDown()	{ SendMouse(MOUSEEVENTF_RIGHTDOWN); }
void CInputSimulator::MouseRButtonUp()		{ SendMouse(MOUSEEVENTF_RIGHTUP); }

void CInputSimulator::MouseLButtonClick()
{
	MouseLButtonDown();
	MouseLButtonUp();
}

void CInputSimulator::MouseRButtonClick()
{
	MouseRButtonDown();
	MouseRButtonUp();
}

void CInputSimulator::KeyAction(WORD virtualKey, BOOL bHold)
{
	// 仮想キーコードの 'A'〜'Z' は大文字側なので、小文字の文字コードを変換する
	if (_T('a') <= virtualKey && virtualKey <= _T('z'))
	{
		virtualKey = static_cast<WORD>(virtualKey - (_T('a') - _T('A')));
	}
	SendInputKey(virtualKey, bHold);
}

void CInputSimulator::FunctionKeyAction(BYTE virtualKey, BOOL bHold)
{
	if (virtualKey != static_cast<BYTE>(VK_NONE))
	{
		SendInputKey(virtualKey, bHold);
	}
}

void CInputSimulator::SendInputKey(WORD virtualKey, BOOL bHold)
{
	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = virtualKey;
	input.ki.wScan = static_cast<WORD>(::MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC));
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	input.ki.dwExtraInfo = ::GetMessageExtraInfo();
	::SendInput(1, &input, sizeof(INPUT));

	if (!bHold)
	{
		input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
		::SendInput(1, &input, sizeof(INPUT));
	}
}
