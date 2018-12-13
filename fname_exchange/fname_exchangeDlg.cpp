// fname_exchangeDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include <afxtempl.h>
#include "stdafx.h"
#include "fname_exchange.h"
#include "fname_exchangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFname_exchangeDlg �_�C�A���O

CFname_exchangeDlg::CFname_exchangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFname_exchangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFname_exchangeDlg)
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Target = TARGET_FILE;
	m_ignore_alert = TRUE;
	m_comp = FALSE;
}

void CFname_exchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFname_exchangeDlg)
	DDX_Control(pDX, IDLB_FILE, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFname_exchangeDlg, CDialog)
	//{{AFX_MSG_MAP(CFname_exchangeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_SETFOCUS(IDET_DIR, OnSetfocus)
	ON_BN_CLICKED(IDBT_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDBT_GET_FILE, OnGetFile)
	ON_BN_CLICKED(IDRB_FILE, OnFile)
	ON_BN_CLICKED(IDRB_FOLDER, OnFolder)
	ON_BN_CLICKED(IDBT_ALL_CHECK, OnAllCheck)
	ON_BN_CLICKED(IDBT_ALL_UNCHECK, OnAllUncheck)
	ON_BN_CLICKED(IDCH_IGNORE_ALERT, OnIgnoreAlert)
	ON_BN_CLICKED(IDCH_COMP_BIG_SMALL, OnCompBigSmall)
	ON_BN_CLICKED(IDRB_ENUM, OnRADIOEnum)
	ON_BN_CLICKED(IDRB_ALL_DBCS, OnRADIOAllDbcs)
	ON_BN_CLICKED(IDRB_ALL_SBCS, OnRADIOAllSbcs)
	ON_BN_CLICKED(IDRB_DEL_NUM, OnRADIODelEnum)
	ON_BN_CLICKED(IDRB_DEL_DIST, OnRADIODelDist)
	ON_BN_CLICKED(IDRB_ADD, OnRadioAdd)
	ON_BN_CLICKED(IDRB_DEL, OnRadioDel)
	ON_BN_CLICKED(IDRB_REP, OnRadioRep)
	ON_BN_CLICKED(IDBT_EXE, OnExecute)
	ON_BN_CLICKED(IDBT_UNDO, OnUndo)
	ON_BN_CLICKED(IDBT_END, OnEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFname_exchangeDlg ���b�Z�[�W �n���h��

BOOL CFname_exchangeDlg::OnInitDialog()
{
	long ind;
	CString str;

	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�

	str.LoadString(IDSTR_FILE_EXCHANGE);
	SetWindowText(str);
	SetDlgItemTextAll(this->m_hWnd, tbl, sizeof(tbl) / sizeof(tbl[0]));

    CheckRadioButton(IDRB_FILE, IDRB_FOLDER,IDRB_FILE);
	SetDlgItemText(IDET_FIRST_NUM, "1");
    CheckRadioButton(IDRB_ENUM,IDRB_REP,IDRB_ENUM);
	CheckDlgButton(IDCH_KEEP_NAME, BST_CHECKED);
	CheckDlgButton(IDCH_ADD_BEF, BST_CHECKED);
	CheckDlgButton(IDCH_IGNORE_ALERT, DEFAULT_ALERT);
	m_type = ENUM;

	//makelist box
	SendDlgItemMessage(IDCB_KETA, CB_RESETCONTENT, 0, 0L);

	str.LoadString(IDSTR_AUTO);
	SendDlgItemMessage(IDCB_KETA, CB_RESETCONTENT, 0, 0L);
	ind = SendDlgItemMessage(IDCB_KETA, CB_ADDSTRING, 0,(LPARAM)str.GetString());
	SendDlgItemMessage(IDCB_KETA, CB_SETITEMDATA, (WPARAM)0, ind);
	for (int i=1; i <= KETA_MAX; i++)
	{
		str.Format(IDSTR_KETA,i);
		ind = SendDlgItemMessage(IDCB_KETA, CB_ADDSTRING, 0,(LPARAM)str.GetString());
		SendDlgItemMessage(IDCB_KETA, CB_SETITEMDATA, (WPARAM)i, ind);
	}
    SendDlgItemMessage(IDCB_KETA,CB_SETCURSEL,0,0L);

	InitUndo();
	Refresh();

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CFname_exchangeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CFname_exchangeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CFname_exchangeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CFname_exchangeDlg::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}


void CFname_exchangeDlg::OnSetfocus()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	//TODO:�t�H�[�J�X������������A�I����Ԃ𔽓]������
}

void CFname_exchangeDlg::OnBrowse() 
{
	CString title,path;
	title = "Select a destination folder";

	BOOL rt = Browse(this->m_hWnd, title, &path);
	if ( rt )
	{
		SetDlgItemText(IDET_DIR, path);
		OnGetFile();
	}
}

void CFname_exchangeDlg::OnGetFile() 
{
	TCHAR pathName[MAX_PATH] = { 0 };
	size_t szPathLength;
	int szLen;
	int nMax=0;

	// �w��t�H���_���̃t�@�C����\������
	GetDlgItemText(IDET_DIR,pathName,sizeof(pathName));
	if(lstrcmp(pathName,"") == 0)
	{
		return;
	}

	szPathLength = strlen(pathName);
	szPathLength++; // "\\"
	strcat_s(pathName,sizeof(pathName), "\\*.*");

	// �\�����e�����Z�b�g����
	m_list.ResetContent();

	if ( m_Target == TARGET_FILE )
	{
		// m_list.Dir �Ńt�@�C��
		m_list.Dir(DDL_READWRITE,pathName);
//		m_list.Dir(DDL_DIRECTORY,pathName);
	}
	else // ( m_Target == TARGET_DIRECOTY )
	{
		// cFileFind �Ńt�H���_���擾
		CFileFind cFind;
		CString strFolder;
		BOOL bContinue = cFind.FindFile(pathName);

		while( bContinue )
		{
			bContinue = cFind.FindNextFile();

			if( cFind.IsDirectory() )
			{
				strFolder = cFind.GetFilePath();
				strFolder.Delete(0, szPathLength);
				if ( (strcmp(strFolder, ".") == 0 ) ||
					 (strcmp(strFolder, "..") == 0 ) )
				{
					continue;
				}

				m_list.AddString(strFolder);
				szLen = strFolder.GetLength();
				if ( nMax < szLen )
				{
					nMax = szLen;
				}
			}
		}
	}
	m_list.SetHorizontalExtent(nMax * HORIZONTAIL_OFFSET);
}

void CFname_exchangeDlg::OnFile()
{
	m_Target = TARGET_FILE;
	OnGetFile();
}

void CFname_exchangeDlg::OnFolder()
{
	m_Target = TARGET_DIRECTOY;
	OnGetFile();
}

void CFname_exchangeDlg::OnAllCheck() 
{
	m_list.SetSel(-1,TRUE);
}

void CFname_exchangeDlg::OnAllUncheck() 
{
	m_list.SetSel(-1,FALSE);
}

void CFname_exchangeDlg::OnIgnoreAlert()
{
	UINT check = IsDlgButtonChecked(IDCH_IGNORE_ALERT);
	if( check == BST_CHECKED )
	{
		m_ignore_alert = TRUE;
	}
	else
	{
		m_ignore_alert = FALSE;
	}
}

void CFname_exchangeDlg::OnCompBigSmall()
{
	UINT check = IsDlgButtonChecked(IDCH_COMP_BIG_SMALL);
	if( check == BST_CHECKED )
	{
		m_comp = TRUE;
	}
	else
	{
		m_comp = FALSE;
	}
}

void CFname_exchangeDlg::OnEnd() 
{
	CDialog::OnOK();
}

void CFname_exchangeDlg::OnRADIOEnum() 
{
	m_type = ENUM;
	Refresh();
}

void CFname_exchangeDlg::OnRADIOAllDbcs()
{
	m_type = ALLDBCS;
	Refresh();
}

void CFname_exchangeDlg::OnRADIOAllSbcs()
{
	m_type = ALLSBCS;
	Refresh();
}

void CFname_exchangeDlg::OnRADIODelEnum() 
{
	m_type = DELNUM;
	Refresh();
}

void CFname_exchangeDlg::OnRADIODelDist() 
{
	m_type = DELDIST;
	Refresh();
	
}

void CFname_exchangeDlg::OnRadioAdd() 
{
	m_type = ADD;
	Refresh();
}

void CFname_exchangeDlg::OnRadioDel() 
{
	m_type = DEL;
	Refresh();
}

void CFname_exchangeDlg::OnRadioRep() 
{
	m_type = REP;
	Refresh();
}

void CFname_exchangeDlg::OnExecute() 
{
	// �ϐ���������
	// ��ʏ�̐ݒ�l���擾
	GetSetting();
	GetFileList();
	if( m_list_cnt == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_NO_SELECT);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	//�t�@�C�����ϊ�����
	ExChangeProc();

	//���������X�V
	m_undo.curnum++;
	m_undo.maxnum = m_undo.curnum;
	OnGetFile();

	//TODO : ����ȏ㕜���ł��Ȃ��ꍇ�ɂ�Disable�ɂ��Ȃ��ƁB�B
	GetDlgItem(IDBT_UNDO)->EnableWindow(TRUE);
}

void CFname_exchangeDlg::OnUndo() 
{
	UndoProc();
	OnGetFile();
	Refresh();
}

void CFname_exchangeDlg::Refresh() 
{
	BOOL bEnable=FALSE;
	CString str;

	//�ʂ��ԍ��t��
	bEnable = FALSE;
	if ( m_type == ENUM )
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_FIRST_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDET_FIRST_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDCH_KEEP_NAME)->EnableWindow(bEnable);
	GetDlgItem(IDCB_KETA)->EnableWindow(bEnable);

	//�w�蕶�����̍폜
	bEnable = FALSE;
	if ( m_type == DELNUM )
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_DEL_BEF_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDET_DEL_BEF_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDST_DEL_AFT_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDET_DEL_AFT_NUM)->EnableWindow(bEnable);

	//�����̒ǉ�[�擪����][�㕔����]
	bEnable = FALSE;
	if ( m_type == ADD )
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDCH_ADD_BEF)->EnableWindow(bEnable);
	GetDlgItem(IDCH_ADD_AFT)->EnableWindow(bEnable);

	//����1
	bEnable = FALSE;
	if(m_type == DEL || m_type == REP)
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_NAME1)->EnableWindow(bEnable);
	GetDlgItem(IDET_NAME1)->EnableWindow(bEnable);

	//����2
	bEnable = FALSE;
	if(m_type == ADD || m_type == REP)
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_NAME2)->EnableWindow(bEnable);
	GetDlgItem(IDET_NAME2)->EnableWindow(bEnable);

	// ����1�A����2
	switch(m_type)
	{
	case ADD :
		str.LoadString(IDSTR_NAME_ADD);
		GetDlgItem(IDST_NAME2)->SetWindowText(str);
		break;
	case DEL :
		str.LoadString(IDSTR_NAME_DEL);
		GetDlgItem(IDST_NAME1)->SetWindowText(str);
		break;
	case REP :
		str.LoadString(IDSTR_REP_BEF);
		GetDlgItem(IDST_NAME1)->SetWindowText(str);
		str.LoadString(IDSTR_NAME_REP_AFT);
		GetDlgItem(IDST_NAME2)->SetWindowText(str);
		break ;
	default :
		break ;

	// ����
	bEnable = FALSE;
	if(m_undo.curnum > 0)
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDBT_UNDO)->EnableWindow(bEnable);
	}
}

