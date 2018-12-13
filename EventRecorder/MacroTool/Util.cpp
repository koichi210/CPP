
// Util.cpp : 実装ファイル
//
#include "stdafx.h"
#include "Util.h"


CString CMyComboBox::GetWindowText()
{
	CString str;
	CComboBox::GetWindowText(str);

	return str;
}

CString CreateTempFile()
{
	// テンポラリファイルを生成
	char TempDir[MAX_PATH];
	char TempName[MAX_PATH];
	CString TempPathName;

	GetTempPath( sizeof(TempDir), TempDir);
	GetTempFileName( TempDir, TEMP_FILE_PRE_FIX, 0, TempName );
	TempPathName = TempName;
	TempPathName.Replace(".tmp", ".bat");

	CStdioFile	file;
	if ( ! file.Open(TempPathName, CFile::modeRead | CFile::typeText) )
	{
		file.Open(TempPathName, CFile::modeCreate);
	}
	file.Close();

	return TempPathName;
}

void SetExecuteParam(CString FileName, CString Param)
{
	CStdioFile	file;
	if ( file.Open(FileName, CFile::modeWrite | CFile::typeText) )
	{
		file.WriteString(Param);
		file.Close();
	}
}

UINT ExecuteCommand(LPVOID pParam, CString OutFileName)
{
	CString* Param = (CString*)pParam;

	CString FileName = CreateTempFile();
	SetExecuteParam(FileName, *Param);

	CString Command = FileName;
	if ( ! OutFileName.IsEmpty() )
	{
		Command += " > " + OutFileName;
	}
	system(Command);

	return TRUE;
}

UINT ExecuteProcess(LPVOID pParam)
{
	CString* Command = (CString*)pParam;

	CString FileName = CreateTempFile();
	SetExecuteParam(FileName, *Command);
	system(FileName);

	return TRUE;
}

void UpdateComboBox(CMyComboBox *CtrlCB)
{
	for (int i = 0; i < CtrlCB->GetCount(); i++)
	{
		CString Text;
		CtrlCB->GetLBText(i, Text);
		if (CtrlCB->GetWindowText() == Text)
		{
			return;
		}
	}

	// コンボボックスにないとfor文を抜ける
	CtrlCB->AddString(CtrlCB->GetWindowText());
	CtrlCB->SetCurSel(CtrlCB->GetCount() - 1);
}

// Ini取得 //////////////////////////////////////////////////////////////////
CString GetIniFileParam( CString FileName, CString SectionName, CString KeyName, CString DefaultKeyName)
{
	char Value[MAX_PATH];
	GetPrivateProfileString(SectionName, KeyName, DefaultKeyName, Value, sizeof(Value), FileName);

	return Value;
}

CString GetIniFileParam( CString FileName, CString SectionName, CString KeyName, int LoopMax, CString DefaultKeyName)
{
	CString Result;
	CString Add;
	for(int i = 0; i < LoopMax; i++)
	{
		Add	= GetIniFileParam(FileName, SectionName, AppendString(KeyName, i), "" );
		if ( Add.IsEmpty() )
		{
			continue;
		}
		Result += Add + "\r\n";
	}
	return Result;
}

void GetIniFileParam( CString FileName, CString SectionName, CString KeyName, int LoopMax, CMyComboBox *CtrlCB )
{
	CtrlCB->ResetContent();
	for(int i = 0; i < LoopMax; i++)
	{
		CString Add	= GetIniFileParam(FileName, SectionName, AppendString(KeyName, i), "" );
		if ( Add.IsEmpty() )
		{
			continue;
		}
		CtrlCB->AddString(Add);
	}
}

// Ini設定 //////////////////////////////////////////////////////////////////
void SetIniFileParam( CString FileName, CString SectionName, CString KeyName, CString Value)
{
	WritePrivateProfileString(SectionName, KeyName, Value, FileName);
}

void SetIniFileParam(CString FileName, CString SectionName, CString KeyName, int Value)
{
	CString ValString;
	ValString.Format("%d", Value);
	WritePrivateProfileString(SectionName, KeyName, ValString, FileName);
}

void SetIniFileParams( CString FileName, CString SectionName, CString KeyName, CString Value)
{
	int cnt = 0;
	CString Key = "";
	int curPos = 0;

	CAtlString resToken = Value.Tokenize("\r\n", curPos);
	while (resToken != _T(""))
	{
		Key.Format("%s[%d]", KeyName, cnt);
		SetIniFileParam(FileName, SectionName, Key, resToken);	// TODO:不要な改行が入っている
		resToken = Value.Tokenize(_T("\r\n"), curPos);
		cnt++;
	}
}

void SetIniFileParams( CString FileName, CString SectionName, CString KeyName, CMyComboBox *CtrlCB)
{
	CString Key = "";
	CString Value = "";
	for ( int i = 0; i < CtrlCB->GetCount(); i++)
	{
		Key.Format("%s[%d]", KeyName, i);
		CtrlCB->GetLBText(i, Value);
		SetIniFileParam(FileName, SectionName, Key, Value);
	}
}

// 文字列操作 ////////////////////////////////////////////////////////////////////
CString AppendString(CString Path, int Value)
{
	CString Append;
	Append.Format("%s[%d]", Path, Value);
	return Append;
}

CString Append(CString Param1, CString Param2)
{
	return Param1 + Param2;
}

CString GetDriveName(CString FullPath)
{
	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FileName[MAX_PATH];
	char Ext[MAX_PATH];
	_splitpath(FullPath, Drive, Dir, FileName, Ext);

	return Drive;
}

CString GetDirectoryPath(CString FullPath)
{
	CString AddString;
	TCHAR path[ MAX_PATH ] = {0};

	strcpy(path, FullPath);
	PathRemoveFileSpec(path);
	return path;
}

CString GetFileName(CString FullPath)
{
	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FileName[MAX_PATH];
	char Ext[MAX_PATH];
	_splitpath(FullPath, Drive, Dir, FileName, Ext);

	return Append(FileName, Ext);
}
