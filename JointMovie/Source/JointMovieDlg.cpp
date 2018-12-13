// JointMovieDlg.cpp : �����t�@�C��
//

#include <afxtempl.h>
#include "stdafx.h"
#include "JointMovie.h"
#include "JointMovieDlg.h"

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


// CJointMovieDlg �_�C�A���O


CJointMovieDlg::CJointMovieDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJointMovieDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJointMovieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJointMovieDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OUT_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN1_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN2_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN3_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN4_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN5_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN6_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN7_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_IN8_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_EXECUTE, OnExecute)
END_MESSAGE_MAP()


// CJointMovieDlg ���b�Z�[�W �n���h��

BOOL CJointMovieDlg::OnInitDialog()
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

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CJointMovieDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CJointMovieDlg::OnPaint()
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
HCURSOR CJointMovieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CJointMovieDlg::OnBrowse()
{
	const MSG *pMsg = GetCurrentMessage();

	if ( pMsg )
	{
		CString	strFileType;
		char	szFileNames[MAX_PATH]="\0";
		int		idCtrl = LOWORD(pMsg->wParam);
		int		idxCtrl = idCtrl - IDC_IN1_BROWSE;	// �I�t�Z�b�g�擾

		idxCtrl += IDC_INPUTFILE1;	// �ݒ肷��G�f�B�b�g�R���g���[��ID���Z�o

		strFileType.Format("����i*.mpg; *.mpeg;�j|*.mpg; *.mpeg;|���ׂĂ�̧�� �i*.*�j|*.*||");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFileType, this);
		dlg.GetOFN().lpstrFile = szFileNames;
		dlg.GetOFN().nMaxFile = sizeof(szFileNames) / sizeof(char);
		if ( dlg.DoModal() == IDOK )
		{
			SetDlgItemText(idxCtrl, szFileNames);
		}
	}
}


void CJointMovieDlg::OnExecute()
{
	CString org_fname;
	CString new_fname;
	CString tmp;
	BOOL rt = FALSE;

	GetDlgItem(IDC_OUTPUTFILE)->GetWindowText(new_fname);
	for(int i=0;i < GROUP_MAX;i++)
	{
		GetDlgItem(IDC_INPUTFILE1 + i)->GetWindowText(tmp);
		if ( tmp != "" )
		{
			if ( org_fname != "" )
			{
				org_fname += "+";
			}
			org_fname += tmp;
		}
	}

	if ( new_fname == "" )
	{
		MessageBox("������t�@�C�������s��");
	}
	else if ( org_fname == "" )
	{
		MessageBox("���t�@�C�������s��");
	}
	else
	{
		char command[MAX_PATH];

		sprintf_s(command, sizeof(command),"copy /B /-Y %s %s",org_fname,new_fname);
		rt = system(command);
	}
}
