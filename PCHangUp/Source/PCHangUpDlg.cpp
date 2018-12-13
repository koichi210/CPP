
// PCHangUpDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "PCHangUp.h"
#include "PCHangUpDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ******** ������L���ɂ����PC���n���O���܂��B ********
//#define PC_HANG_UP	


// CPCHangUpDlg �_�C�A���O
CPCHangUpDlg::CPCHangUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCHangUpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCHangUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPCHangUpDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPCHangUpDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPCHangUpDlg ���b�Z�[�W �n���h���[

BOOL CPCHangUpDlg::OnInitDialog()
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

void CPCHangUpDlg::OnPaint()
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
HCURSOR CPCHangUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPCHangUpDlg::OnBnClickedButton1()
{
	BOOL bExec = FALSE;	

#ifdef PC_HANG_UP
	bExec = TRUE;
#endif

	if ( bExec ) 
	{
		MessageBox("PC���n���O���܂�");
	}
	else
	{
		MessageBox("PC���n���O������ꍇ�A���L�}�N����L���ɂ��Ă�������\n   PC_HANG_UP");
	}

	while ( bExec )
	{
		AfxBeginThread(ProcThread, this);	
	}
}

UINT ProcThread(LPVOID pParam)
{
	// �������X���b�h�Ŏ��s����鏈��
	// �d����������������PC���ׂ����債�Ă���
	return TRUE;
}