void CFname_exchangeDlg::GetFileList() 
{
	CArray <int, int> selections;

	UpdateData(TRUE);

	m_list_cnt = m_list.GetSelCount();
	selections.SetSize(m_list_cnt);
	m_list.GetSelItems((int) m_list_cnt, selections.GetData());

	m_list_cnt = selections.GetSize();

	for (int i=0; i < m_list_cnt; i++)
	{
		m_list.GetText(selections.GetAt(i), m_file_name[i]);
	}
}


void CFname_exchangeDlg::GetSetting() 
{
	m_first_num = 0;
	m_cur_num = 0;
	m_list_cnt = 0;
	m_Bef_Del_num = 0;
	m_Aft_Del_num = 0;
	m_opt=0;
	m_keta=0;
	memset(m_dir,0,sizeof(m_dir));
	memset(m_name1,0,sizeof(m_name1));
	memset(m_name2,0,sizeof(m_name2));

	GetDlgItemText(IDET_DIR, m_dir, sizeof(m_dir));

	switch(m_type)
	{
	case ENUM:
		m_first_num = GetDlgItemInt(IDET_FIRST_NUM, NULL, 0);
		m_cur_num = m_first_num;
		if(IsDlgButtonChecked(IDCH_KEEP_NAME) == BST_CHECKED){
			m_opt += KEEP;
		}
		m_keta = SendDlgItemMessage(IDCB_KETA, CB_GETCURSEL, 0, 0);
		break;

	case DELNUM:
		m_Bef_Del_num = GetDlgItemInt(IDET_DEL_BEF_NUM, NULL, 0);
		m_Aft_Del_num = GetDlgItemInt(IDET_DEL_AFT_NUM, NULL, 0);
		break;

//	case DELDIST:
//		break;

	case ADD:
		GetDlgItemText(IDET_NAME2, m_name2, sizeof(m_name2));
		if(IsDlgButtonChecked(IDCH_ADD_BEF) == BST_CHECKED){
			m_opt += BEF;
		}
		if(IsDlgButtonChecked(IDCH_ADD_AFT) == BST_CHECKED){
			m_opt += AFT;
		}
		break;

	case DEL:
		GetDlgItemText(IDET_NAME1, m_name1, sizeof(m_name1));
		break;

	case REP:
		GetDlgItemText(IDET_NAME1, m_name1, sizeof(m_name1));
		GetDlgItemText(IDET_NAME2, m_name2, sizeof(m_name2));
		break;

	default :
		break;
	}
}

