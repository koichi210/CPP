// util.cc : INI 形式の設定ファイルの読み書き

#include "stdafx.h"
#include "util.h"

CString GetIniFileParam(LPCTSTR file_name, LPCTSTR section_name, LPCTSTR key_name, LPCTSTR default_value)
{
	TCHAR value[MAX_PATH];
	GetPrivateProfileString(section_name, key_name, default_value, value, MAX_PATH, file_name);
	return CString(value);
}

void SetIniFileParam(LPCTSTR file_name, LPCTSTR section_name, LPCTSTR key_name, LPCTSTR value)
{
	WritePrivateProfileString(section_name, key_name, value, file_name);
}

void SetIniFileParam(LPCTSTR file_name, LPCTSTR section_name, LPCTSTR key_name, int value)
{
	CString text;
	text.Format(_T("%d"), value);
	WritePrivateProfileString(section_name, key_name, text, file_name);
}
