
// TemplateDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Template.h"
#include "TemplateDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;
 
//  �e���v���[�g�֐�
template <typename T>
T add(T x, T y){
    return x + y;
}

// CTemplateDlg �_�C�A���O
CTemplateDlg::CTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTemplateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTemplateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CTemplateDlg ���b�Z�[�W �n���h���[

BOOL CTemplateDlg::OnInitDialog()
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

void CTemplateDlg::OnPaint()
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
HCURSOR CTemplateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTemplateDlg::OnBnClickedButton1()
{
	// �֐��̌��Ɍ^���L��
	char Msg[MAX_PATH] = "";
	char AddStr[MAX_PATH] = "";

	sprintf(AddStr, "%s + %s = %s\n", "ABC", "def", add<string>("ABC", "def"));	// string�𖾎��I�Ɏw��
	sprintf(Msg, "%s%s", Msg, AddStr);

	sprintf(AddStr, "%d + %d = %d\n", 12, 34, add<int>(12, 34));				// int�𖾎��I�Ɏw��
	sprintf(Msg, "%s%s", Msg, AddStr);

	sprintf(AddStr, "%d + %d = %d\n", 5, 6, add(5, 6));							// int�̏ꍇ�A�w��ȗ��\
	sprintf(Msg, "%s%s", Msg, AddStr);

	MessageBox(Msg);
}
