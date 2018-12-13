
// DivisionCouplingDlg.cpp : �����t�@�C��
//

#include <string> 

#include "stdafx.h"
#include "DivisionCoupling.h"
#include "DivisionCouplingDlg.h"
#include "afxdialogex.h"
#include "StandardTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDivisionCouplingDlg �_�C�A���O
CDivisionCouplingDlg::CDivisionCouplingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDivisionCouplingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDivisionCouplingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDPR_SPLITMERGE, mdx_progress);
}

BEGIN_MESSAGE_MAP(CDivisionCouplingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_SPLIT, OnBnClickedSplit)
	ON_BN_CLICKED(IDBT_MERGE, OnBnClickedMerge)
	ON_BN_CLICKED(IDBT_SPLIT_BROWSE, OnBnClickedSplitBrowse)
	ON_BN_CLICKED(IDBT_MERGE_BROWSE, OnBnClickedMergeBrowse)
	ON_BN_CLICKED(IDBT_STOP, OnBnClickedStop)
END_MESSAGE_MAP()


// CDivisionCouplingDlg ���b�Z�[�W �n���h���[

BOOL CDivisionCouplingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CDivisionCouplingDlg::OnPaint()
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
HCURSOR CDivisionCouplingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDivisionCouplingDlg::OnBnClickedSplit()
{
	GetDlgItem(IDET_SPLIT_FNAME)->GetWindowText(m_fNameOrg);
	m_fDivSize = GetDlgItemInt(IDET_SIZE);
	m_fDivSize *= UNIT_VALUE;
	Initialize();
	AfxBeginThread(SplitProcThread, this);
}

void CDivisionCouplingDlg::OnBnClickedMerge()
{
	GetDlgItem(IDET_MERGE_FNAME)->GetWindowText(m_fNameNew);
	Initialize();
	AfxBeginThread(MergeProcThread, this);
}

void CDivisionCouplingDlg::Initialize()
{
	// GUI����ݒ�l���擾
	m_nIdx = 1;
	m_Err = NO_ERROR;
	m_bProc = TRUE;
	EnableControl(m_bProc);
}

void CDivisionCouplingDlg::Result()
{
	m_bProc = FALSE;
	EnableControl(m_bProc);

	if ( m_Err != NO_ERROR )
	{
		// �G���[���b�Z�[�W�쐬
		CString msg;
		switch(m_Err)
		{
		case ERR_OPEN_ORGFILE :
			msg.Format("���t�@�C���I�[�v���G���[");
			break;

		case ERR_OPEN_NEWFILE :
			msg.Format("�t�@�C�����쐬�ł��܂���\n\n%s" ,m_fNameNew);
			break;

		case ERR_CALLOC :
			msg.Format("Work�������m�ۃG���[");
			break;

		case ERR_DIV_SIZE :
			msg.Format("�����T�C�Y�G���[");
			break;

		default :
			break;
		}

		MessageBox(msg , "error", MB_OK);
	}
}

