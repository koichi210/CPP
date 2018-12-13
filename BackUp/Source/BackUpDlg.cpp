// BackUpDlg.cpp : �����t�@�C��
//

#include <shlwapi.h>
#include <shlobj.h>
#include "stdafx.h"
#include "BackUp.h"
#include "BackUpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBackUpDlg �_�C�A���O




CBackUpDlg::CBackUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackUpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_idx = 0;
	m_end = END_NONE;
	m_FileOut = FALSE;

	m_bkDefault.bBkEnable = TRUE;
	m_bkDefault.opt = SUBDIR_MASK | DIFF_MASK;
	m_bkDefault.strDstPath = "";
	m_bkDefault.strSrcPath = "";
}

void CBackUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL, m_listctl);
}

BEGIN_MESSAGE_MAP(CBackUpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BACKUP_START, OnBackupStart)
	ON_BN_CLICKED(IDC_DIFF, OnDiff)
	ON_BN_CLICKED(IDC_BROWSE_SRC, OnBrowseSrc)
	ON_BN_CLICKED(IDC_BROWSE_DEST, OnBrowseDest)
	ON_BN_CLICKED(IDC_SAVE_SETTING, OnBnClickedSaveSetting)
	ON_BN_CLICKED(IDC_SUBDIR, OnBnClickedSubdir)
	ON_BN_CLICKED(IDC_OVERWRITE, OnBnClickedOverWrite)
	ON_BN_CLICKED(IDC_SELECT_BACKUP, OnBnClickedEnableBK)
	ON_EN_CHANGE(IDC_EDIT_SRC, OnEnChangeEditSrc)
	ON_EN_CHANGE(IDC_EDIT_DST, OnEnChangeEditDst)
	ON_BN_CLICKED(IDC_ALL_CLEAR, OnBnClickedAllClear)
	ON_BN_CLICKED(IDC_END_NONE, OnBnClickedEndNone)
	ON_BN_CLICKED(IDC_END_REBOOT, OnBnClickedEndReboot)
	ON_BN_CLICKED(IDC_END_SHUTDOWN, OnBnClickedEndShutdown)
	ON_BN_CLICKED(IDC_END_APP, OnBnClickedEndApp)
END_MESSAGE_MAP()


// CBackUpDlg ���b�Z�[�W �n���h��

BOOL CBackUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// ������
	CheckRadioButton( IDC_END_NONE, IDC_END_SHUTDOWN, IDC_END_NONE );
	LCSetData();
	Refresh();

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CBackUpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CBackUpDlg::OnPaint()
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
HCURSOR CBackUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBackUpDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ( pNMLV )
	{
		if ( pNMLV->iItem != m_idx )
		{
			m_idx = pNMLV->iItem;
			Refresh();
		}
	}
	*pResult = 0;
}