void CFname_exchangeDlg::ExChangeProc()
{
	// ���X�g�R���g���[���őI�����ꂽ���������[�v
	for(int i=0; i < m_list_cnt; i++)
	{
		// �I���W�i���̃t�@�C����
		m_oname = m_dir;
		AppendPath(&m_oname, m_file_name[i]);

		//�V�����t�@�C�����擾
		switch(m_type)
		{
		case ENUM:		EnumProc();		break;	// �ʂ��ԍ��t��
		case ALLSBCS:	SbcsProc();		break;	// ���ׂĔ��p
		case ALLDBCS:	DbcsProc();		break;	// ���ׂđS�p
		case DELNUM:	DelNumProc();	break;	// �w�蕶�����폜
		case DELDIST:	DelDistProc();	break;	// �g���q�폜
		case ADD:		AddProc();		break;	// ������ǉ�
		case DEL:		DelProc();		break;	// ������폜
		case REP:		RepProc();		break;	// ������u��
		default :		break;
		}

		//�t�@�C�����ϊ�
		MoveFileProc();
	}
}

void CFname_exchangeDlg::EnumProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// �g���q
	CString num;
	UINT number = m_first_num;
	int dst_keta = GetKeta(m_first_num + m_list_cnt); //�ړI�̃P�^��
	int cur_keta;	//�J�����g�̃P�^��

	if ( dst_keta < m_keta )
	{
		dst_keta = m_keta;
	}

	// �ړI�̃t�@�C���p�X�i�f�B���N�g�����j
	m_nname = m_dir;
	m_nname += "\\";

	// ��������0���l�߂�
	if ( m_cur_num == 0 )
	{
		cur_keta = 1;
	}
	else
	{
		cur_keta = GetKeta(m_cur_num);
	}

	for(int j=0; j < dst_keta - cur_keta; j++)
	{
		m_nname += "0";
	}

	// �ړI�̃t�@�C���p�X�i�t�@�C�����j
	num.Format("%d", m_cur_num);
	m_nname += num;
	m_cur_num++;

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&file,&ext);

	// �I���W�i���̃t�@�C�������c���ꍇ
	if ( m_opt == KEEP )
	{
		m_nname += " ";
		m_nname += file;
	}

	// add distination
	AppendExt(&m_nname, ext);
}

