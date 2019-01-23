// StrMathDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "StrMath.h"
#include "StrMathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStrMathDlg �_�C�A���O

CStrMathDlg::CStrMathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStrMathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStrMathDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStrMathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrMathDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStrMathDlg, CDialog)
	//{{AFX_MSG_MAP(CStrMathDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_SUM, OnSum)
	ON_BN_CLICKED(IDC_SUB, OnSub)
	ON_BN_CLICKED(IDC_2KETA, On2keta)
	ON_BN_CLICKED(IDC_3KETA, On3keta)
	ON_BN_CLICKED(IDC_ANS, OnAns)
	ON_BN_CLICKED(IDC_HLP, OnHlp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrMathDlg ���b�Z�[�W �n���h��

BOOL CStrMathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	CheckDlgButton(IDC_2KETA, BST_CHECKED);
	CheckDlgButton(IDC_SUM, BST_CHECKED);
	keta = 2;
	mark = SUM;
	num1 = 0;
	num2 = 0;
	srand((unsigned)time(NULL));

	{
		static HFONT font;	//�\�������\����
		LOGFONT viewfont;	//�\�������\����

		::memset(&viewfont, 0, sizeof(viewfont));
		//�\���t�H���g�쐬
		viewfont.lfCharSet = DEFAULT_CHARSET;
		viewfont.lfWeight = SHOW_WEIGHT;
		viewfont.lfHeight = SHOW_HEIGHT;
		font = CreateFontIndirect(&viewfont);
		SendDlgItemMessage(IDC_VALUE1, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(IDC_VALUE2, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(IDC_MARK, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	}

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CStrMathDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CStrMathDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStrMathDlg::OnOK() 
{
	CDialog::OnOK();
}

void CStrMathDlg::OnStart() 
{
	int num;
	char  str[STR_BUFF];
	num = 345;
	memset(str, 0, sizeof(str));

	if(mark == SUM){
		GetDlgItem(IDC_MARK)->SetWindowText("+");
	}else if(mark == SUB){
		GetDlgItem(IDC_MARK)->SetWindowText("-");
	}

	SetDlgItemText(IDC_IANS, "");

	BuiltNumber(&num1);
	BuiltNumber(&num2);

	if(mark == SUB && num1 < num2){
		int tmp;
		tmp = num1;
		num1 = num2;
		num2 = tmp;
	}

	GetString(num1, str);
	SetDlgItemText(IDC_VALUE1, str);
	GetString(num2, str);
	SetDlgItemText(IDC_VALUE2, str);
}

void CStrMathDlg::BuiltNumber(int * num)
{
	*num = rand();

	if(*num < 1000){
		BuiltNumber(num);
	}else{
		if(keta == 2){
			*num %= 100;
		}else if(keta == 3){
			*num %= 1000;
		}
	}
}

void GetString(int num, LPSTR str)
{
	int val;
	memset(str, 0, sizeof(str));

	// 100
	{
		val = num / 100;
		if(val != 1){
			GetString2(val, str, TRUE);
		}

		switch(val){
		case 2:
			// no braek
		case 4:
			// no braek
		case 5:
			// no braek
		case 7:
			// no braek
		case 9:
			strcat(str, DEF100A);
			break;

		case 3:
			strcat(str, DEF100B);
			break;

		case 6:
			// no braek
		case 8:
			strcat(str, DEF100C);
			break;

		default :
			break;
		}
	}

	// 10
	{
		num %= 100;
		val = num / 10;

		if(val != 1){
			GetString2(val, str, FALSE);
		}

		if(val != 0){
			strcat(str, DEF10);
		}
	}

	// 1
	{
		val = num % 10;
	}

	GetString2(val, str, FALSE);
}

void GetString2(int num, LPSTR str, BOOL flg)
{
	switch(num){
	case 1:
		strcat(str, DEF1);
		break;

	case 2:
		strcat(str, DEF2);
		break;

	case 3:
		strcat(str, DEF3);
		break;

	case 4:
		strcat(str, DEF4);
		break;

	case 5:
		strcat(str, DEF5);
		break;

	case 6:
		if(flg){
			strcat(str, DEF6B);
		}else{
			strcat(str, DEF6A);
		}
		break;

	case 7:
		strcat(str, DEF7);
		break;

	case 8:
		if(flg){
			strcat(str, DEF8B);
		}else{
			strcat(str, DEF8A);
		}
		break;

	case 9:
		strcat(str, DEF9);
		break;

	default:
		break;
	}
}	

void CStrMathDlg::OnSum() 
{
	mark = SUM;
}

void CStrMathDlg::OnSub() 
{
	mark = SUB;
}

void CStrMathDlg::On2keta() 
{
	keta = 2;
}

void CStrMathDlg::On3keta() 
{
	keta = 3;	
}

void CStrMathDlg::OnAns() 
{
	int ans;
	int ians;
	char str[STR_BUFF];

	memset(str, 0, sizeof(str));

	if(num1 && num2){
		// ����
		if(mark == SUM){
			ans = num1 + num2;
		}else{
			ans = num1 - num2;
		}

		// ���͒l
		ians = GetDlgItemInt(IDC_IANS, NULL, 0);

		if(ans == ians){
			sprintf(str, "�����I�I");
		}else{
			sprintf(str, "�c�O�B�B", ans);
		}
		MessageBox(str, "��", MB_OK);
	}else{
		MessageBox("�X�^�[�g�������ĉ������B", "Caution", MB_OK);
	}
}

void CStrMathDlg::OnHlp() 
{
	char str[STR_BUFF];

	memset(str, 0, sizeof(str));
	sprintf(str, "�Ђ炪�Ȃő����Z�^�����Z�����܂�\n\n"
		"1 �o�茅����I��\n"
		"2 ���Z��ʂ�I��\n"
		"3 �X�^�[�g�t�����ɂ��o�肳���\n"
		"����ɂ��]���b�����܂��I�I�I\n");
	MessageBox(str, "�w���v", MB_OK);
}