void CBackUpDlg::LCSetData()
{
	LV_COLUMN	lvCol;
	LV_ITEM		lvItem;
	char		str[MAX_PATH];

	// Column���ʐݒ�
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	lvCol.fmt  = LVCFMT_LEFT;

	// Item���ʐݒ�
	lvItem.mask      = LVIF_TEXT | LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state     = 0;

	// ���x���}��
	lvCol.cx   = LC_VAL_WIDTH;
	InsertListCtrl(lvCol,SUBITEM_ENABLE_BK,STR_ENABLE_BK);
	InsertListCtrl(lvCol,SUBITEM_NUMBER,STR_NUMBER);
	lvCol.cx   = LC_STR_WIDTH;
	InsertListCtrl(lvCol,SUBITEM_SRC_PATH,STR_ORG_PATH);
	InsertListCtrl(lvCol,SUBITEM_DST_PATH,STR_NEW_PATH);
	lvCol.cx   = LC_VAL_WIDTH;
	InsertListCtrl(lvCol,SUBITEM_SUBDIRECTORY,STR_SUBDIRECTORY);
	InsertListCtrl(lvCol,SUBITEM_DIFF_FILE,STR_DIFF_FILE);
	InsertListCtrl(lvCol,SUBITEM_OVERWRITE,STR_OVERWRITE);

	m_idx = 0;
	ReadSetting();
	for( int i=0,idx=0; i < MAXNUM_IDX; i++ )
	{
		lvItem.iItem = idx;

		//�o�b�N�A�b�v�L��
		if ( m_bkStruct[i].bBkEnable )
			sprintf_s(str,sizeof(str),STR_ENABLE);
		else
			sprintf_s(str,sizeof(str),STR_DISABLE);
		lvItem.iSubItem  = SUBITEM_ENABLE_BK;
		lvItem.pszText   = str;
		m_listctl.InsertItem(&lvItem);

		//No
		sprintf_s(str,sizeof(str),"%d",i+1);
		lvItem.iSubItem  = SUBITEM_NUMBER;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		//���p�X
		lvItem.iSubItem  = SUBITEM_SRC_PATH;
		lvItem.pszText   = (LPSTR)m_bkStruct[i].strSrcPath.GetString();
		m_listctl.SetItem(&lvItem);

		//��p�X
		lvItem.iSubItem  = SUBITEM_DST_PATH;
		lvItem.pszText   = (LPSTR)m_bkStruct[i].strDstPath.GetString();
		m_listctl.SetItem(&lvItem);

		// �T�u�f�B���N�g�����Ώ�
		if ( m_bkStruct[i].opt & SUBDIR_MASK )
			sprintf_s(str,sizeof(str),STR_ON);
		else
			sprintf_s(str,sizeof(str),STR_OFF);
		lvItem.iSubItem  = SUBITEM_SUBDIRECTORY;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		//�����̂ݑΏ�
		if ( m_bkStruct[i].opt & DIFF_MASK )
			sprintf_s(str,sizeof(str),STR_ON);
		else
			sprintf_s(str,sizeof(str),STR_OFF);
		lvItem.iSubItem  = SUBITEM_DIFF_FILE;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		//�㏑���m�F����
		if ( m_bkStruct[i].opt & OVERWRITE_MASK )
			sprintf_s(str,sizeof(str),STR_ON);
		else
			sprintf_s(str,sizeof(str),STR_OFF);
		lvItem.iSubItem  = SUBITEM_OVERWRITE;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		idx++;
	}

	m_listctl.SetExtendedStyle(LVS_EX_FULLROWSELECT); //�s�őI������悤�Ɏw��
	m_listctl.SetItemState(m_idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}

void CBackUpDlg::InsertListCtrl(LV_COLUMN lvCol, int idx, CString name)
{
	lvCol.iSubItem = idx;
	lvCol.pszText  = (LPSTR)name.GetString();
	m_listctl.InsertColumn(idx, &lvCol);
}

void CBackUpDlg::OnBackupStart()
{
	char str[MAX_PATH];
	BOOL bErrFind = FALSE;

	strcpy_s(str,sizeof(str),"�o�b�N�A�b�v��");
	SetDlgItemText(IDC_LABEL, str);
	
	bErrFind = BackUpProc();
	if ( bErrFind == FALSE )
	{
		strcpy_s(str,sizeof(str),"�o�b�N�A�b�v����I��");
	}
	else
	{
		strcpy_s(str,sizeof(str),"�o�b�N�A�b�v�Ɏ��s���܂���");
	}
	SetDlgItemText(IDC_LABEL, str);

	if ( m_end == END_SHUTDOWN )
	{
		system("shutdown -s -t 0");
	}
	else if ( m_end == END_REBOOT )
	{
		system("shutdown -r -t 0");
	}
	else if ( m_end == END_APP )
	{
		CDialog::OnOK();
	}
}

BOOL CBackUpDlg::GetDirectory(TCHAR * dir) 
{
	LPITEMIDLIST pidlSelected = NULL;
	BROWSEINFO browseInfo = { 0 };
	char folderName[MAX_PATH] = { 0 };
	BOOL rt = FALSE;

	browseInfo.hwndOwner = this->m_hWnd;
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = folderName;
	CString title;
	title="�ړI�̃t�H���_��I�����ĉ������B�B";
	browseInfo.lpszTitle = title.GetBuffer(10);
	browseInfo.ulFlags = 0;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0; 
	browseInfo.ulFlags = 0x41;

	title.ReleaseBuffer();

	pidlSelected = SHBrowseForFolder(&browseInfo);
	if (pidlSelected != NULL)
	{
		SHGetPathFromIDList(pidlSelected, dir);
		rt = TRUE;
	}
	return rt;
}

BOOL CBackUpDlg::BackUpProc()
{
	BOOL bReturn = FALSE;
	TCHAR cmdName[MAX_PATH];
	TCHAR ExeName[MAX_PATH];
	CString cmd;

	for(int idx=0; idx < MAXNUM_IDX; idx++)
	{
		if(m_bkStruct[idx].bBkEnable == FALSE ||
			m_bkStruct[idx].strSrcPath.Compare("") == 0 ||
			m_bkStruct[idx].strDstPath.Compare("") == 0 )
		{
			continue;
		}

		memset(cmdName,0,sizeof(cmdName));
		memset(ExeName,0,sizeof(ExeName));

		strcpy_s(cmdName,sizeof(cmdName), "xcopy");
		if(m_bkStruct[idx].opt & SUBDIR_MASK)
		{
			strcat_s(cmdName,sizeof(cmdName)," /E");	//	/E�@�f�B���N�g�����ƃR�s�[
		}

		if(m_bkStruct[idx].opt & DIFF_MASK)
		{
			strcat_s(cmdName,sizeof(cmdName)," /D");	//	/D�@�V�����t�@�C���̂݃R�s�[
		}

		if(m_bkStruct[idx].opt & OVERWRITE_MASK)
		{
			strcat_s(cmdName,sizeof(cmdName)," /-Y");	//	/-Y�@�㏑���̊m�F��\��
		}
		else
		{
			strcat_s(cmdName,sizeof(cmdName)," /Y");	//	/-Y�@�㏑���̊m�F��\�����Ȃ�
		}

		strcat_s(cmdName,sizeof(cmdName)," /I");	//	/I�@�󂯑��f�B���N�g����V�K�쐬

		strcat_s(cmdName,sizeof(cmdName)," /H");	//	/H�@�B���t�@�C����V�X�e���t�@�C�����Ώ�

		strcat_s(cmdName,sizeof(cmdName)," /R");	//	/R	�ǂݎ���p�ł��㏑��

//		strcat_s(cmdName,sizeof(cmdName)," /Q");	//	/Q�@�R�s�[���A�t�@�C������\�����Ȃ�
//		strcat_s(cmdName,sizeof(cmdName)," /F");	//	/F�@�R�s�[���A���葤�Ǝ󂯑��̑S�t�@�C�����\��
//		strcat_s(cmdName,sizeof(cmdName)," /L");	//	/L�@�R�s�[�Ώۂ̃t�@�C������\���˃R�s�[�����\���̂�

		sprintf_s(ExeName,sizeof(ExeName),"%s \"%s\" \"%s\"\n",cmdName,m_bkStruct[idx].strSrcPath,m_bkStruct[idx].strDstPath);

		if ( m_FileOut )
		{
			cmd.Append(ExeName);
		}
		else
		{
			int rt;
			rt = system(ExeName);
			if ( bReturn == FALSE && rt )
			{
				bReturn = rt;
			}
		}
	}

	if ( m_FileOut )
	{
		WriteBatchFile(cmd);
	}

	return bReturn ;
}


void CBackUpDlg::OnBrowseSrc()
{
	TCHAR pathName[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_EDIT_SRC, pathName,sizeof(pathName));
	if(GetDirectory(pathName) == TRUE){
		SetDlgItemText(IDC_EDIT_SRC, pathName);
		m_bkStruct[m_idx].strSrcPath.SetString(pathName);
		UpdateSrcPath(pathName);
	}
}

void CBackUpDlg::OnBrowseDest()
{
	TCHAR pathName[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_EDIT_SRC, pathName,sizeof(pathName));
	if(GetDirectory(pathName) == TRUE){
		SetDlgItemText(IDC_EDIT_DST, pathName);
		m_bkStruct[m_idx].strDstPath.SetString(pathName);
		UpdateDstPath(pathName);
	}
}

void CBackUpDlg::OnEnChangeEditSrc()
{
	char str[MAX_PATH];

	GetDlgItemText(IDC_EDIT_SRC, str, sizeof(str));
	m_bkStruct[m_idx].strSrcPath.SetString(str);
	m_listctl.SetItemText(m_idx, SUBITEM_SRC_PATH, str);

}

void CBackUpDlg::OnEnChangeEditDst()
{
	char str[MAX_PATH];

	GetDlgItemText(IDC_EDIT_DST, str, sizeof(str));
	m_bkStruct[m_idx].strDstPath.SetString(str);
	m_listctl.SetItemText(m_idx, SUBITEM_DST_PATH, str);

}

void CBackUpDlg::OnBnClickedEnableBK()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_SELECT_BACKUP) == BST_CHECKED);
	m_bkStruct[m_idx].bBkEnable = bOn;
	UpdateEnableBK(bOn);
}

