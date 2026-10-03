// menu.cc : アプリケーションのエントリ ポイント
//            「機能」メニューから「テスト領域」メニューへ項目を追加・削除して挙動を確かめる

#include "stdafx.h"
#include "menu.h"

namespace
{
	constexpr int kMaxLoadString = 100;

	// メニューバー上の「テスト領域」の位置と、その中のサブメニュー付き項目の位置
	constexpr int kTestAreaPosition = 2;
	constexpr int kTestSubMenuPosition = 1;

	// 実行時に追加する項目のコマンド ID（ハンドラは無い）
	constexpr UINT_PTR kAddedItemId = 1;

	HINSTANCE inst = nullptr;
	TCHAR title[kMaxLoadString];
	TCHAR window_class[kMaxLoadString];

	HMENU GetTestAreaMenu(HWND wnd)
	{
		return GetSubMenu(GetMenu(wnd), kTestAreaPosition);
	}

	HMENU GetTestSubMenu(HWND wnd)
	{
		return GetSubMenu(GetTestAreaMenu(wnd), kTestSubMenuPosition);
	}

	// 末尾に項目を追加する
	void AppendTestItem(HMENU menu, LPCTSTR text)
	{
		AppendMenu(menu, MF_BYPOSITION, kAddedItemId, text);
	}

	// 先頭の項目を削除する
	void DeleteFirstItem(HMENU menu)
	{
		DeleteMenu(menu, 0, MF_BYPOSITION);
	}

	INT_PTR CALLBACK About(HWND dlg, UINT message, WPARAM w_param, LPARAM l_param)
	{
		UNREFERENCED_PARAMETER(l_param);
		switch (message)
		{
		case WM_INITDIALOG:
			return static_cast<INT_PTR>(TRUE);

		case WM_COMMAND:
			if (LOWORD(w_param) == IDOK || LOWORD(w_param) == IDCANCEL)
			{
				EndDialog(dlg, LOWORD(w_param));
				return static_cast<INT_PTR>(TRUE);
			}
			break;
		}
		return static_cast<INT_PTR>(FALSE);
	}

	LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM w_param, LPARAM l_param)
	{
		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(w_param))
			{
			case IDM_ADD_MENU:
				AppendTestItem(GetTestAreaMenu(wnd), _T("Create Menu!!"));
				break;
			case IDM_DEL_MENU:
				DeleteFirstItem(GetTestAreaMenu(wnd));
				break;
			case IDM_ADD_SUB_MENU:
				AppendTestItem(GetTestSubMenu(wnd), _T("Create SubMenu!!"));
				break;
			case IDM_DEL_SUB_MENU:
				DeleteFirstItem(GetTestSubMenu(wnd));
				break;
			case IDM_ABOUT:
				DialogBox(inst, MAKEINTRESOURCE(IDD_ABOUTBOX), wnd, About);
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
			PAINTSTRUCT ps;
			BeginPaint(wnd, &ps);
			EndPaint(wnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(wnd, message, w_param, l_param);
		}
		return 0;
	}

	ATOM MyRegisterClass(HINSTANCE instance)
	{
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = instance;
		wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_MENU));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
		wcex.lpszClassName = window_class;
		wcex.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassEx(&wcex);
	}

	BOOL InitInstance(HINSTANCE instance, int cmd_show)
	{
		inst = instance;

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
}

int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPTSTR cmd_line, int cmd_show)
{
	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(cmd_line);

	LoadString(instance, IDS_APP_TITLE, title, kMaxLoadString);
	LoadString(instance, IDC_MENU, window_class, kMaxLoadString);
	MyRegisterClass(instance);

	if (!InitInstance(instance, cmd_show))
	{
		return FALSE;
	}

	HACCEL accel_table = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_MENU));

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
