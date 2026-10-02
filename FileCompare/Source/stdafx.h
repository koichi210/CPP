// stdafx.h : プリコンパイル済みヘッダー
//            標準のシステムインクルードと、参照が多く変更の少ないヘッダーを記述する

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されない部分を除外
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクターを explicit にする

// 無視しても安全な MFC の警告も表示する
#define _AFX_ALL_WARNINGS

#include <afxwin.h>			// MFC のコアおよび標準コンポーネント
#include <afxext.h>			// MFC の拡張部分
#include <afxdisp.h>		// MFC オートメーション クラス

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif

#include <afxcontrolbars.h>	// CDialogEx
