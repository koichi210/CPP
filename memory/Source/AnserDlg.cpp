// AnserDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CAnserDlg �_�C�A���O


CAnserDlg::CAnserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnserDlg)
	anser = NULL;
	//}}AFX_DATA_INIT
}

void CAnserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnserDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnserDlg, CDialog)
	//{{AFX_MSG_MAP(CAnserDlg)
	ON_BN_CLICKED(ID_ANS_CHK, OnAnserCheck)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_ANSOK, OnAnsok)
	ON_BN_CLICKED(ID_ANS_SHOW, OnAnsShow)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAnserDlg::OnInitDialog()
{
	int i ;
	CMemoryDlg *pParent = (CMemoryDlg *)GetParent();

	CDialog::OnInitDialog();

	cheat = FALSE ;	// cheat�t���O

	// Enable 2 Disable����
	for(i=0;i<PR_NUM_MAX;i++){
		// �Ƃ肠����Disable�ŏ�����
		GetDlgItem(IDC_ANS_TEXT1+i)->EnableWindow(FALSE) ;
		GetDlgItem(ANS_NO1+i)->EnableWindow(FALSE) ;

		if(pParent->old_mode == KEISAN){
			GetDlgItem(IDC_ANS_TEXT1)->EnableWindow(TRUE) ;
			GetDlgItem(ANS_NO1)->EnableWindow(TRUE) ;
		}else if(i<pParent->num){
			GetDlgItem(IDC_ANS_TEXT1+i)->EnableWindow(TRUE) ;
			GetDlgItem(ANS_NO1+i)->EnableWindow(TRUE) ;
		}
	}
	return TRUE ;
}

/////////////////////////////////////////////////////////////////////////////
// CAnserDlg ���b�Z�[�W �n���h��

void CAnserDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CAnserDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CAnserDlg::OnAnsok() 
{
	CDialog::OnOK() ;
}

void CAnserDlg::OnAnsShow() 
{
	char wk[ANSER_BUFF] ;
	int i ;
	long sum = 0 ;
	CMemoryDlg *pParent = (CMemoryDlg *)GetParent();

	::memset(wk,0,sizeof(wk)) ;

	cheat = !cheat ;

	if(cheat == TRUE){
		for(i=0;i<pParent->num ;i++){
			if(pParent->old_mode == KEISAN){
				sum += atol(pParent->record[i]) ;
			}else{
				GetDlgItem(IDC_ANSER1+i)->SetWindowText(pParent->record[i]) ;
			}
		}

		if(pParent->old_mode == KEISAN){
			sprintf(wk,"%ld",sum) ;
			GetDlgItem(IDC_ANSER1)->SetWindowText(wk) ;
		}

		// �t���̕ύX
		GetDlgItem(ID_ANS_SHOW)->SetWindowText("�������\��") ;

	}else{
		for(i=0;i<pParent->num ;i++){
			GetDlgItem(IDC_ANSER1+i)->SetWindowText("") ;
		}
		// �t���̕ύX
		GetDlgItem(ID_ANS_SHOW)->SetWindowText("�J���j���O��") ;
	}

	GetDlgItem(IDC_CHEAT)->SetWindowText("�����{���ς�") ;
}
	
void CAnserDlg::OnAnserCheck() 
{
	char str[ANSER_BUFF] ;
	char wk[ANSER_BUFF] ;
	char gsBuf[ANS_MAX] ;
	int i ;
	int rt ;
	int now_keta = 0 ;
	long sum = 0 ;
	long input_val = 0 ;

	CMemoryDlg *pParent = (CMemoryDlg *)GetParent();

	::memset(str,0,sizeof(str)) ;
	::memset(gsBuf,0,sizeof(gsBuf)) ;
	::memset(wk,0,sizeof(wk)) ;

	// �\���f�[�^���쐬
	for(i=0;i<pParent->num ;i++){
		GetDlgItemText(IDC_ANS_TEXT1+i,gsBuf,sizeof(gsBuf)) ;

		if(pParent->old_mode == KEISAN){
			// �v�Z
			// ���͒l�����Z
			sum += atol(pParent->record[i]) ;
		}else{
			// �ËL
			//�@����
			rt = strcmp(pParent->record[i],gsBuf) ;
			if(rt == 0){
				// ����
				GetDlgItem(IDC_JUDGE1+i)->SetWindowText("��") ;
				GetDlgItem(IDC_ANSER1+i)->SetWindowText(pParent->record[i]) ;
			}else{
				// �s����
				GetDlgItem(IDC_JUDGE1+i)->SetWindowText("�~") ;
				// �Ԉ�����̂��������̂�������
			}
		}
	}

	// ���v�l��\��
	if(pParent->old_mode == KEISAN){
		GetDlgItemText(IDC_ANS_TEXT1,gsBuf,sizeof(gsBuf)) ;
		input_val= atol(gsBuf) ;

		if(sum == input_val){
			// ����
				sprintf(wk,"%d",input_val) ;
				GetDlgItem(IDC_JUDGE1)->SetWindowText("��") ;
				GetDlgItem(IDC_ANSER1)->SetWindowText(wk) ;
		}else{
			// �s����
				GetDlgItem(IDC_JUDGE1)->SetWindowText("�~") ;
		}
	}
}

HBRUSH CAnserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int ID = pWnd->GetDlgCtrlID();
	switch(ID)   {
	   case IDC_CHEAT:
         pDC->SetTextColor(RGB(0xFF0,0,0));   // �����F�͐�
		 break ;
	}

	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	return hbr;
}