void CFname_exchangeDlg::SbcsProc()
{
	CString dir;
	CString ofile;	// old file name
	CString nfile;	// new file name
	CString ext;	// �g���q

	LPSTR pfname = NULL;
	int len = 0;

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&ofile,&ext);

	// �ړI�̃t�@�C���p�X
	m_nname = dir;
	m_nname += "\\";

	// ������̍폜
	ReplaceString2(ofile, &nfile, TRUE);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DbcsProc()
{
	CString dir;
	CString ofile;	// old file name
	CString nfile;	// new file name
	CString ext;	// �g���q

	LPSTR pfname = NULL;
	int len = 0;

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&ofile,&ext);

	// �ړI�̃t�@�C���p�X
	m_nname = dir;
	m_nname += "\\";

	// ������̍폜
	ReplaceString2(ofile, &nfile, FALSE);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DelNumProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// �g���q
	UINT len;

	if( ! (m_Bef_Del_num || m_Aft_Del_num) )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_DEL_NUM);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&file,&ext);

	// �w�蕶�����폜�����Ƃ��̃t�@�C�������m�F
	len = strlen(file);
	if ( len < (m_Bef_Del_num + m_Aft_Del_num + DIST_MAX) )
	{
		if ( ! m_ignore_alert )
		{
			CString str,title;
			str.LoadString(IDSTR_ERR_SHORT_NAME);
			title.LoadString(IDSTR_ERROR);
			MessageBox(str,title,MB_OK);
		}
		return;
	}

	// �㕔����n�����폜
	file.Delete(len-m_Aft_Del_num,m_Aft_Del_num);

	// �擪����n�����폜
	file.Delete(0,m_Bef_Del_num);

	// �ړI�̃t�@�C���p�X�i�t�@�C�����j
	m_nname = m_dir;
	AppendPath(&m_nname,file);

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DelDistProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// �g���q

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&file,&ext);

	//�ړI�̃t�@�C����
	m_nname = dir + file;
}

