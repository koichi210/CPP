// input_simulator.h : キーボード・マウスの入力をエミュレートする

#ifndef COMMON_INPUT_SIMULATOR_H_
#define COMMON_INPUT_SIMULATOR_H_

#include <afxwin.h>

constexpr int kVkNone = -1;		// キー指定なし

class InputSimulator
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

	// キーボード（hold = TRUE なら押しっぱなし、FALSE なら押して離す）
	static void KeyAction(WORD virtual_key, BOOL hold = FALSE);			// 'a'〜'z' は 'A'〜'Z' のキーとして送る
	static void FunctionKeyAction(BYTE virtual_key, BOOL hold = FALSE);	// kVkNone なら何もしない
	static void SendInputKey(WORD virtual_key, BOOL hold = FALSE);

private:
	static void SendMouse(DWORD flags, LONG dx = 0, LONG dy = 0);
};

#endif  // COMMON_INPUT_SIMULATOR_H_
