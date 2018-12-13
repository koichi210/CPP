// TurnMemoryDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "TurnMemory.h"
#include "TurnMemoryDlg.h"
#include "AnsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryDlg �_�C�A���O

CTurnMemoryDlg::CTurnMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTurnMemoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTurnMemoryDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTurnMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTurnMemoryDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTurnMemoryDlg, CDialog)
	//{{AFX_MSG_MAP(CTurnMemoryDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_ANS, OnAns)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryDlg ���b�Z�[�W �n���h��

BOOL CTurnMemoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�

	GetDlgItem(IDC_TITLE)->SetWindowText(START_STR);
	srand(time(NULL));
	//makelist box
	SendDlgItemMessage(IDC_PROBLEM, CB_RESETCONTENT, 0, 0L);
	for (int i = CELL_MIN; i <= CELL_MAX; i++)
	{
		char str[256];
		long ind;
		memset(str, 0, sizeof(str));

		sprintf(str, "%d �~ %d", i, i); 
		ind = SendDlgItemMessage(IDC_PROBLEM, CB_ADDSTRING, 0, (LPARAM)str);
		SendDlgItemMessage(IDC_PROBLEM, CB_SETITEMDATA, (WPARAM)i, ind);
	}

	prb_num = DEF_PRB;
	states = INIT;
	m_cnt = 1;
    SendDlgItemMessage(IDC_PROBLEM, CB_SETCURSEL, prb_num-CELL_MIN, 0L);
	Refresh();

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CTurnMemoryDlg::OnPaint() 
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
HCURSOR CTurnMemoryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTurnMemoryDlg::OnStart() 
{
	if(states != START){
		memset(&intbl, 0, sizeof(intbl));
		states = START;
		GetDlgItem(IDC_START)->SetWindowText(STOP_BUTTON);
		GetDlgItem(IDC_TITLE)->SetWindowText(KEEP_MIND_STR);
		GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PROBLEM)->EnableWindow(FALSE);

		Refresh();
		BuildNumber();
		this->SetTimer(EVENT_SHOW, SHOW_INVAL, NULL);
	}else{
		states = STOP;
		InitProc();
		ExitProc();
	}
}

void CTurnMemoryDlg::OnAns() 
{
	CTurnMemoryDlg *pParent = (CTurnMemoryDlg *)GetParent();
	AnsDlg Ans(pParent);

	int ans_cnt = 0;

	for(int i=0;i<prb_num;i++){
		for(int j=0;j<prb_num;j++){
			int CurId = i*CELL_MAX+j;
			intbl[ans_cnt++] = GetDlgItemInt(IDC_EDIT1+CurId, NULL, 0);
		}
	}

	Ans.DoModal();

	GetDlgItem(IDC_TITLE)->SetWindowText(START_STR);
}

void CTurnMemoryDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == EVENT_SHOW){
		//�l�\��
		ShowProc();
	}else if(nIDEvent == EVENT_WAIT){
		//�L�����ԃJ�E���g�_�E��
		WaitProc();
	}
	CDialog::OnTimer(nIDEvent);
}

void CTurnMemoryDlg::EndProc()
{
	m_cnt = 1;
	this->KillTimer(EVENT_SHOW);
	this->SetTimer(EVENT_WAIT, WAIT_INVAL, NULL);
}

void CTurnMemoryDlg::ExitProc()
{
	states = END;
	m_cnt = 1;
	memset(anstbl, 0, sizeof(anstbl));
	GetDlgItem(IDC_TITLE)->SetWindowText(START_STR);
	GetDlgItem(IDC_START)->SetWindowText(START_BUTTON);
	this->KillTimer(EVENT_SHOW);
	this->KillTimer(EVENT_WAIT);
}

void CTurnMemoryDlg::InitProc()
{
	GetDlgItem(IDC_START)->SetWindowText(START_BUTTON);
	GetDlgItem(IDC_TITLE)->SetWindowText(END_STR);
	GetDlgItem(IDC_PROBLEM)->EnableWindow(TRUE);
	if(states == END && intbl[0] == 0){
		GetDlgItem(IDC_ANS)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	}

	for(int i=0;i < prb_num;i++){
		for(int j=0;j < prb_num;j++){
			int CurId = j*CELL_MAX+i;
			GetDlgItem(IDC_EDIT1+CurId)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT1+CurId)->SetWindowText("");
		}
	}
}

void CTurnMemoryDlg::BuildNumber()
{
	int max = 0;
	int idx = 0;
	int cell_cnt = CurCellMax(prb_num);
	int i, j;

	// make
	memset(&anstbl, 0, sizeof(anstbl));
	for(i=0;i<CurCellMax(prb_num);i++){
		anstbl[i] = rand();
	}

	// sort and build
	for(i=0;i<CurCellMax(prb_num);i++){
		for(j=0;j<CurCellMax(prb_num);j++){
			if(anstbl[j] > max){
				max = anstbl[j];
				idx = j;
			}
		}
		anstbl[idx] = cell_cnt--;
		max = 0 ;
	}
}

void CTurnMemoryDlg::Refresh()
{
	prb_num = SendDlgItemMessage(IDC_PROBLEM, CB_GETCURSEL, 0, 0);
	prb_num += CELL_MIN;

	for(int i=0;i<CELL_MAX;i++){
		// cell
		for(int j=0;j<CELL_MAX;j++){
			int CurId = j*CELL_MAX+i;
			if((i < prb_num) && (j < prb_num)){
				GetDlgItem(IDC_EDIT1+CurId)->ShowWindow(TRUE);
				GetDlgItem(IDC_EDIT1+CurId)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT1+CurId)->SetWindowText("");
			}else{
				GetDlgItem(IDC_EDIT1+CurId)->ShowWindow(FALSE);
			}
		}

		// edge
		if(i < prb_num){
			GetDlgItem(IDC_LINE1+i)->ShowWindow(TRUE);
			GetDlgItem(IDC_ROW1+i)->ShowWindow(TRUE);
		}else{
			GetDlgItem(IDC_LINE1+i)->ShowWindow(FALSE);
			GetDlgItem(IDC_ROW1+i)->ShowWindow(FALSE);
		}
	}
}

void CTurnMemoryDlg::ShowProc() 
{
	int i;
	int CurId = -1;
	char str[STR_BUFF];

	memset(str, 0, sizeof(str));

	// serch
	for(i=0;i<CurCellMax(prb_num);i++){
		if(anstbl[i] == m_cnt){
			CurId = (i/prb_num) * CELL_MAX + (i%prb_num);
			break ;
		}
	}

	sprintf(str, "%d", m_cnt);
	GetDlgItem(IDC_EDIT1+CurId)->SetWindowText(str);
	m_cnt ++;
	if(m_cnt > CurCellMax(prb_num)){
		EndProc();
	}
}

void CTurnMemoryDlg::WaitProc() 
{
	static int timer = WAIT_MAX;
	char str[STR_BUFF];

	memset(str, 0, sizeof(str));

	sprintf(str, "%d%s", timer, CNT_REMEMBER_STR);
	GetDlgItem(IDC_TITLE)->SetWindowText(str);
	timer --;
	if(timer < 0){
		states = END;
		InitProc();
		timer = WAIT_MAX ;
		this->KillTimer(EVENT_WAIT);
	}
}