void CBackUpDlg::OnBnClickedSubdir()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_SUBDIR) == BST_CHECKED);
	if ( bOn )
		m_bkStruct[m_idx].opt |= SUBDIR_MASK;
	else
		m_bkStruct[m_idx].opt &= ~SUBDIR_MASK;
	UpdateSubDirectory(m_bkStruct[m_idx].opt);
}

void CBackUpDlg::OnDiff()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_DIFF) == BST_CHECKED);
	if ( bOn )
	{
		CheckDlgButton(IDC_OVERWRITE, BST_UNCHECKED);
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(FALSE);
		m_bkStruct[m_idx].opt |= DIFF_MASK;
		m_bkStruct[m_idx].opt &= ~OVERWRITE_MASK;
		UpdateOverWrite(m_bkStruct[m_idx].opt);
	}
	else
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(TRUE);
		m_bkStruct[m_idx].opt &= ~DIFF_MASK;
	}

	UpdateDiffFile(m_bkStruct[m_idx].opt);
}

void CBackUpDlg::OnBnClickedOverWrite()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_OVERWRITE) == BST_CHECKED);
	if ( bOn )
	{
		m_bkStruct[m_idx].opt |= OVERWRITE_MASK;
	}
	else
	{
		m_bkStruct[m_idx].opt &= ~OVERWRITE_MASK;
	}
	UpdateOverWrite(m_bkStruct[m_idx].opt);
}

