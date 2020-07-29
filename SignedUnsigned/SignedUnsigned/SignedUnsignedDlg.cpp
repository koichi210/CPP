
// SignedUnsignedDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "SignedUnsigned.h"
#include "SignedUnsignedDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSignedUnsignedDlg �_�C�A���O




CSignedUnsignedDlg::CSignedUnsignedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignedUnsignedDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignedUnsignedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSignedUnsignedDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSignedUnsignedDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSignedUnsignedDlg ���b�Z�[�W �n���h���[

BOOL CSignedUnsignedDlg::OnInitDialog()
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

void CSignedUnsignedDlg::OnPaint()
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
HCURSOR CSignedUnsignedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSignedUnsignedDlg::OnBnClickedButton1()
{
	UINT32	BitDepthValue		= 8;
	UINT32	YOffset				= 1;
	UINT32	BaseWidth			= 1;
	UINT32	XOffset				= 1;
	UINT32	Compass_Hcopy_num	= 18;
	UINT32	Pix2ByteDen			= 1;

	UINT32	CompIn1Offset = (( YOffset * BaseWidth + XOffset ) - Compass_Hcopy_num ) * BitDepthValue / Pix2ByteDen;
	CString str;
	str.Format("Result = %d", CompIn1Offset );
	MessageBox(str);
}