void CFname_exchangeDlg::AddProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// �g���q

	// TODO : �x����GetSetting�ɂ����Ă�����
	if ( m_opt == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_SEL_INSERT);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}
	else if( strlen(m_name2) == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_INPUT_ADD_STR);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&file,&ext);

	// �ړI�̃t�@�C���p�X
	m_nname = dir;
	m_nname += "\\";

	// �擪�ɒǉ����镶����
	if( m_opt & BEF )
	{
		m_nname += m_name2;
	}
	
	//�I���W�i���̕�����
	m_nname += file;

	// �㕔�ɒǉ����镶����
	if( m_opt & AFT )
	{
		m_nname += m_name2;
	}

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DelProc()
{
	CString dir;
	CString ofile;	// file name
	CString nfile;	// �V�t�@�C����
	CString ext;	// �g���q

	// TODO: �x���͂������ւ����Ă�����
	if(strlen(m_name1) == 0)
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_INPUT_DEL_STR);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}
	
	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&ofile,&ext);

	// �ړI�̃t�@�C���p�X
	m_nname = m_dir;
	m_nname += "\\";

	// ������̍폜
	ReplaceString(ofile, &nfile, m_name1, NULL, m_comp);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::RepProc()
{
	CString dir;
	CString ofile;	// old file name
	CString nfile;	// new file name
	CString ext;	// �g���q

	// TODO: �������ւ����Ă�����
	if(strlen(m_name1) == 0 || strlen(m_name2) == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_INPUT_REP_STR);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	LPSTR pfname = NULL;
	int len = 0;

	// �p�X�𕪉�
	SplitPath(m_oname,&dir,&ofile,&ext);

	// �ړI�̃t�@�C���p�X
	m_nname = dir;
	m_nname += "\\";

	// ������̍폜
	ReplaceString(ofile, &nfile, m_name1, m_name2, m_comp);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::UndoProc()
{
	CString oname;
	CString nname;
	BOOL bReturn = TRUE;

	if(m_undo.curnum <= 0)
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_NOT_UNDO);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	m_undo.curnum--;
	for(int i=0; i < m_undo.exch[m_undo.curnum].num; i++)
	{
		oname = m_undo.exch[m_undo.curnum].ofname[i];
		nname = m_undo.exch[m_undo.curnum].nfname[i];
		bReturn = MoveFile(nname.GetString(), oname.GetString());
		if ( ! bReturn )
		{
			if ( ! m_ignore_alert )
			{
				CString str,title;
				str.Format(IDSTR_ERR_FAIL_UNDO,oname,nname);
				title.LoadString(IDSTR_ERROR);
				MessageBox(str,title,MB_OK);
			}
		}
	}
}

