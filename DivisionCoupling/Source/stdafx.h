// stdafx.h : プリコンパイル済みヘッダー
//            標準のシステムインクルードと、参照が多く変更の少ないヘッダーを記述する

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されない部分を除外
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクターは明示的
#define _AFX_ALL_WARNINGS					// 無視しても安全な MFC の警告も表示する

#include <afxwin.h>			// MFC のコアおよび標準コンポーネント
#include <afxext.h>			// MFC の拡張部分
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#include <afxcontrolbars.h>	// MFC におけるリボンとコントロール バーのサポート
#include <afxdialogex.h>	// CDialogEx

#include <shlwapi.h>		// PathFileExists
#pragma comment(lib, "shlwapi.lib")
