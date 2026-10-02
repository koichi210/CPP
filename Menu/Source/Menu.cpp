// Menu.cpp : アプリケーションのエントリ ポイント
//            「機能」メニューから「テスト領域」メニューへ項目を追加・削除して挙動を確かめる

#include "stdafx.h"
#include "Menu.h"

namespace
{
	constexpr int kMaxLoadString = 100;

	// メニューバー上の「テスト領域」の位置と、その中のサブメニュー付き項目の位置
	constexpr int kTestAreaPosition = 2;
	constexpr int kTestSubMenuPosition = 1;

	// 実行時に追加する項目のコマンド ID（ハンドラは無い）
	constexpr UINT_PTR kAddedItemId = 1;

	HINSTANCE g_hInst = nullptr;
	TCHAR g_szTitle[kMaxLoadString];
	TCHAR g_szWindowClass[kMaxLoadString];

	HMENU GetTestAreaMenu(HWND hWnd)
	{
		return GetSubMenu(GetMenu(hWnd), kTestAreaPosition);
	}

	HMENU GetTestSubMenu(HWND hWnd)
	{
		return GetSubMenu(GetTestAreaMenu(hWnd), kTestSubMenuPosition);
	}

	// 末尾に項目を追加する
	void AppendTestItem(HMENU hMenu, LPCTSTR text)
	{
		AppendMenu(hMenu, MF_BYPOSITION, kAddedItemId, text);
	}

	// 先頭の項目を削除する
	void DeleteFirstItem(HMENU hMenu)
	{
		DeleteMenu(hMenu, 0, MF_BYPOSITION);
	}

	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return static_cast<INT_PTR>(TRUE);

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return static_cast<INT_PTR>(TRUE);
			}
			break;
		}
		return static_cast<INT_PTR>(FALSE);
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDM_ADD_MENU:
				AppendTestItem(GetTestAreaMenu(hWnd), _T("Create Menu!!"));
				break;
			case IDM_DEL_MENU:
				DeleteFirstItem(GetTestAreaMenu(hWnd));
				break;
			case IDM_ADD_SUB_MENU:
				AppendTestItem(GetTestSubMenu(hWnd), _T("Create SubMenu!!"));
				break;
			case IDM_DEL_SUB_MENU:
				DeleteFirstItem(GetTestSubMenu(hWnd));
				break;
			case IDM_ABOUT:
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	ATOM MyRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MENU));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
		wcex.lpszClassName = g_szWindowClass;
		wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassEx(&wcex);
	}

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
		g_hInst = hInstance;

		HWND hWnd = CreateWindow(g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
		if (!hWnd)
		{
			return FALSE;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		return TRUE;
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, g_szTitle, kMaxLoadString);
	LoadString(hInstance, IDC_MENU, g_szWindowClass, kMaxLoadString);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU));

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}
