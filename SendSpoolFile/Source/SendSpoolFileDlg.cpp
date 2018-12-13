// SendSpoolFileDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "SendSpoolFile.h"
#include "SendSpoolFileDlg.h"
#include <winspool.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendSpoolFileDlg �_�C�A���O
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


// CSendSpoolFileDlg ���b�Z�[�W �n���h��

BOOL CSendSpoolFileDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�


	// �v�����^���̂��
	AddMyPrinter(PRINTER_ENUM_LOCAL);
	AddMyPrinter(PRINTER_ENUM_FAVORITE);


	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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


// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CSendSpoolFileDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CSendSpoolFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSendSpoolFileDlg::OnBrowse()
{
	CString	strFileType;
	char	szFileNames[MAX_PATH]="\0";

	//�X�v�[���t�@�C������I�������ʕ\��
	strFileType.Format("�X�v�[���t�@�C���i*.SPL�j|*.spl;|���ׂĂ�̧�� �i*.*�j|*.*||");
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
			//EMF�t�@�C�����X�v�[���ɓ�����
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

		// EMF�t�@�C�����J��
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

				//TODO�F�K�v�H
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
