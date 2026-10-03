// util.h : INI 形式の設定ファイルの読み書き

#pragma once

CString GetIniFileParam(LPCTSTR file_name, LPCTSTR section_name, LPCTSTR key_name, LPCTSTR default_value = _T(""));

void SetIniFileParam(LPCTSTR file_name, LPCTSTR section_name, LPCTSTR key_name, LPCTSTR value);
void SetIniFileParam(LPCTSTR file_name, LPCTSTR section_name, LPCTSTR key_name, int value);
