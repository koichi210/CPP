
// ProgressBar_MultiThreadDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ProgressBar_MultiThread.h"
#include "ProgressBar_MultiThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgressBarDlg �_�C�A���O




CProgressBarDlg::CProgressBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressBarDlg::IDD, pParent)
	, m_StatusBar(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ddx_progress);
	DDX_Text(pDX, IDST_STATUS_BAR, m_StatusBar);
}

BEGIN_MESSAGE_MAP(CProgressBarDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CProgressBarDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CProgressBarDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CProgressBarDlg ���b�Z�[�W �n���h���[

BOOL CProgressBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// �v���O���X�o�[�͈͐ݒ�
	m_ddx_progress.SetRange32(0, PROGRESS_MAX -1);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CProgressBarDlg::OnPaint()
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
HCURSOR CProgressBarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProgressBarDlg::OnBnClickedStart()
{
	m_stop = FALSE;
	m_StatusBar.SetString("Start �������ꂽ��");
	UpdateData(false);
	AfxBeginThread(ProcThread, this);
}

void CProgressBarDlg::OnBnClickedStop()
{
	m_stop = TRUE;
	m_StatusBar.SetString("Stop �������ꂽ��");
	UpdateData(false);
}

UINT ProcThread(LPVOID pParam)
{
	CProgressBarDlg* pDlg = (CProgressBarDlg*)pParam;
	int cnt=0;
	CString str;

	for (int i = 0; i < PROGRESS_MAX; i++)
	{
		if ( pDlg->m_stop )
		{
			break;
		}
		// �v���O���X�o�[���݈ʒu�ݒ�
		pDlg->m_ddx_progress.SetPos(i);
	}
//	pDlg->m_ddx_progress.SetPos(0);
	return TRUE;
}
