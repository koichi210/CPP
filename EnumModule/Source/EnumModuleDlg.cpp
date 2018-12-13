
// EnumModuleDlg.cpp : 実装ファイル
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


// CEnumModuleDlg ダイアログ
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


// CEnumModuleDlg メッセージ ハンドラー

BOOL CEnumModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CEnumModuleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
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
		// TODO：プロセス名からPIDを取得
		//ProcessID = GetCurrentProcessId();
		MessageBox("プロセス名を任意に指定する機能は非サポート");
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

	// 指定されたプロセス内の各モジュールのハンドルを取得
	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR szModName[MAX_PATH];
			// 指定されたモジュールの完全修飾パス名を取得
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

	// 指定されたファイルに対する、名前付きまたは名前なしのファイルマッピングオブジェクトを作成
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

	// 呼び出し側プロセスのアドレス空間に、ファイルのビューをマッピング
	void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
	if (pMem == NULL)
	{
		DWORD gle = GetLastError();
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		SetLastError(gle);
		return FALSE;
	}

	// 指定されたアドレスが、指定されたプロセスのアドレス空間内のメモリマップトファイルに存在するかどうかをチェック
	TCHAR pszFilename[MAX_PATH+1];
	BOOL bSuccess = FALSE;
	if (GetMappedFileName (GetCurrentProcess(), pMem, pszFilename,MAX_PATH)) 
	{
		TCHAR szTemp[MAX_PATH+1];
		szTemp[0] = '\0';

		// システムで利用可能なドライブを表す文字列をバッファに格納
		if (GetLogicalDriveStrings(MAX_PATH, szTemp)) 
		{
			TCHAR szName[MAX_PATH];
			TCHAR szDrive[3] = " :";
			BOOL bFound = FALSE;
			TCHAR* p = szTemp;

			do 
			{
				*szDrive = *p;

				// アプリケーションで MS-DOS デバイス名に関する情報を取得
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

				// NULLまで回す
				while (*p++);
			} while (!bFound && *p); // 文字列の最後
		}
	}

	// 後処理
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
