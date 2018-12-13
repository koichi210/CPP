// SendSpoolFileDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SendSpoolFile.h"
#include "SendSpoolFileDlg.h"
#include <winspool.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendSpoolFileDlg ダイアログ
CSendSpoolFileDlg::CSendSpoolFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendSpoolFileDlg::IDD, pParent)
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
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_EXE, OnExecute)
END_MESSAGE_MAP()


// CSendSpoolFileDlg メッセージ ハンドラ

BOOL CSendSpoolFileDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定


	// プリンタ名称を列挙
	AddMyPrinter(PRINTER_ENUM_LOCAL);
	AddMyPrinter(PRINTER_ENUM_FAVORITE);


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CSendSpoolFileDlg::AddMyPrinter(DWORD PrinterEnumId)
{
	PRINTER_INFO_4	*ppi4;
	DWORD			dwNeeded;
	DWORD			dwNum;

	EnumPrinters(PrinterEnumId, NULL, 4, NULL, 0, &dwNeeded, &dwNum);
	ppi4 = (PRINTER_INFO_4 *) new BYTE [dwNeeded];
	if ( ppi4 )
	{
		EnumPrinters(PrinterEnumId, NULL, 4, (LPBYTE)ppi4, dwNeeded, &dwNeeded, &dwNum);
		for( DWORD i=0; i < dwNum; i++ )
		{
			SendDlgItemMessage(IDCB_PRINTER_NAME, CB_INSERTSTRING, i, (LPARAM)ppi4[i].pPrinterName);
		}
		if ( dwNum > 0 )
		{
			SendDlgItemMessage(IDCB_PRINTER_NAME, CB_SETCURSEL, 0, 0);
		}
		delete ppi4;
	}
}


// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSendSpoolFileDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSendSpoolFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSendSpoolFileDlg::OnBrowse()
{
	CString	strFileType;
	char	szFileNames[MAX_PATH]="\0";

	//スプールファイル名を選択する画面表示
	strFileType.Format("スプールファイル（*.SPL）|*.spl;|すべてのﾌｧｲﾙ （*.*）|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFileType, this);
	dlg.GetOFN().lpstrFile = szFileNames;
	dlg.GetOFN().nMaxFile = sizeof(szFileNames) / sizeof(char);
	if ( dlg.DoModal() == IDOK )
	{
		SetDlgItemText(IDC_SPOOL_NAME, szFileNames);
	}
}

void CSendSpoolFileDlg::OnExecute()
{
	char				szPrinterName[MAX_PATH]="\0";
	char				szSpoolFileName[MAX_PATH]="\0";

	GetDlgItem(IDCB_PRINTER_NAME)->GetWindowText(szPrinterName,sizeof(szPrinterName));
	GetDlgItem(IDC_SPOOL_NAME)->GetWindowText(szSpoolFileName,sizeof(szSpoolFileName));

	if( strcmp(szPrinterName,"") && strcmp(szSpoolFileName,"") )
	{
		HANDLE				hPrinter=NULL;
		PRINTER_DEFAULTS	printer_defaults;

		ZeroMemory(&printer_defaults, sizeof(PRINTER_DEFAULTS));
		printer_defaults.DesiredAccess = PRINTER_ALL_ACCESS;

		if(OpenPrinter((LPSTR)szPrinterName, &hPrinter, &printer_defaults))
		{
			//EMFファイルをスプーラに投げる
			SpoolJob(hPrinter, szSpoolFileName);
			ClosePrinter(hPrinter);
		}
		else
		{
			MessageBox("OpenPrinter() - error");
		}
	}
	else
	{
		MessageBox("Illegal input parameter");
	}

}

BOOL SpoolJob(HANDLE hPrinter, LPSTR SpoolName)
{
	BOOL		bRtn = TRUE;
	DWORD		JobId;
	DOC_INFO_1	DocInfo;

	DocInfo.pDocName = SpoolName;
	DocInfo.pOutputFile = NULL;
//	DocInfo.pDatatype = "RAW";
	DocInfo.pDatatype = "NT EMF 1.008";

	// StartDoc
	JobId = StartDocPrinter(hPrinter, 1, (LPBYTE)&DocInfo);
	if ( ! JobId )
	{
		bRtn = FALSE;
	}
	else
	{
		HANDLE	hFile;

		// EMFファイルを開く
		hFile = CreateFile(
							SpoolName,
							GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							(LPSECURITY_ATTRIBUTES)NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
		if ( hFile == INVALID_HANDLE_VALUE )
		{
			bRtn = FALSE;
		}
		else
		{
			DWORD		FileSize, ReadSize, WriteSize;
			DWORD		Total = 0;

//			BOOL		bStartPagePrinter;
//			BOOL		bEndPagePrinter;
			BOOL		bWritePrinter;
			DWORD		cnt = 0;
			CHAR		buff[MAX_BUFF];

			FileSize = GetFileSize(hFile, NULL);

			while( Total != FileSize )
			{
				ReadFile(hFile, buff, sizeof(buff), &ReadSize, NULL);
//				bStartPagePrinter = StartPagePrinter(hPrinter);
				bWritePrinter = WritePrinter(hPrinter, buff, ReadSize, &WriteSize);
				if ( ! bWritePrinter )
				{
					MessageBox(NULL,"WritePrinter error\n","Warning!!",MB_OK);
				}
//				bEndPagePrinter = EndPagePrinter(hPrinter);
				Total += ReadSize;
				cnt++;

				//TODO：必要？
				if ( cnt%30 == 0 )
				{
					ReadSize = 0;
				}
			}
			CloseHandle(hFile);
		}
		EndDocPrinter(hPrinter);
	}

	return bRtn;
}
