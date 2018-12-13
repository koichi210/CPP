
// Util.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"


// ��Ǝ���Work�Ƃ��Đ�������e���|�����t�@�C���̃v���t�B�b�N�X
#define TEMP_FILE_PRE_FIX		"DCS"


class CMyComboBox : public CComboBox
{
public :
	CString GetWindowText();
};


// ��X���b�h���s
UINT ExecuteCommand(LPVOID pParam, CString OutFileName = "");

// �X���b�h���s
UINT ExecuteProcess(LPVOID pParam);

void UpdateComboBox(CMyComboBox *CtrlCB);

// Ini�擾
CString GetIniFileParam( CString FileName, CString SectionName, CString KeyName, CString DefaultKeyName = "");
CString GetIniFileParam( CString FileName, CString SectionName, CString KeyName, int LoopMax, CString DefaultKeyName = "");
void GetIniFileParam( CString FileName, CString SectionName, CString KeyName, int LoopMax, CMyComboBox *CtrlCB );

// Ini�ݒ�
void SetIniFileParam( CString FileName, CString SectionName, CString KeyName, CString Value);
void SetIniFileParam( CString FileName, CString SectionName, CString KeyName, int Value);
void SetIniFileParams( CString FileName, CString SectionName, CString KeyName, CString Value);
void SetIniFileParams( CString FileName, CString SectionName, CString KeyName, CMyComboBox *CtrlCB);

// �����񑀍�
CString AppendString(CString Path, int Value);
CString Append(CString Param1, CString Param2);
CString GetDriveName(CString FullPath);
CString GetDirectoryPath(CString FullPath);
CString GetFileName(CString FullPath);

