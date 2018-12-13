
// EnumModuleDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "EnumModule.h"
#include "EnumModuleDlg.h"
#include "afxdialogex.h"

#include <tchar.h>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEnumModuleDlg �_�C�A���O
CEnumModuleDlg::CEnumModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnumModuleDlg::IDD, pParent)
	, m_ProcessName(_T(""))
	, m_Result(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnumModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_PROCESS_NAME, m_ProcessName);
	DDX_Text(pDX, IDET_RESULT, m_Result);
}

BEGIN_MESSAGE_MAP(CEnumModuleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_GET_MODULENAME, &CEnumModuleDlg::OnBnClickedGetModulename)
END_MESSAGE_MAP()


// CEnumModuleDlg ���b�Z�[�W �n���h���[

BOOL CEnumModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CEnumModuleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CEnumModuleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEnumModuleDlg::OnBnClickedGetModulename()
{
	UpdateData(TRUE);

	DWORD ProcessID;
	if ( m_ProcessName == "")
	{
		ProcessID = GetCurrentProcessId();
	}
	else
	{
		// TODO�F�v���Z�X������PID���擾
		//ProcessID = GetCurrentProcessId();
		MessageBox("�v���Z�X����C�ӂɎw�肷��@�\�͔�T�|�[�g");
		return;
	}
	m_Result = PrintModuleName(ProcessID);
	UpdateData(FALSE);
}

CString PrintModuleName( DWORD processID )
{
	CString Result = "";

	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	TRACE( _T("\nProcess ID: %u\n"), processID );
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ, FALSE, processID);
	if (NULL == hProcess)
	{
		return Result;
	}

	// �w�肳�ꂽ�v���Z�X���̊e���W���[���̃n���h�����擾
	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR szModName[MAX_PATH];
			// �w�肳�ꂽ���W���[���̊��S�C���p�X�����擾
			if ( GetModuleFileNameEx( hProcess, hMods[i], szModName, sizeof(szModName)))
			{
				CString AddString;
				AddString.Format("[%2d]\r\n", i );
				Result += AddString;

				AddString.Format(_T("%s (0x%p)\r\n"), szModName, hMods[i] );
				Result += AddString;

				TCHAR szRealFileName[MAX_PATH];
				if (GetPhysicalFileName(szModName, szRealFileName, MAX_PATH) != FALSE)
				{
					AddString.Format(_T("   => %s\r\n"), szRealFileName);
					Result += AddString;
				}
				Result += "\r\n";
			}
		}
	}
	CloseHandle( hProcess );

	return Result;
}

BOOL GetPhysicalFileName(LPCTSTR szFileName, LPTSTR szRealFileName, SIZE_T nBufSize)
{
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi); 

	if( dwFileSizeLo == 0 && dwFileSizeHi == 0 )
	{
		CloseHandle(hFile);
		SetLastError(ERROR_EMPTY);
		return FALSE;
	}

	// �w�肳�ꂽ�t�@�C���ɑ΂���A���O�t���܂��͖��O�Ȃ��̃t�@�C���}�b�s���O�I�u�W�F�N�g���쐬
	HANDLE hFileMap;
	hFileMap = CreateFileMapping(hFile, 
		NULL, 
		PAGE_READONLY,
		0, 
		1,
		NULL);

	if (hFileMap == NULL)
	{
		DWORD gle = GetLastError();
		CloseHandle(hFile);
		SetLastError(gle);
		return FALSE;
	}

	// �Ăяo�����v���Z�X�̃A�h���X��ԂɁA�t�@�C���̃r���[���}�b�s���O
	void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
	if (pMem == NULL)
	{
		DWORD gle = GetLastError();
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		SetLastError(gle);
		return FALSE;
	}

	// �w�肳�ꂽ�A�h���X���A�w�肳�ꂽ�v���Z�X�̃A�h���X��ԓ��̃������}�b�v�g�t�@�C���ɑ��݂��邩�ǂ������`�F�b�N
	TCHAR pszFilename[MAX_PATH+1];
	BOOL bSuccess = FALSE;
	if (GetMappedFileName (GetCurrentProcess(), pMem, pszFilename,MAX_PATH)) 
	{
		TCHAR szTemp[MAX_PATH+1];
		szTemp[0] = '\0';

		// �V�X�e���ŗ��p�\�ȃh���C�u��\����������o�b�t�@�Ɋi�[
		if (GetLogicalDriveStrings(MAX_PATH, szTemp)) 
		{
			TCHAR szName[MAX_PATH];
			TCHAR szDrive[3] = " :";
			BOOL bFound = FALSE;
			TCHAR* p = szTemp;

			do 
			{
				*szDrive = *p;

				// �A�v���P�[�V������ MS-DOS �f�o�C�X���Ɋւ�������擾
				if (QueryDosDevice(szDrive, szName, MAX_PATH))
				{
					size_t uNameLen = _tcslen(szName);

					if (uNameLen < MAX_PATH) 
					{
						bFound = _tcsnicmp(pszFilename, szName, 
						uNameLen) == 0;

						if (bFound) 
						{
							TCHAR szTempFile[MAX_PATH];
							_stprintf_s(szTempFile,
								"%s%s",
								szDrive,
								pszFilename+uNameLen);
								_tcsncpy_s(pszFilename, szTempFile, MAX_PATH);
							bSuccess = TRUE;
						}
					}
				}

				// NULL�܂ŉ�
				while (*p++);
			} while (!bFound && *p); // ������̍Ō�
		}
	}

	// �㏈��
	UnmapViewOfFile(pMem);
	CloseHandle(hFileMap);
	CloseHandle(hFile);

	if (bSuccess == FALSE)
	{
		SetLastError(ERROR_FILE_NOT_FOUND);
	}
	else
	{
		_tcscpy_s(szRealFileName, nBufSize, pszFilename);
		SetLastError(ERROR_SUCCESS);
	}

	return(bSuccess);
}
