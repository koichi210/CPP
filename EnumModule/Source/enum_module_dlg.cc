// enum_module_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "enum_module.h"
#include "enum_module_dlg.h"
#include "afxdialogex.h"

#include <psapi.h>
#include <vector>
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// ドライブ文字（"C:"）と、それに対応するデバイス名（"\Device\HarddiskVolume3"）の組
	struct DriveDevice
	{
		CString drive;
		CString device_name;
	};

	// 論理ドライブとデバイス名の対応表を作る。モジュールごとに引き直さないよう、一度だけ作って使い回す
	std::vector<DriveDevice> GetDriveDevices()
	{
		std::vector<DriveDevice> drive_devices;

		TCHAR drives[MAX_PATH + 1] = {};
		if (GetLogicalDriveStrings(MAX_PATH, drives) == 0)
		{
			return drive_devices;
		}

		// "A:\<NUL>C:\<NUL>...<NUL><NUL>" の形式で並んでいる
		for (LPCTSTR p = drives; *p != _T('\0'); p += _tcslen(p) + 1)
		{
			const TCHAR drive[3] = { p[0], _T(':'), _T('\0') };
			TCHAR device_name[MAX_PATH] = {};
			if (QueryDosDevice(drive, device_name, MAX_PATH) != 0)
			{
				drive_devices.push_back({ drive, device_name });
			}
		}
		return drive_devices;
	}

	// GetMappedFileName が返すデバイスパス（\Device\HarddiskVolumeN\...）を
	// ドライブ文字のパス（C:\...）に戻す
	bool DevicePathToDosPath(LPCTSTR device_path, const std::vector<DriveDevice>& drive_devices, CString& dos_path)
	{
		for (const DriveDevice& drive_device : drive_devices)
		{
			const int name_len = drive_device.device_name.GetLength();
			// \Device\HarddiskVolume1 が \Device\HarddiskVolume10 に前方一致しないよう、
			// デバイス名の直後が '\' または文字列の終端のときだけ一致とする
			if (_tcsnicmp(device_path, drive_device.device_name, name_len) == 0 &&
				(device_path[name_len] == _T('\\') || device_path[name_len] == _T('\0')))
			{
				dos_path = drive_device.drive + (device_path + name_len);
				return true;
			}
		}
		return false;
	}

	// ファイルをメモリマップし、マップされた実体のファイル名を得る
	bool GetPhysicalFileName(LPCTSTR file_name, const std::vector<DriveDevice>& drive_devices, CString& real_file_name)
	{
		HANDLE file = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			nullptr, OPEN_EXISTING, 0, nullptr);
		if (file == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		// サイズ 0 のファイルはマッピングできない
		DWORD size_high = 0;
		const DWORD size_low = GetFileSize(file, &size_high);
		if (size_low == 0 && size_high == 0)
		{
			CloseHandle(file);
			return false;
		}

		HANDLE file_map = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 1, nullptr);
		if (file_map == nullptr)
		{
			CloseHandle(file);
			return false;
		}

		void* mem = MapViewOfFile(file_map, FILE_MAP_READ, 0, 0, 1);
		if (mem == nullptr)
		{
			CloseHandle(file_map);
			CloseHandle(file);
			return false;
		}

		bool succeeded = false;
		TCHAR mapped_name[MAX_PATH + 1] = {};
		if (GetMappedFileName(GetCurrentProcess(), mem, mapped_name, MAX_PATH))
		{
			succeeded = DevicePathToDosPath(mapped_name, drive_devices, real_file_name);
		}

		UnmapViewOfFile(mem);
		CloseHandle(file_map);
		CloseHandle(file);
		return succeeded;
	}

	// 指定プロセスが読み込んでいるモジュールの一覧を文字列にする
	CString ListModuleNames(DWORD process_id)
	{
		CString result;

		TRACE(_T("\nProcess ID: %u\n"), process_id);
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
		if (process == nullptr)
		{
			return result;
		}

		HMODULE mods[1024];
		DWORD bytes_needed = 0;
		if (EnumProcessModules(process, mods, sizeof(mods), &bytes_needed))
		{
			// モジュール数が配列より多いと bytes_needed は配列サイズを超えるので、配列の範囲で打ち切る
			if (bytes_needed > sizeof(mods))
			{
				bytes_needed = sizeof(mods);
			}

			const DWORD count = bytes_needed / sizeof(HMODULE);
			const std::vector<DriveDevice> drive_devices = GetDriveDevices();
			for (DWORD i = 0; i < count; i++)
			{
				TCHAR mod_name[MAX_PATH];
				if (GetModuleFileNameEx(process, mods[i], mod_name, _countof(mod_name)) == 0)
				{
					continue;
				}

				CString line;
				line.Format(_T("[%2u]\r\n%s (0x%p)\r\n"), i, mod_name, mods[i]);
				result += line;

				CString real_name;
				if (GetPhysicalFileName(mod_name, drive_devices, real_name))
				{
					line.Format(_T("   => %s\r\n"), static_cast<LPCTSTR>(real_name));
					result += line;
				}
				result += _T("\r\n");
			}
		}
		CloseHandle(process);

		return result;
	}
}

EnumModuleDlg::EnumModuleDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(EnumModuleDlg::IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void EnumModuleDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Text(dx, IDET_PROCESS_NAME, process_name_);
	DDX_Text(dx, IDET_RESULT, result_);
}

BEGIN_MESSAGE_MAP(EnumModuleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_GET_MODULENAME, &EnumModuleDlg::OnBnClickedGetModulename)
END_MESSAGE_MAP()

BOOL EnumModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void EnumModuleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - icon_width + 1) / 2;
		const int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR EnumModuleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void EnumModuleDlg::OnBnClickedGetModulename()
{
	UpdateData(TRUE);

	if (!process_name_.IsEmpty())
	{
		// プロセス名から PID を引く機能は未実装
		MessageBox(_T("プロセス名を任意に指定する機能は非サポート"));
		return;
	}

	result_ = ListModuleNames(GetCurrentProcessId());
	UpdateData(FALSE);
}