void CBackUpDlg::UpdateEnableBK(BOOL bChk, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( bChk == TRUE )
	{
		CheckDlgButton(IDC_SELECT_BACKUP, BST_CHECKED);
		str.SetString(STR_ENABLE);
	}
	else
	{
		CheckDlgButton(IDC_SELECT_BACKUP, BST_UNCHECKED);
		str.SetString(STR_DISABLE);
	}

	m_listctl.SetItemText(nItem, SUBITEM_ENABLE_BK, str);
}

void CBackUpDlg::UpdateSubDirectory(DWORD opt, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( opt & SUBDIR_MASK )
	{
		CheckDlgButton(IDC_SUBDIR, BST_CHECKED);
		str.SetString(STR_ON);
	}
	else
	{
		CheckDlgButton(IDC_SUBDIR, BST_UNCHECKED);
		str.SetString(STR_OFF);
	}

	m_listctl.SetItemText(nItem, SUBITEM_SUBDIRECTORY, str);
}

void CBackUpDlg::UpdateDiffFile(DWORD opt, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( opt & DIFF_MASK )
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(FALSE);
		CheckDlgButton(IDC_DIFF, BST_CHECKED);
		str.SetString(STR_ON);
	}
	else
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(TRUE);
		CheckDlgButton(IDC_DIFF, BST_UNCHECKED);
		str.SetString(STR_OFF);
	}

	m_listctl.SetItemText(nItem, SUBITEM_DIFF_FILE, str);
}

void CBackUpDlg::UpdateOverWrite(DWORD opt, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( opt & OVERWRITE_MASK )
	{
		CheckDlgButton(IDC_OVERWRITE, BST_CHECKED);
		str.SetString(STR_ON);
	}
	else
	{
		CheckDlgButton(IDC_OVERWRITE, BST_UNCHECKED);
		str.SetString(STR_OFF);
	}

	m_listctl.SetItemText(nItem, SUBITEM_OVERWRITE, str);
}

void CBackUpDlg::UpdateSrcPath(char *str, int idx)
{
	int nItem = GetIdx(idx);

	SetDlgItemText(IDC_EDIT_SRC, str);
	m_listctl.SetItemText(nItem, SUBITEM_SRC_PATH, str);
}

void CBackUpDlg::UpdateDstPath(char *str, int idx)
{
	int nItem = GetIdx(idx);

	SetDlgItemText(IDC_EDIT_DST, str);
	m_listctl.SetItemText(nItem, SUBITEM_DST_PATH, str);
}

void CBackUpDlg::Refresh()
{
	UpdateEnableBK(m_bkStruct[m_idx].bBkEnable);
	UpdateSubDirectory(m_bkStruct[m_idx].opt);
	UpdateDiffFile(m_bkStruct[m_idx].opt);
	UpdateOverWrite(m_bkStruct[m_idx].opt);
	UpdateSrcPath((LPSTR)m_bkStruct[m_idx].strSrcPath.GetString());
	UpdateDstPath((LPSTR)m_bkStruct[m_idx].strDstPath.GetString());
}