void CDivisionCouplingDlg::SplitProc()
{
	if ( m_fDivSize <= 0 )
	{
		m_Err = ERR_DIV_SIZE;
		return;
	}

	if ( ! m_cstOrgFile.Open(m_fNameOrg, CFile::typeBinary) )
	{
		m_Err = ERR_OPEN_ORGFILE;
		return;
	}

	char *buff;	// �ǂݍ��݃o�b�t�@�i�����ő�T�C�Y�Ŋm�ہj
	buff = (char *)calloc(m_fDivSize, sizeof(char));
	if ( ! buff )
	{
		m_Err = ERR_CALLOC;
		return;
	}

	// �c��T�C�Y�̏����l�́A���t�@�C���̃T�C�Y
	m_fRestSize = (size_t)m_cstOrgFile.GetLength();

	// �v���O���X�o�[�Z�b�g
	SetProgressSplitFileNum();
	mdx_progress.SetRange32(0, GetProgressBarEnd());

	int szDivSize;
	while( m_fRestSize > 0 && m_bProc )
	{
		m_fNameNew.Format("%s.div%03d", m_fNameOrg, m_nIdx);
		if ( ! m_cstNewFile.Open(m_fNameNew, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
		{
			m_Err = ERR_OPEN_NEWFILE;
			break;
		}

		// ����̕����T�C�Y������
		if ( m_fRestSize > m_fDivSize )
		{
			szDivSize = m_fDivSize;
		}
		else
		{
			szDivSize = m_fRestSize;
		}
		m_fRestSize -= szDivSize;

		// �o�C�i���f�[�^��Read&Write
		m_cstOrgFile.Read(buff, szDivSize);
		m_cstNewFile.Write(buff, szDivSize);
		m_cstNewFile.Close();

		// �v���O���X�o�[�̍X�V
		UpdateProgressBar(m_nIdx);

		// �t�@�C�����̃C���f�b�N�X
		m_nIdx++;
	}

	free(buff);
	m_cstOrgFile.Close();
}

void CDivisionCouplingDlg::MergeProc()
{
	std::string fullpath = m_fNameNew;

	// ex) fname.jpg.div001�@���@fname.jpg
	int idx = fullpath.find_last_of(".");
	std::string pathname = fullpath.substr(0, idx);
	m_fNameNew = pathname.c_str();

	if ( PathFileExists(m_fNameNew))
	{
		int MsgResult = MessageBox("���łɃt�@�C�������݂��܂��B�㏑�����܂����H\n" + m_fNameNew,"Warning", MB_YESNO);
		if ( MsgResult == IDNO)
		{
			MessageBox("�����𒆒f���܂����B");
			return;
		}
	}

	if ( ! m_cstNewFile.Open(m_fNameNew, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
	{
		m_Err = ERR_OPEN_NEWFILE;
		return;
	}

	char *buff;	// �ǂݍ��݃o�b�t�@
	int szDivSize;
	while( m_bProc )
	{
		m_fNameOrg.Format("%s.div%03d", m_fNameNew, m_nIdx);
		if ( ! m_cstOrgFile.Open(m_fNameOrg, CFile::typeBinary) )
		{
			// �ǂݍ��ރt�@�C���������Ȃ�����I��
			break;
		}

		szDivSize = (size_t)m_cstOrgFile.GetLength();
		buff = (char *)calloc(szDivSize, sizeof(char));
		if ( ! buff )
		{
			m_Err = ERR_CALLOC;
			break;
		}

		// �o�C�i���f�[�^��Read&Write
		m_cstOrgFile.Read(buff, szDivSize);
		m_cstNewFile.Write(buff, szDivSize);
		m_cstOrgFile.Close();

		// TODO�F��������t�@�C���S�����擾����K�v������
		// �v���O���X�o�[�̍X�V
		//UpdateProgressBar(m_nIdx);

		m_nIdx++;
		free(buff);
	}

	m_cstNewFile.Close();
}

void CDivisionCouplingDlg::OnBnClickedSplitBrowse()
{
	OnBrowse(IDET_SPLIT_FNAME, TRUE);
}

void CDivisionCouplingDlg::OnBnClickedMergeBrowse()
{
	OnBrowse(IDET_MERGE_FNAME, TRUE);
}

void CDivisionCouplingDlg::OnBrowse(UINT nID, BOOL bIsFileOpen)
{
	// �t�@�C�����J��
	CFileDialog *dlgFile;

	dlgFile = new CFileDialog(bIsFileOpen, "*.*", NULL, OFN_CREATEPROMPT, "*.*|*.*|�S��(*.*)|*.*||");
	if ( dlgFile->DoModal() == IDOK )
	{
		CString strFileName;
		strFileName = dlgFile->GetPathName();
		GetDlgItem(nID)->SetWindowText(strFileName);
	}
}

void CDivisionCouplingDlg::EnableControl(BOOL bEnable)
{
	GetDlgItem(IDBT_SPLIT)->EnableWindow(!bEnable);
	GetDlgItem(IDBT_MERGE)->EnableWindow(!bEnable);
	GetDlgItem(IDBT_STOP)->EnableWindow(bEnable);
}

void CDivisionCouplingDlg::UpdateProgressBar(int nPos)
{
	// Start�ʒu�AEnd�ʒu��ݒ�
	int start,end;
	switch(nPos)
	{
	case POS_INIT :
		start = 0;
		end = 0;
		break;

	case POS_END :
		start = GetProgressBarEnd();
		end = GetProgressBarEnd();
		break;

	default :
		start = nPos;
		end = GetProgressBarEnd();
		break;
	}

	// �v���O���X�o�[�̌��݈ʒu�ݒ�
	mdx_progress.SetPos(start);
}

int CDivisionCouplingDlg::GetProgressBarEnd()
{
	return m_MaxNum;
}

void CDivisionCouplingDlg::SetProgressSplitFileNum()
{
	m_MaxNum = m_fRestSize / m_fDivSize;
	if ( m_fRestSize % m_fDivSize)
	{
		m_MaxNum++;
	}
}

void CDivisionCouplingDlg::SetProgressMergeFileNum()
{
	m_MaxNum = 1;
	while( TRUE )
	{
		m_fNameOrg.Format("%s.div%03d" ,m_fNameNew, m_MaxNum);
		if ( ! m_cstOrgFile.Open(m_fNameOrg, CFile::typeBinary) )
		{
			break;
		}
		m_cstOrgFile.Close();
		m_MaxNum++;
	}
}

UINT SplitProcThread(LPVOID pParam)
{
	CDivisionCouplingDlg *pDlg = (CDivisionCouplingDlg*)pParam;
	pDlg->SplitProc();
	pDlg->Result();
	return TRUE;
}

UINT MergeProcThread(LPVOID pParam)
{
	CDivisionCouplingDlg *pDlg = (CDivisionCouplingDlg*)pParam;
	pDlg->MergeProc();
	pDlg->Result();
	return TRUE;
}

void CDivisionCouplingDlg::OnBnClickedStop()
{
	m_bProc = FALSE;
	EnableControl(m_bProc);
}
