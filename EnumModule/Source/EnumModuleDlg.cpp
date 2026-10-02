// EnumModuleDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "EnumModule.h"
#include "EnumModuleDlg.h"
#include "afxdialogex.h"

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// GetMappedFileName が返すデバイスパス（\Device\HarddiskVolumeN\...）を
	// ドライブ文字のパス（C:\...）に戻す
	bool DevicePathToDosPath(LPCTSTR devicePath, CString& dosPath)
	{
		TCHAR drives[MAX_PATH + 1] = {};
		if (GetLogicalDriveStrings(MAX_PATH, drives) == 0)
		{
			return false;
		}

		// "A:\<NUL>C:\<NUL>...<NUL><NUL>" の形式で並んでいる
		for (LPCTSTR p = drives; *p != _T('\0'); p += _tcslen(p) + 1)
		{
			const TCHAR drive[3] = { p[0], _T(':'), _T('\0') };
			TCHAR deviceName[MAX_PATH] = {};
			if (QueryDosDevice(drive, deviceName, MAX_PATH) == 0)
			{
				continue;
			}

			const size_t nameLen = _tcslen(deviceName);
			if (_tcsnicmp(devicePath, deviceName, nameLen) == 0)
			{
				dosPath = CString(drive) + (devicePath + nameLen);
				return true;
			}
		}
		return false;
	}

	// ファイルをメモリマップし、マップされた実体のファイル名を得る
	bool GetPhysicalFileName(LPCTSTR fileName, CString& realFileName)
	{
		HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			nullptr, OPEN_EXISTING, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		// サイズ 0 のファイルはマッピングできない
		DWORD sizeHigh = 0;
		const DWORD sizeLow = GetFileSize(hFile, &sizeHigh);
		if (sizeLow == 0 && sizeHigh == 0)
		{
			CloseHandle(hFile);
			return false;
		}

		HANDLE hFileMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 1, nullptr);
		if (hFileMap == nullptr)
		{
			CloseHandle(hFile);
			return false;
		}

		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
		if (pMem == nullptr)
		{
			CloseHandle(hFileMap);
			CloseHandle(hFile);
			return false;
		}

		bool succeeded = false;
		TCHAR mappedName[MAX_PATH + 1] = {};
		if (GetMappedFileName(GetCurrentProcess(), pMem, mappedName, MAX_PATH))
		{
			succeeded = DevicePathToDosPath(mappedName, realFileName);
		}

		UnmapViewOfFile(pMem);
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return succeeded;
	}

	// 指定プロセスが読み込んでいるモジュールの一覧を文字列にする
	CString ListModuleNames(DWORD processId)
	{
		CString result;

		TRACE(_T("\nProcess ID: %u\n"), processId);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
		if (hProcess == nullptr)
		{
			return result;
		}

		HMODULE hMods[1024];
		DWORD cbNeeded = 0;
		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			// モジュール数が配列より多いと cbNeeded は配列サイズを超えるので、配列の範囲で打ち切る
			if (cbNeeded > sizeof(hMods))
			{
				cbNeeded = sizeof(hMods);
			}

			const DWORD count = cbNeeded / sizeof(HMODULE);
			for (DWORD i = 0; i < count; i++)
			{
				TCHAR modName[MAX_PATH];
				if (GetModuleFileNameEx(hProcess, hMods[i], modName, _countof(modName)) == 0)
				{
					continue;
				}

				CString line;
				line.Format(_T("[%2u]\r\n%s (0x%p)\r\n"), i, modName, hMods[i]);
				result += line;

				CString realName;
				if (GetPhysicalFileName(modName, realName))
				{
					line.Format(_T("   => %s\r\n"), static_cast<LPCTSTR>(realName));
					result += line;
				}
				result += _T("\r\n");
			}
		}
		CloseHandle(hProcess);

		return result;
	}
}

CEnumModuleDlg::CEnumModuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CEnumModuleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnumModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_PROCESS_NAME, m_strProcessName);
	DDX_Text(pDX, IDET_RESULT, m_strResult);
}

BEGIN_MESSAGE_MAP(CEnumModuleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_GET_MODULENAME, &CEnumModuleDlg::OnBnClickedGetModulename)
END_MESSAGE_MAP()

BOOL CEnumModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void CEnumModuleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CEnumModuleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEnumModuleDlg::OnBnClickedGetModulename()
{
	UpdateData(TRUE);

	if (!m_strProcessName.IsEmpty())
	{
		// プロセス名から PID を引く機能は未実装
		MessageBox(_T("プロセス名を任意に指定する機能は非サポート"));
		return;
	}

	m_strResult = ListModuleNames(GetCurrentProcessId());
	UpdateData(FALSE);
}
