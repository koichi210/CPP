// AnserDlg.cpp : インプリメンテーション ファイル
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
// CAnserDlg ダイアログ


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
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
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

	cheat = FALSE ;	// cheatフラグ

	// Enable 2 Disable処理
	for(i=0;i<PR_NUM_MAX;i++){
		// とりあえずDisableで初期化
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
// CAnserDlg メッセージ ハンドラ

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

		// 釦名称変更
		GetDlgItem(ID_ANS_SHOW)->SetWindowText("答えを非表示") ;

	}else{
		for(i=0;i<pParent->num ;i++){
			GetDlgItem(IDC_ANSER1+i)->SetWindowText("") ;
		}
		// 釦名称変更
		GetDlgItem(ID_ANS_SHOW)->SetWindowText("カンニング示") ;
	}

	GetDlgItem(IDC_CHEAT)->SetWindowText("答え閲覧済み") ;
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

	// 表示データを作成
	for(i=0;i<pParent->num ;i++){
		GetDlgItemText(IDC_ANS_TEXT1+i,gsBuf,sizeof(gsBuf)) ;

		if(pParent->old_mode == KEISAN){
			// 計算
			// 入力値を加算
			sum += atol(pParent->record[i]) ;
		}else{
			// 暗記
			//　判定
			rt = strcmp(pParent->record[i],gsBuf) ;
			if(rt == 0){
				// 正解
				GetDlgItem(IDC_JUDGE1+i)->SetWindowText("○") ;
				GetDlgItem(IDC_ANSER1+i)->SetWindowText(pParent->record[i]) ;
			}else{
				// 不正解
				GetDlgItem(IDC_JUDGE1+i)->SetWindowText("×") ;
				// 間違ったのだけ書くのもいいし
			}
		}
	}

	// 合計値を表示
	if(pParent->old_mode == KEISAN){
		GetDlgItemText(IDC_ANS_TEXT1,gsBuf,sizeof(gsBuf)) ;
		input_val= atol(gsBuf) ;

		if(sum == input_val){
			// 正解
				sprintf(wk,"%d",input_val) ;
				GetDlgItem(IDC_JUDGE1)->SetWindowText("○") ;
				GetDlgItem(IDC_ANSER1)->SetWindowText(wk) ;
		}else{
			// 不正解
				GetDlgItem(IDC_JUDGE1)->SetWindowText("×") ;
		}
	}
}

HBRUSH CAnserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int ID = pWnd->GetDlgCtrlID();
	switch(ID)   {
	   case IDC_CHEAT:
         pDC->SetTextColor(RGB(0xFF0,0,0));   // 文字色は赤
		 break ;
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