void CBackUpDlg::OnBnClickedAllClear()
{
	// TODO�F�o�O���Ă�H
	for(int i=0; i < MAXNUM_IDX; i++)
	{
		memcpy(&m_bkStruct[i], &m_bkDefault, sizeof(m_bkDefault));
	}
	Refresh();
}

void CBackUpDlg::OnBnClickedSaveSetting()
{
	BOOL bReturn = WriteSetting();

	if ( bReturn == TRUE )
	{
		SetDlgItemText(IDC_LABEL, STR_SAVE_SET_SUCCESS);
	}
	else
	{
		SetDlgItemText(IDC_LABEL, STR_SAVE_SET_ERROR);
	}
}

BOOL CBackUpDlg::WriteSetting()
{
	char path[MAX_PATH];
	BOOL bReturn = FALSE;

	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		CStdioFile	file;

		strcat_s(path,sizeof(path),SET_FILE_NAME);
		if ( file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText) )
		{
			char str[MAX_PATH];

			for(int i=0;i<MAXNUM_IDX;i++)
			{
				if( m_bkStruct[i].strSrcPath.Compare("") == 0 ||
					m_bkStruct[i].strDstPath.Compare("") == 0 )
				{
					continue;
				}

				sprintf_s(str, sizeof(str),"%d,%d,%s,%s\n",
					m_bkStruct[i].bBkEnable,
					m_bkStruct[i].opt,
					m_bkStruct[i].strSrcPath,
					m_bkStruct[i].strDstPath);
				file.WriteString(str);
				bReturn = TRUE;
			}
			file.Close();
		}
	}

	return bReturn;
}

BOOL CBackUpDlg::ReadSetting()
{
	char path[MAX_PATH];
	BOOL bReturn = FALSE;
	int idx=0;

	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		CStdioFile	file;

		strcat_s(path,sizeof(path),SET_FILE_NAME);
		if ( file.Open(path, CFile::modeRead | CFile::typeText) )
		{
			CString str;
			int curPos;

			while( file.ReadString(str) )
			{
				curPos=0;
				m_bkStruct[idx].bBkEnable = atoi(str.Tokenize(",",curPos));
				m_bkStruct[idx].opt = atoi(str.Tokenize(",",curPos));
				m_bkStruct[idx].strSrcPath = str.Tokenize(",",curPos);
				m_bkStruct[idx].strDstPath = str.Tokenize(",",curPos);
				idx++;
			}

			bReturn = TRUE;
			file.Close();
		}
	}

	for(;idx<MAXNUM_IDX;idx++)
	{
		memcpy(&m_bkStruct[idx],&m_bkDefault,sizeof(m_bkDefault));
	}

	return bReturn;
}

void CBackUpDlg::WriteBatchFile(CString cmd)
{
	char path[MAX_PATH];
	BOOL bReturn = FALSE;

	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		CStdioFile	file;

		strcat_s(path,sizeof(path),BAT_FILE_NAME);
		if ( file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText) )
		{
			file.WriteString(cmd);
			file.Close();
		}
	}
}

int CBackUpDlg::GetIdx(int idx)
{
	int nItem = idx;

	if ( nItem == CURRENT_IDX )
		nItem = m_idx;

	return nItem;
}

BOOL CBackUpDlg::IsBlankData(BACKUP *BkStruct)
{
	BOOL bReturn = FALSE;

	if ( BkStruct->strSrcPath.Compare("") == 0 &&
		 BkStruct->strDstPath.Compare("") == 0 )
	{
		bReturn = TRUE;
	}

	return bReturn;
}

void CBackUpDlg::OnBnClickedEndNone()
{
	m_end = END_NONE;
}

void CBackUpDlg::OnBnClickedEndApp()
{
	m_end = END_APP;
}

void CBackUpDlg::OnBnClickedEndReboot()
{
	m_end = END_REBOOT;
}

void CBackUpDlg::OnBnClickedEndShutdown()
{
	m_end = END_SHUTDOWN;
}

void CBackUpDlg::OnCancel()
{
	m_FileOut = TRUE;
	BackUpProc();

	CDialog::OnCancel();
}

