// InputSimulator.h : キーボード・マウスの入力をエミュレートする

#pragma once

#include <afxwin.h>

constexpr int VK_NONE = -1;		// キー指定なし

class CInputSimulator
{
public:
	// マウス（座標はスクリーン座標）
	static void MouseMove(CPoint pt);
	static void MouseLButtonDown();
	static void MouseLButtonUp();
	static void MouseLButtonClick();
	static void MouseRButtonDown();
	static void MouseRButtonUp();
	static void MouseRButtonClick();

	// キーボード（bHold = TRUE なら押しっぱなし、FALSE なら押して離す）
	static void KeyAction(WORD virtualKey, BOOL bHold = FALSE);			// 'a'〜'z' は 'A'〜'Z' のキーとして送る
	static void FunctionKeyAction(BYTE virtualKey, BOOL bHold = FALSE);	// VK_NONE なら何もしない
	static void SendInputKey(WORD virtualKey, BOOL bHold = FALSE);

private:
	static void SendMouse(DWORD flags, LONG dx = 0, LONG dy = 0);
};
