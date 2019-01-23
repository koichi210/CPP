// AnsDlg.cpp : インプリメンテーション ファイル
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
// AnsDlg ダイアログ


AnsDlg::AnsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AnsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AnsDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void AnsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AnsDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AnsDlg, CDialog)
	//{{AFX_MSG_MAP(AnsDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AnsDlg メッセージ ハンドラ

BOOL AnsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pParent = (CTurnMemoryDlg *)GetParent();

	for(int i=0;i<CELL_MAX;i++){
		// cell
		for(int j=0;j<CELL_MAX;j++){
			int CurId = j*CELL_MAX+i;
			if((i < pParent->prb_num) && (j < pParent->prb_num)){
				GetDlgItem(IDC_EDIT1+CurId)->ShowWindow(TRUE);
			}else{
				GetDlgItem(IDC_EDIT1+CurId)->ShowWindow(FALSE);
			}
		}

		// edge
		if(i < pParent->prb_num){
			GetDlgItem(IDC_LINE1+i)->ShowWindow(TRUE);
			GetDlgItem(IDC_ROW1+i)->ShowWindow(TRUE);
		}else{
			GetDlgItem(IDC_LINE1+i)->ShowWindow(FALSE);
			GetDlgItem(IDC_ROW1+i)->ShowWindow(FALSE);
		}
	}

	CheckProc();
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void AnsDlg::CheckProc()
{
	BOOL ok = TRUE;
	char str[STR_BUFF];

	memset(&judge, 0, sizeof(judge));
	memset(&str, 0, sizeof(str));


	for(int i=0;i<CurCellMax(pParent->prb_num);i++){
		int CurId = (i/pParent->prb_num) * CELL_MAX + (i%pParent->prb_num);
		if(pParent->anstbl[i] != pParent->intbl[i]){
			judge[i] = FALSE;
			ok = FALSE;
		}else{
			judge[i] = TRUE;
		}
		sprintf(str, "%d", pParent->anstbl[i]);
		GetDlgItem(IDC_EDIT1+CurId)->SetWindowText(str);
	}

	if(ok == TRUE){
		sprintf(str, "全問正解！！");
	}else{
		sprintf(str, "残念。。");
	}
	GetDlgItem(IDC_TITLE)->SetWindowText(str);
}


HBRUSH AnsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int ID = pWnd->GetDlgCtrlID();
	int CurId;

	if(IDC_EDIT1 <= ID && ID <= IDC_EDIT100){
		ID-= IDC_EDIT1 ;
		CurId = ID / CELL_MAX * pParent->prb_num + ID % CELL_MAX;
		if(judge[CurId] == FALSE){
			 pDC->SetTextColor(RGB(0xFF0, 0, 0));   // 文字色は赤
		}
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}
