// send_spool_file_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "send_spool_file.h"
#include "send_spool_file_dlg.h"
#include <winspool.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr DWORD kReadBufferSize = 4096;

	// スプールファイルの中身を、開始済みの印刷ジョブへ書き込む
	BOOL WriteFileToPrinter(HANDLE printer, const CString& spool_name)
	{
		HANDLE file = CreateFile(
			spool_name,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);
		if (file == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		BOOL result = TRUE;
		const DWORD file_size = GetFileSize(file, nullptr);
		DWORD total = 0;
		CHAR buff[kReadBufferSize];

		while (total != file_size)
		{
			DWORD read_size = 0;
			if (!ReadFile(file, buff, sizeof(buff), &read_size, nullptr) || read_size == 0)
			{
				result = FALSE;
				break;
			}
			DWORD write_size = 0;
			if (!WritePrinter(printer, buff, read_size, &write_size))
			{
				::MessageBox(nullptr, "WritePrinter error\n", "Warning!!", MB_OK);
			}
			total += read_size;
		}
		CloseHandle(file);
		return result;
	}

	// EMF スプールファイルの中身をそのままプリンタへ流し込む
	BOOL SpoolJob(HANDLE printer, const CString& spool_name)
	{
		// DOC_INFO_1 のメンバは非 const の LPSTR なので、書き換え可能なバッファを渡す
		CString doc_name = spool_name;
		char data_type[] = "NT EMF 1.008";

		DOC_INFO_1 doc_info = {};
		doc_info.pDocName = doc_name.GetBuffer();
		doc_info.pOutputFile = nullptr;
		doc_info.pDatatype = data_type;

		if (!StartDocPrinter(printer, 1, reinterpret_cast<LPBYTE>(&doc_info)))
		{
			return FALSE;
		}

		const BOOL result = WriteFileToPrinter(printer, spool_name);
		EndDocPrinter(printer);
		return result;
	}
}

SendSpoolFileDlg::SendSpoolFileDlg(CWnd* parent /*=nullptr*/)
	: CDialog(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SendSpoolFileDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(SendSpoolFileDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, &SendSpoolFileDlg::OnBrowse)
	ON_BN_CLICKED(IDC_EXE, &SendSpoolFileDlg::OnExecute)
END_MESSAGE_MAP()

BOOL SendSpoolFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	AddPrinters(PRINTER_ENUM_LOCAL);
	AddPrinters(PRINTER_ENUM_FAVORITE);

	return TRUE;
}

void SendSpoolFileDlg::AddPrinters(DWORD enum_flags)
{
	DWORD needed = 0;
	DWORD num = 0;

	// 1回目で必要サイズを得て、2回目で実際に取得する
	EnumPrinters(enum_flags, nullptr, 4, nullptr, 0, &needed, &num);
	std::vector<BYTE> buffer(needed);
	auto* printer_infos = reinterpret_cast<PRINTER_INFO_4*>(buffer.data());
	if (!EnumPrinters(enum_flags, nullptr, 4, buffer.data(), needed, &needed, &num))
	{
		num = 0;
	}

	for (DWORD i = 0; i < num; i++)
	{
		SendDlgItemMessage(IDCB_PRINTER_NAME, CB_INSERTSTRING, i, reinterpret_cast<LPARAM>(printer_infos[i].pPrinterName));
	}
	if (num > 0)
	{
		SendDlgItemMessage(IDCB_PRINTER_NAME, CB_SETCURSEL, 0, 0);
	}
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void SendSpoolFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int icon_width = GetSystemMetrics(SM_CXICON);
		int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - icon_width + 1) / 2;
		int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR SendSpoolFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void SendSpoolFileDlg::OnBrowse()
{
	char file_names[MAX_PATH] = "";

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY,
		"スプールファイル（*.SPL）|*.spl;|すべてのﾌｧｲﾙ （*.*）|*.*||", this);
	dlg.GetOFN().lpstrFile = file_names;
	dlg.GetOFN().nMaxFile = _countof(file_names);
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_SPOOL_NAME, file_names);
	}
}

void SendSpoolFileDlg::OnExecute()
{
	CString printer_name;
	CString spool_file_name;
	GetDlgItemText(IDCB_PRINTER_NAME, printer_name);
	GetDlgItemText(IDC_SPOOL_NAME, spool_file_name);

	if (printer_name.IsEmpty() || spool_file_name.IsEmpty())
	{
		MessageBox("Illegal input parameter");
		return;
	}

	HANDLE printer = nullptr;
	PRINTER_DEFAULTS printer_defaults = {};
	printer_defaults.DesiredAccess = PRINTER_ALL_ACCESS;

	// OpenPrinter の第1引数は非 const の LPSTR
	BOOL opened = OpenPrinter(printer_name.GetBuffer(), &printer, &printer_defaults);
	printer_name.ReleaseBuffer();
	if (opened)
	{
		SpoolJob(printer, spool_file_name);
		ClosePrinter(printer);
	}
	else
	{
		MessageBox("OpenPrinter() - error");
	}
}
