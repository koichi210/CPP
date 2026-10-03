// util.cc : INI 形式の設定ファイルの読み書き

#include "stdafx.h"
#include "util.h"

CString GetIniFileParam(LPCTSTR fileName, LPCTSTR sectionName, LPCTSTR keyName, LPCTSTR defaultValue)
{
	TCHAR value[MAX_PATH];
	GetPrivateProfileString(sectionName, keyName, defaultValue, value, MAX_PATH, fileName);
	return CString(value);
}

void SetIniFileParam(LPCTSTR fileName, LPCTSTR sectionName, LPCTSTR keyName, LPCTSTR value)
{
	WritePrivateProfileString(sectionName, keyName, value, fileName);
}

void SetIniFileParam(LPCTSTR fileName, LPCTSTR sectionName, LPCTSTR keyName, int value)
{
	CString text;
	text.Format(_T("%d"), value);
	WritePrivateProfileString(sectionName, keyName, text, fileName);
}