int CFname_exchangeDlg::GetKeta(int number)
{
	int keta;
	for( keta=0; number; keta++)
	{
		number /= 10;
	}
	return keta;
}

void CFname_exchangeDlg::MoveFileProc()
{
	int bReturn;

	// �啶������������ʂ���
	bReturn = m_oname.CompareNoCase(m_nname);
	if ( bReturn )
	{
		bReturn = MoveFile(m_oname.GetString(), m_nname.GetString());
		if ( bReturn )
		{
			int unum = m_undo.curnum;	// ���s�{�^����������
			int fnum = m_undo.exch[m_undo.curnum].num; // 1�x�ɕύX�����t�@�C���̐�

			m_undo.exch[unum].ofname[fnum] = m_oname;
			m_undo.exch[unum].nfname[fnum] = m_nname;
			if ( m_undo.exch[m_undo.curnum].num < FILE_MAX )
			{
				m_undo.exch[unum].num++;
			}
			else
			{
				CString str,title;
				int rt;

				str.LoadString(IDSTR_WRN_CACHE_FULL);
				title.LoadString(IDSTR_WRN);
				rt = MessageBox(str,title,MB_YESNO);
				if ( rt == IDYES )
				{
					InitUndo();
				}
			}
		}
		else
		{
			if ( ! m_ignore_alert )
			{
				CString str,title;
				int err = GetLastError();
				str.Format(IDSTR_ERR_FAIL_CHANGE_NAME,err,m_oname,m_nname);
				title.LoadString(IDSTR_ERROR);
				MessageBox(str,title,MB_OK);
			}
		}
	}
	else
	{
		if ( ! m_ignore_alert )
		{
			CString str,title;
			int err = GetLastError();
			str.Format(IDSTR_ERR_FAIL_OVERLAP,m_oname,m_nname);
			title.LoadString(IDSTR_ERROR);
			MessageBox(str,title,MB_OK);
		}
	}
}

void CFname_exchangeDlg::InitUndo()
{
	m_undo.curnum=0;
	m_undo.maxnum=0;
	for(int i=0; i<UNDO_MAX;i++)
	{
		m_undo.exch[i].num=0;
	}
}

