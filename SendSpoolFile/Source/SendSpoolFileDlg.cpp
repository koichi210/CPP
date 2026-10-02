// SendSpoolFileDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "SendSpoolFile.h"
#include "SendSpoolFileDlg.h"
#include <winspool.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr DWORD READ_BUFFER_SIZE = 4096;

	// EMF スプールファイルの中身をそのままプリンタへ流し込む
	BOOL SpoolJob(HANDLE hPrinter, const CString& spoolName)
	{
		// DOC_INFO_1 のメンバは非 const の LPSTR なので、書き換え可能なバッファを渡す
		CString docName = spoolName;
		char dataType[] = "NT EMF 1.008";

		DOC_INFO_1 docInfo = {};
		docInfo.pDocName = docName.GetBuffer();
		docInfo.pOutputFile = nullptr;
		docInfo.pDatatype = dataType;

		BOOL bRtn = TRUE;
		DWORD jobId = StartDocPrinter(hPrinter, 1, reinterpret_cast<LPBYTE>(&docInfo));
		if (!jobId)
		{
			bRtn = FALSE;
		}
		else
		{
			HANDLE hFile = CreateFile(
				spoolName,
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				nullptr,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				bRtn = FALSE;
			}
			else
			{
				DWORD fileSize = GetFileSize(hFile, nullptr);
				DWORD total = 0;
				CHAR buff[READ_BUFFER_SIZE];

				while (total != fileSize)
				{
					DWORD readSize = 0;
					if (!ReadFile(hFile, buff, sizeof(buff), &readSize, nullptr) || readSize == 0)
					{
						bRtn = FALSE;
						break;
					}
					DWORD writeSize = 0;
					if (!WritePrinter(hPrinter, buff, readSize, &writeSize))
					{
						::MessageBox(nullptr, "WritePrinter error\n", "Warning!!", MB_OK);
					}
					total += readSize;
				}
				CloseHandle(hFile);
			}
			EndDocPrinter(hPrinter);
		}
		docName.ReleaseBuffer();

		return bRtn;
	}
}

CSendSpoolFileDlg::CSendSpoolFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendSpoolFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSendSpoolFileDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, &CSendSpoolFileDlg::OnBrowse)
	ON_BN_CLICKED(IDC_EXE, &CSendSpoolFileDlg::OnExecute)
END_MESSAGE_MAP()

BOOL CSendSpoolFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	AddPrinters(PRINTER_ENUM_LOCAL);
	AddPrinters(PRINTER_ENUM_FAVORITE);

	return TRUE;
}

void CSendSpoolFileDlg::AddPrinters(DWORD enumFlags)
{
	DWORD dwNeeded = 0;
	DWORD dwNum = 0;

	// 1回目で必要サイズを得て、2回目で実際に取得する
	EnumPrinters(enumFlags, nullptr, 4, nullptr, 0, &dwNeeded, &dwNum);
	std::vector<BYTE> buffer(dwNeeded);
	auto* ppi4 = reinterpret_cast<PRINTER_INFO_4*>(buffer.data());
	if (!EnumPrinters(enumFlags, nullptr, 4, buffer.data(), dwNeeded, &dwNeeded, &dwNum))
	{
		dwNum = 0;
	}

	for (DWORD i = 0; i < dwNum; i++)
	{
		SendDlgItemMessage(IDCB_PRINTER_NAME, CB_INSERTSTRING, i, reinterpret_cast<LPARAM>(ppi4[i].pPrinterName));
	}
	if (dwNum > 0)
	{
		SendDlgItemMessage(IDCB_PRINTER_NAME, CB_SETCURSEL, 0, 0);
	}
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void CSendSpoolFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSendSpoolFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendSpoolFileDlg::OnBrowse()
{
	char szFileNames[MAX_PATH] = "";

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
		"スプールファイル（*.SPL）|*.spl;|すべてのﾌｧｲﾙ （*.*）|*.*||", this);
	dlg.GetOFN().lpstrFile = szFileNames;
	dlg.GetOFN().nMaxFile = _countof(szFileNames);
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_SPOOL_NAME, szFileNames);
	}
}

void CSendSpoolFileDlg::OnExecute()
{
	CString strPrinterName;
	CString strSpoolFileName;
	GetDlgItemText(IDCB_PRINTER_NAME, strPrinterName);
	GetDlgItemText(IDC_SPOOL_NAME, strSpoolFileName);

	if (strPrinterName.IsEmpty() || strSpoolFileName.IsEmpty())
	{
		MessageBox("Illegal input parameter");
		return;
	}

	HANDLE hPrinter = nullptr;
	PRINTER_DEFAULTS printerDefaults = {};
	printerDefaults.DesiredAccess = PRINTER_ALL_ACCESS;

	// OpenPrinter の第1引数は非 const の LPSTR
	BOOL bOpened = OpenPrinter(strPrinterName.GetBuffer(), &hPrinter, &printerDefaults);
	strPrinterName.ReleaseBuffer();
	if (bOpened)
	{
		SpoolJob(hPrinter, strSpoolFileName);
		ClosePrinter(hPrinter);
	}
	else
	{
		MessageBox("OpenPrinter() - error");
	}
}
