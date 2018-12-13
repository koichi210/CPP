
// SplitPathOwnDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "SplitPathOwn.h"
#include "SplitPathOwnDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSplitPathOwnDlg �_�C�A���O




CSplitPathOwnDlg::CSplitPathOwnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplitPathOwnDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSplitPathOwnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplitPathOwnDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSplitPathOwnDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSplitPathOwnDlg ���b�Z�[�W �n���h���[

BOOL CSplitPathOwnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

void CSplitPathOwnDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CSplitPathOwnDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CSplitPathOwnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSplitPathOwnDlg::OnBnClickedButton1()
{
	char FileFullPath[256] = "C:/Windows/System32/explorer.exe";
	char drive[256] = "";       // Drive     : Output
	char dir[256] = "";         // Directory : Output
	char fname[256] = "";       // Filename  : Output
	SplitPath( FileFullPath, drive, dir, fname );
	
	CString Result;
	Result.Format("org=%s\n\n drv=%s\n dir=%s\n fname=%s",
			FileFullPath,  // Path Input
			drive,       // Drive     : Output
			dir,         // Directory : Output
			fname       // Filename  : Output
			);
	MessageBox(Result);
}

void CSplitPathOwnDlg::SplitPath( char *pFileFullPath, char *pDrive, char *pDir, char *pFile )
{
	char *pt;

	/* �h���C�u���擾 */
	{
		pt = pFileFullPath;	/* �|�C���^��擪�Ɉړ�*/
		while(*pt != '/')
		{
			pt++;
		}
		strncpy(pDrive, pFileFullPath, strlen(pFileFullPath) - strlen(pt) + 1);	// �u+1�v�͏I�[�́u/�v��ǉ�
	}

	/* �t�@�C�����擾 */
	{
		pt = pFileFullPath + strlen(pFileFullPath);	/* �|�C���^���I�[�Ɉړ� */
		while(*pt != '/')
		{
			pt--;
		}
		pt++;	// �擪�́u/�v���폜
		strncpy(pFile, pt, strlen(pt));
	}

	/* �f�B���N�g�����擾 */
	{
		pt = &pFileFullPath[strlen(pDrive)];	/* �|�C���^���h���C�u���^�[�̌��Ɉړ� */
		strncpy(pDir, pt, strlen(pt) - strlen(pFile));
	}
}