void CFname_exchangeDlg::ReplaceString2(CString oname, CString *nname, BOOL bDB2SB)
{
	char old_fname[MAX_PATH];
	char new_fname[MAX_PATH];
	char tmp[MAX_PATH];
	LPSTR pfname = NULL;
	UINT cp_num=0;	//�R�s���鐔
	UINT cp_start=0;//�R�s����擪�z��
	BOOL rt = FALSE;

	memset(new_fname, 0, sizeof(new_fname));
	memset(tmp, 0, sizeof(tmp));

	strcpy_s(old_fname,sizeof(old_fname),(LPSTR)oname.GetString());	// �I���W�i���t�@�C�������R�s
	if ( bDB2SB )
	{
		zen2han(old_fname);
	}
	else
	{
		han2zen(old_fname);
	}

	strcat_s(new_fname, sizeof(new_fname), old_fname);	//�ŏI�I�ȖړI�̕�����
	*nname = new_fname;

#if 0
	char old_fname[MAX_PATH];
	char new_fname[MAX_PATH];
	char tmp[MAX_PATH];
	LPSTR pfname = NULL;
	UINT cp_num=0;	//�R�s���鐔
	UINT cp_start=0;//�R�s����擪�z��
	BOOL rt = FALSE;

	memset(new_fname, 0, sizeof(new_fname));
	memset(tmp, 0, sizeof(tmp));

	strcpy_s(old_fname,sizeof(old_fname),(LPSTR)oname.GetString());	// �I���W�i���t�@�C�������R�s
	if ( bDB2SB )
	{
		for(UINT i=0; i<_tcslen(old_fname); i++)	// 1����������
		{
			for(UINT j=0; j<_tcslen(dbcs); j++)		// 1����������
			{
				rt = strncmp(&old_fname[i], &dbcs[j], 2);	// �w�蕶�����ŖړI�̕����������

				if(rt == 0)	// �w�蕶�����ŖړI�̕����������
				{
					strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//������O�܂ł̕�����
					strcat_s(new_fname, sizeof(new_fname), tmp);	//�ŏI�I�ȖړI�̕�����
					strcat_s(new_fname, sizeof(new_fname), &sbcs[j]);

					cp_num=0;	//�L���b�V���̃J�E���g�����Z�b�g����
					cp_start = i + 1;
				}
				else
				{
					cp_num++;
				}
			}
		}
	}
	else // bDBCS
	{
		for(UINT i=0; i<_tcslen(old_fname); i++)	// 1����������
		{
			for(UINT j=0; j<_tcslen(sbcs); j++)		// 1����������
			{
				rt = strncmp(&old_fname[i], &sbcs[j], 1);	// �w�蕶�����ŖړI�̕����������

				if(rt == 0)	// �w�蕶�����ŖړI�̕����������
				{
					strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//������O�܂ł̕�����
					strcat_s(new_fname, sizeof(new_fname), tmp);	//�ŏI�I�ȖړI�̕�����
					strcat_s(new_fname, sizeof(new_fname), &dbcs[j]);

					cp_num=0;	//�L���b�V���̃J�E���g�����Z�b�g����
					cp_start = i + 1;
				}
				else
				{
					cp_num++;
				}
			}
		}
	}

	//for(UINT i=0; i<strlen(old_fname); i++)	// 1����������
	//{
	//	if ( bDiff )
	//	{
	//		rt = strncmp(&old_fname[i], srch, strlen(srch));	// �w�蕶�����ŖړI�̕����������
	//	}
	//	else
	//	{
	//		rt = _strnicmp(&old_fname[i], srch, strlen(srch));	// �w�蕶�����ŖړI�̕����������
	//	}

	//	if(rt == 0)	// �w�蕶�����ŖړI�̕����������
	//	{
	//		strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//������O�܂ł̕�����
	//		strcat_s(new_fname, sizeof(new_fname), tmp);	//�ŏI�I�ȖړI�̕�����
	//		if(rep != NULL)
	//		{
	//			// �u���Ώە���������Ȃ�A�����Ŏ��s
	//			strcat_s(new_fname, sizeof(new_fname), rep);
	//		}
	//		i += strlen(srch) - 1; //�݂����������񕪂̌����͔�΂�
	//		cp_num=0;	//�L���b�V���̃J�E���g�����Z�b�g����
	//		cp_start = i + 1;
	//	}
	//	else
	//	{
	//		cp_num++;
	//	}
	//}

	//// �Ώە�����ȍ~�̕�����A��
	//if ( cp_num )
	//{
	//	strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//������O�܂ł̕�����
	//	strcat_s(new_fname, sizeof(new_fname), tmp);	//�ŏI�I�ȖړI�̕�����
	//}
	//*nname = new_fname;
#endif
}
