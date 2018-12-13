
// VariableArgumentDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "VariableArgument.h"
#include "VariableArgumentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVariableArgumentDlg �_�C�A���O




CVariableArgumentDlg::CVariableArgumentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVariableArgumentDlg::IDD, pParent)
	, m_Input(_T(""))
	, m_Replace(_T(""))
	, m_Output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVariableArgumentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_Input);
	DDX_Text(pDX, IDC_EDIT_REPLACE, m_Replace);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_Output);
}

BEGIN_MESSAGE_MAP(CVariableArgumentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC_C, OnBnClickedButtonExecC)
	ON_BN_CLICKED(IDC_BUTTON_EXE_CPP, OnBnClickedButtonExeCpp)
END_MESSAGE_MAP()


// CVariableArgumentDlg ���b�Z�[�W �n���h���[

BOOL CVariableArgumentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	m_Input = "Filename_%d.bin";
	m_Replace = "1";
	UpdateData(FALSE);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CVariableArgumentDlg::OnPaint()
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
HCURSOR CVariableArgumentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVariableArgumentDlg::OnBnClickedButtonExecC()
{
	char Input[256] = "";
	char Output[256] = "";
	int Replace = 0;

	UpdateData(TRUE);

	strcpy(Input, m_Input);
	Replace = atoi(m_Replace);
	strcpy(Output, m_Output);
	sprintf(Output, Input, Replace);

	m_Output = Output;
	UpdateData(FALSE);
}

void CVariableArgumentDlg::OnBnClickedButtonExeCpp()
{
	UpdateData(TRUE);
	m_Output = m_Input;
	m_Output.Replace("%d", m_Replace);
	UpdateData(FALSE);
}
