
// PointerDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Pointer.h"
#include "PointerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPointerDlg �_�C�A���O




CPointerDlg::CPointerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPointerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPointerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPointerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPointerDlg ���b�Z�[�W �n���h���[

BOOL CPointerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	m_test.param1 = 1;
	m_test.param2 = 20;
	m_test.param3 = 300;

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CPointerDlg::OnPaint()
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
HCURSOR CPointerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPointerDlg::OnBnClickedButton1()
{
	TEST_T*	pTest = NULL;
	TEST_T*	pTest2;

	GetParam(&pTest);
	pTest2 = GetParam();

	CString AddrStr;
	AddrStr.Format(
		"Addr  m_test�{��\t\t = %08x\n"
		"Addr  pTest���w����\t = %08x\n"
		"Addr  pTest2���w����\t = %08x\n"
		"Addr  pTest�{��\t\t = %08x\n"
		"Addr  pTest2�{��\t\t = %08x\n\n"

		"Data  m_test.param2\t = %08x\n" 
		"Data  pTest->param2\t = %08x\n"
		"Data  pTest2->param2\t = %08x\n" ,
		&m_test,
		pTest,
		pTest2,

		&pTest,
		&pTest2,

		m_test.param2,
		pTest->param2,
		pTest2->param2);

	MessageBox(AddrStr);
}

void CPointerDlg::GetParam(TEST_T** p )
{
	*p = &m_test;
}

TEST_T* CPointerDlg::GetParam( )
{
	return &m_test;
}
