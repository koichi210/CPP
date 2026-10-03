// binary_edit_win32.cc : アプリケーションのエントリ ポイント

#include "stdafx.h"
#include "binary_edit_win32.h"

namespace
{
	constexpr int kMaxLoadString = 100;

	HINSTANCE	app_instance;							// 現在のインスタンス
	TCHAR		title[kMaxLoadString];			// タイトル バーのテキスト
	TCHAR		window_class[kMaxLoadString];	// メイン ウィンドウ クラス名

	ATOM				RegisterMainWindowClass(HINSTANCE instance);
	BOOL				InitInstance(HINSTANCE instance, int cmd_show);
	LRESULT CALLBACK	WndProc(HWND wnd, UINT message, WPARAM w_param, LPARAM l_param);
	INT_PTR CALLBACK	AboutDlgProc(HWND dlg, UINT message, WPARAM w_param, LPARAM l_param);
}

int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE /*prev_instance*/, LPTSTR /*cmd_line*/, int cmd_show)
{
	LoadString(instance, IDS_APP_TITLE, title, kMaxLoadString);
	LoadString(instance, IDC_BINARYEDIT_WIN32, window_class, kMaxLoadString);
	RegisterMainWindowClass(instance);

	if (!InitInstance(instance, cmd_show))
	{
		return FALSE;
	}

	HACCEL accel_table = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_BINARYEDIT_WIN32));

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

namespace
{

ATOM RegisterMainWindowClass(HINSTANCE instance)
{
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instance;
	wcex.hIcon			= LoadIcon(instance, MAKEINTRESOURCE(IDI_BINARYEDIT_WIN32));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BINARYEDIT_WIN32);
	wcex.lpszClassName	= window_class;
	wcex.hIconSm		= LoadIcon(instance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE instance, int cmd_show)
{
	app_instance = instance;

	HWND wnd = CreateWindow(window_class, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instance, nullptr);
	if (!wnd)
	{
		return FALSE;
	}

	ShowWindow(wnd, cmd_show);
	UpdateWindow(wnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(w_param))
		{
		case IDM_ABOUT:
			DialogBox(app_instance, MAKEINTRESOURCE(IDD_ABOUTBOX), wnd, AboutDlgProc);
			break;
		case IDM_EXIT:
			DestroyWindow(wnd);
			break;
		default:
			return DefWindowProc(wnd, message, w_param, l_param);
		}
		break;
	case WM_PAINT:
		{
			// 描画は未実装。BeginPaint/EndPaint で無効領域だけ検証する
			PAINTSTRUCT ps;
			BeginPaint(wnd, &ps);
			EndPaint(wnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(wnd, message, w_param, l_param);
	}
	return 0;
}

INT_PTR CALLBACK AboutDlgProc(HWND dlg, UINT message, WPARAM w_param, LPARAM /*l_param*/)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(w_param) == IDOK || LOWORD(w_param) == IDCANCEL)
		{
			EndDialog(dlg, LOWORD(w_param));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

}
