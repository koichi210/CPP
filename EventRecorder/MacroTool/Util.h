// Util.h : INI 形式の設定ファイルの読み書き

#pragma once

CString GetIniFileParam(LPCTSTR fileName, LPCTSTR sectionName, LPCTSTR keyName, LPCTSTR defaultValue = _T(""));

void SetIniFileParam(LPCTSTR fileName, LPCTSTR sectionName, LPCTSTR keyName, LPCTSTR value);
void SetIniFileParam(LPCTSTR fileName, LPCTSTR sectionName, LPCTSTR keyName, int value);
