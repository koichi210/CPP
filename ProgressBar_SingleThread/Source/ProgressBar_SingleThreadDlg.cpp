
// ProgressBar_SingleThreadDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ProgressBar_SingleThread.h"
#include "ProgressBar_SingleThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgressBar_SingleThreadDlg �_�C�A���O




CProgressBar_SingleThreadDlg::CProgressBar_SingleThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressBar_SingleThreadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressBar_SingleThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ddx_progress);
}

BEGIN_MESSAGE_MAP(CProgressBar_SingleThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CProgressBar_SingleThreadDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CProgressBar_SingleThreadDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CProgressBar_SingleThreadDlg ���b�Z�[�W �n���h���[

BOOL CProgressBar_SingleThreadDlg::OnInitDialog()
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

void CProgressBar_SingleThreadDlg::OnPaint()
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
HCURSOR CProgressBar_SingleThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProgressBar_SingleThreadDlg::OnBnClickedStart()
{
	// �v���O���X�o�[�͈͐ݒ�
	m_ddx_progress.SetRange32(0, PROGRESS_MAX -1);

	for (int i = 0; i < PROGRESS_MAX; i++)
	{
		// �v���O���X�o�[���݈ʒu�ݒ�
		m_ddx_progress.SetPos(i);
	}
	m_ddx_progress.SetPos(0);
}

void CProgressBar_SingleThreadDlg::OnBnClickedStop()
{
	// SingleThread�Ȃ̂ŁA�������ɌĂ΂�Ȃ��B
	MessageBox("�����𒆒f���܂��B");
}
