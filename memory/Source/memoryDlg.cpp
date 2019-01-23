// memoryDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "memory.h"
#include "memoryDlg.h"
#include "AnserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryDlg �_�C�A���O

CMemoryDlg::CMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemoryDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoryDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	DDX_Control(pDX, IDC_VIEW_SPEED, cyc_bar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMemoryDlg, CDialog)
	//{{AFX_MSG_MAP(CMemoryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_ANS, OnAns)
	ON_BN_CLICKED(IDC_KEISAN, OnKeisan)
	ON_BN_CLICKED(IDC_ANKI, OnAnki)
	ON_BN_CLICKED(IDC_ENG_SMALL, OnEngSmall)
	ON_BN_CLICKED(IDC_ENG_LARGE, OnEngLarge)
	ON_BN_CLICKED(IDC_NUMBER, OnNumber)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryDlg ���b�Z�[�W �n���h��

BOOL CMemoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str ;

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	timer_cyc = CYC_INIT_VAL ;
	new_mode = ANKI ;
	type = NULL ;

	GetDlgItem(IDCANCEL)->ShowWindow(FALSE) ;
	GetDlgItem(IDC_ANS)->EnableWindow(FALSE) ;
	SetDlgItemInt(IDC_PR_NUM,NUM_INIT_VAL,0) ;
	SetDlgItemInt(IDC_PR_KETA,KETA_INIT_VAL,0) ;

	if(TYPE_INIT_VAL == ANKI){
		CheckDlgButton(IDC_ANKI,TRUE) ;
		OnAnki() ;
	}else{
		CheckDlgButton(IDC_KEISAN,TRUE) ;
		OnKeisan() ;
	}

	states = INIT ;
	cyc_bar.SetScrollRange(MIN_CYC,MAX_CYC,TRUE) ;
	cyc_bar.SetScrollPos(CYC_INIT_VAL);
	str.Format("(%d ms)",timer_cyc);
	GetDlgItem(IDC_SPEED_TXT)->SetWindowText(str) ;
	InitProc() ;

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CMemoryDlg::OnPaint() 
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
HCURSOR CMemoryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMemoryDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == GENERATE_ID){
		ViewText() ;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMemoryDlg::ViewText()
{
	char str[KETA_ANIKI_MAX] ;

	::memset(&str,0,sizeof(str)) ;

	if(count < num){
		KeyGen(str) ;
	}else{
		EndProc() ;
	}
	GetDlgItem(IDC_SHOW)->SetWindowText(str);
}

void CMemoryDlg::OnStart() 
{
	int rt ;
	char str[STR_BUFF] ;

	::memset(&str,0,sizeof(str)) ;

	if(type != 0){
		rt = StartCheck() ;
		if(rt == NORMAL){
			InitProc() ;
			StartProc() ;
		}else{
			sprintf(str,"���͒l�G���[�ł��B\n\n") ;
			if(rt & NUM_ERR){
				strcat(str,"����ȏo�萔����͂��ĉ������B\n") ;
			}

			if(rt & KETA_ERR){
				strcat(str,"����Ȍ�������͂��ĉ������B\n") ;
			}
			AfxMessageBox(str) ;
		}

	}else{
		AfxMessageBox("�ǂꂩ�ЂƂ̓`�F�b�N����ĉ������B") ;
	}
}

int CMemoryDlg::StartCheck() 
{
	int max_val ;
	int wk ;
	int rt = NORMAL ;

	//�o�萔�`�F�b�N
	wk = GetDlgItemInt(IDC_PR_NUM,NULL,0);
	if(wk < MIN_VAL || wk > PR_NUM_MAX){
		rt |= NUM_ERR ;
	}else{
		num = wk ;
		SetDlgItemInt(IDC_PR_NUM,num,0);
	}

	//�����`�F�b�N
	if(new_mode == KEISAN){
		max_val = KETA_KEISAN_MAX ;
	}else{
		max_val = KETA_ANIKI_MAX ;
	}

	wk = GetDlgItemInt(IDC_PR_KETA,NULL,0);
	if(wk < MIN_VAL || wk > max_val){
		rt |= KETA_ERR ;
	}else{
		keta = wk ;
		SetDlgItemInt(IDC_PR_KETA,keta,0);
	}
	return rt ;
}

void CMemoryDlg::OnAns() 
{
	CMemoryDlg *pParent = (CMemoryDlg *)GetParent();
	CAnserDlg memans(pParent);

	if ( memans.DoModal() == ID_ANSOK ){
		// �Ȃ񂩏��������ˁ`
	}
}

void CMemoryDlg::OnOK() 
{
	CDialog::OnOK();
}

void CMemoryDlg::OnAnki() 
{
	new_mode = ANKI ;
	GetDlgItem(IDC_ENG_SMALL)->EnableWindow(TRUE) ;
	GetDlgItem(IDC_ENG_LARGE)->EnableWindow(TRUE) ;
	GetDlgItem(IDC_NUMBER)->EnableWindow(TRUE) ;
	CheckDlgButton(IDC_ENG_SMALL,type & ENG_SMALL) ;
	CheckDlgButton(IDC_ENG_LARGE,type & ENG_LARGE) ;
	CheckDlgButton(IDC_NUMBER,type & NUMBER) ;
	GetDlgItem(IDC_KETA_STR)->SetWindowText("(1�`15)") ;

	keta = GetDlgItemInt(IDC_PR_KETA,NULL,0);
	if(keta < MIN_VAL || keta > KETA_ANIKI_MAX){
		keta = KETA_INIT_VAL ;
		SetDlgItemInt(IDC_PR_KETA,keta,0) ;
	}
}

void CMemoryDlg::OnKeisan() 
{
	new_mode = KEISAN ;	

	SetType(NUMBER,TRUE) ;
	SetType(ENG_SMALL,FALSE) ;
	SetType(ENG_LARGE,FALSE) ;

	GetDlgItem(IDC_ENG_SMALL)->EnableWindow(FALSE) ;
	GetDlgItem(IDC_ENG_LARGE)->EnableWindow(FALSE) ;
	GetDlgItem(IDC_NUMBER)->EnableWindow(FALSE) ;
	CheckDlgButton(IDC_ENG_SMALL,type & ENG_SMALL) ;
	CheckDlgButton(IDC_ENG_LARGE,type & ENG_LARGE) ;
	CheckDlgButton(IDC_NUMBER,type & NUMBER) ;
	GetDlgItem(IDC_KETA_STR)->SetWindowText("(1�`3)") ;

	keta = GetDlgItemInt(IDC_PR_KETA,NULL,0);
	if(keta < MIN_VAL || keta > KETA_KEISAN_MAX){
		keta = KETA_INIT_VAL ;
		SetDlgItemInt(IDC_PR_KETA,keta,0) ;
	}
}

void CMemoryDlg::StartProc()
{
	if(states != ING){
		states = ING ;
		old_mode = new_mode ;
		if(old_mode == KEISAN){
			SetType(NUMBER,TRUE) ;
		}
		ItemSts(FALSE) ;
		GetDlgItem(IDC_START)->SetWindowText("�X�g�b�v");

		// �Ƃ肠�����\���������i����Ȃ��ƃ`��������j
		GetDlgItem(IDC_SHOW)->SetWindowText("");

		// �ꔭ��
		ViewText() ;
		this->SetTimer(GENERATE_ID,timer_cyc,NULL);
	}else{
		EndProc();
	}
}

void CMemoryDlg::EndProc()
{
	count = 0 ;
	states = END ;
	ItemSts(TRUE) ;
	GetDlgItem(IDC_START)->SetWindowText("�J�n");
	GetDlgItem(IDC_SHOW)->SetWindowText("");
	this->KillTimer(GENERATE_ID);
}

void CMemoryDlg::InitProc()
{
	::memset(&record,0,sizeof(record)) ;
	count = 0 ;
}

void CMemoryDlg::ItemSts(BOOL flg)
{
	GetDlgItem(IDC_PR_NUM)->EnableWindow(flg) ;
	GetDlgItem(IDC_PR_KETA)->EnableWindow(flg) ;
	GetDlgItem(IDC_ANS)->EnableWindow(flg) ;
	GetDlgItem(IDC_ANKI)->EnableWindow(flg) ;
	GetDlgItem(IDC_KEISAN)->EnableWindow(flg) ;
	GetDlgItem(IDC_VIEW_SPEED)->EnableWindow(flg) ;

	if(old_mode != KEISAN){
		GetDlgItem(IDC_NUMBER)->EnableWindow(flg) ;
		GetDlgItem(IDC_ENG_SMALL)->EnableWindow(flg) ;
		GetDlgItem(IDC_ENG_LARGE)->EnableWindow(flg) ;
		GetDlgItem(IDC_VIEW_SPEED)->EnableWindow(flg) ;
	}
}

void CMemoryDlg::KeyGen(char str[KETA_ANIKI_MAX]) 
{
	char wk[ANS_MAX] ;
	int val ;
	int number = 1 ;
	int i=0 ;
	static HFONT font ;	//�\�������\����
	LOGFONT viewfont ;	//�\�������\����

	::memset(&wk,0,sizeof(wk)) ;
	::memset(&viewfont,0,sizeof(viewfont)) ;

	//�\���t�H���g�쐬
	viewfont.lfCharSet = DEFAULT_CHARSET ;
	viewfont.lfWeight = SHOW_WEIGHT ;
	viewfont.lfHeight = SHOW_HEIGHT ;
	font = CreateFontIndirect(&viewfont) ;
	SendDlgItemMessage(IDC_SHOW,WM_SETFONT,(WPARAM)font,MAKELPARAM(TRUE,0)) ;

	//1�b�Ɉ��X�V����݂����ŁA1�b��葁���Ăяo�����ƑO��l�ƑS�������l�ɂȂ��Ă��܂��B
	//�̂ŁA���׍H���Ė���Ⴄ�l���E���悤�ɂ���B
	srand( (unsigned)time( NULL )*(count+1)*(number+number) );
	for(i=0;i<keta;i++){
		val = rand() ;
		int str_type = GetKeyGenType(val) ;
		GetKeyGenVal(wk, str_type, number, val);

		strcat(str,wk) ;
	}
	sprintf(record[count],"%s",str) ;
	count++ ;

	// font���폜���Ȃ��Ⴂ���Ȃ��񂾂��ǁA�R�R�ł��ƍ�����t�H���g�������ɂȂ��Ă��܂��B
	// ������ׂ��ꏊ��font�̍쐬&�폜������K�v����B
	//DeleteObject(font) ;

}

int CMemoryDlg::GetKeyGenType(int val) 
{
	int str_type = NUMBER;

	// type������ ���Z�����l���L�[�ɗD�揇�ʂ����߂�
	if( (val % 3) == 1){
		if(type & ENG_SMALL){
			//Small Alphabet
			str_type = ENG_SMALL ;
		}else if(type & ENG_LARGE){
			//Large Alphabet
			str_type = ENG_LARGE ;
		}else{
			//Number
			str_type = NUMBER ;
		}
	}else if( (val % 3) == 2){
		if(type & ENG_LARGE){
			//Large Alphabet
			str_type = ENG_LARGE ;
		}else if(type & NUMBER){
			//Number
			str_type = NUMBER ;
		}else{
			//Small Alphabet
			str_type = ENG_SMALL ;
		}
	}else{
		if(type & NUMBER){
			//Number
			str_type = NUMBER ;
		}else if(type & ENG_LARGE){
			//Large Alphabet
			str_type = ENG_LARGE ;
		}else{
			//Small Alphabet
			str_type = ENG_SMALL ;
		}
	}

	return str_type;
}

void CMemoryDlg::GetKeyGenVal(char * wk, int str_type, int number, int val)
{
	static int old_number = -1 ;	// 1���̂Ƃ��̑O��l��v�����l������static�ɁB�B

	// �etype���ɏ���
	switch(str_type){
	case NUMBER :
		//number generate
		number = val % 10 ;

		// ���ꕶ�������΍�
		if(number == old_number){
			MatchProc(val,&number) ;
		}
		sprintf(wk,"%d",number) ;
		break ;

	case ENG_SMALL :
		number = 0x61 + (val % 26) ;

		// ���ꕶ�������΍�
		if(number == old_number){
			MatchProc(val,&number) ;
			number += 0x61 ;
		}
		sprintf(wk,"%c",number) ;
		break ;

	case ENG_LARGE :
		number = 0x41 + (val % 26) ;

		// ���ꕶ�������΍�
		if(number == old_number){
			MatchProc(val,&number) ;
			number += 0x41 ;
		}
		sprintf(wk,"%c",number) ;
		break ;
	}
	old_number = number ;
}

void CMemoryDlg::MatchProc(int org_val, int * new_val) 
{
	int len = 0 ;
	int out_val = *new_val ;

	while(org_val != 0){
		if(out_val != org_val % 10){
			*new_val = org_val % 10 ;
			break ;
		}else{
			org_val /= 10 ;
		}
	}
}


void CMemoryDlg::OnEngSmall() 
{
	int val ;
	val = IsDlgButtonChecked(IDC_ENG_SMALL) ;	

	SetType(ENG_SMALL,val) ;
}

void CMemoryDlg::OnEngLarge() 
{
	int val ;
	val = IsDlgButtonChecked(IDC_ENG_LARGE) ;	

	SetType(ENG_LARGE,val) ;
}

void CMemoryDlg::OnNumber() 
{
	int val ;
	val = IsDlgButtonChecked(IDC_NUMBER) ;	

	SetType(NUMBER,val) ;
}

void CMemoryDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	//�X�N���[���o�[��G�����Ƃ�

	timer_cyc = cyc_bar.GetScrollPos();
	int max,min;
	CString str ;

	cyc_bar.GetScrollRange(&min,&max);
	switch (nSBCode)
	{
	case SB_LINELEFT:
		timer_cyc -= 10;
		break;
	case SB_LINERIGHT:
		timer_cyc += 10;
		break;
	case SB_PAGELEFT:
		timer_cyc -= 100;
		break;
	case SB_PAGERIGHT:
		timer_cyc += 100;
		break;
	case SB_LEFT:
		timer_cyc = min;
		break;
	case SB_RIGHT:
		timer_cyc = max;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		timer_cyc = nPos;
	default:
		break;
	}

	if(timer_cyc < MIN_CYC){
		timer_cyc = MIN_CYC ;
	}else if(timer_cyc > MAX_CYC){
		timer_cyc = MAX_CYC ;
	}

	timer_cyc = timer_cyc /10*10 ;
	cyc_bar.SetScrollPos(timer_cyc);

	str.Format("(%d ms)",timer_cyc);
	GetDlgItem(IDC_SPEED_TXT)->SetWindowText(str) ;

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	
}


