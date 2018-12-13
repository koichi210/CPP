
// Util.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"


// 作業時にWorkとして生成するテンポラリファイルのプリフィックス
#define TEMP_FILE_PRE_FIX		"DCS"


class CMyComboBox : public CComboBox
{
public :
	CString GetWindowText();
};


// 非スレッド実行
UINT ExecuteCommand(LPVOID pParam, CString OutFileName = "");

// スレッド実行
UINT ExecuteProcess(LPVOID pParam);

void UpdateComboBox(CMyComboBox *CtrlCB);

// Ini取得
CString GetIniFileParam( CString FileName, CString SectionName, CString KeyName, CString DefaultKeyName = "");
CString GetIniFileParam( CString FileName, CString SectionName, CString KeyName, int LoopMax, CString DefaultKeyName = "");
void GetIniFileParam( CString FileName, CString SectionName, CString KeyName, int LoopMax, CMyComboBox *CtrlCB );

// Ini設定
void SetIniFileParam( CString FileName, CString SectionName, CString KeyName, CString Value);
void SetIniFileParam( CString FileName, CString SectionName, CString KeyName, int Value);
void SetIniFileParams( CString FileName, CString SectionName, CString KeyName, CString Value);
void SetIniFileParams( CString FileName, CString SectionName, CString KeyName, CMyComboBox *CtrlCB);

// 文字列操作
CString AppendString(CString Path, int Value);
CString Append(CString Param1, CString Param2);
CString GetDriveName(CString FullPath);
CString GetDirectoryPath(CString FullPath);
CString GetFileName(CString FullPath);