void CMemoryDlg::OnHelp() 
{
	char help_str[HELP_BUFF] ;

	::memset(help_str,0,sizeof(help_str)) ;

		sprintf(help_str,"�@�@�E�]���]�������\�t�g�@�w���v\n"
		"\n"
		"���o�萔\t�o�肷�鐔\n"
		"������\t�P��ɂ��o�肳��錅��\n"
		"\n"
		"���v���C���[�h�I���i�v�Z / �ËL�j\n"
		"�@�E�v�Z\t�o�肳�ꂽ����S�ĉ��Z����\n"
		"�@�E�ËL\t�o�肳�ꂽ������S�ĈËL����\n"
		"\n"
		"���v���C�^�C�v�I���i���� / �p���� / �p�厚�j\n"
		"�@�E����\t�o�肷�镶���ɐ�����ǉ�\n"
		"�@�E�p����\t�o�肷�镶���ɉp������ǉ�\n"
		"�@�E�p�厚\t�o�肷�镶���ɉp�厚��ǉ�\n"
		"\n"
		"���\�����x�@���̕\�����x��10ms�P�ʂŒ���\n"
		"\n"
		"���{�^��\n"
		"�@�E�J�n\t�X�^�[�g\n"
		"�@�E��\t�������킹\n"
		"�@�E�I��\t�A�v���̏I��\n"
		"�@�E�w���v\t���������{�^��\n") ;

		MessageBox(help_str,"�w���v",MB_OK);
}

void CMemoryDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CMemoryDlg::SetType(int set_type,BOOL flg) 
{
	if(flg == TRUE){
		type |= set_type ;
	}else{
		type &= ~set_type ;
	}
}
