// memoryDlg.cpp : インプリメンテーション ファイル
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
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
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
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryDlg ダイアログ

CMemoryDlg::CMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemoryDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoryDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
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
// CMemoryDlg メッセージ ハンドラ

BOOL CMemoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str ;

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
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

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
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

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CMemoryDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
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
			sprintf(str,"入力値エラーです。\n\n") ;
			if(rt & NUM_ERR){
				strcat(str,"正常な出題数を入力して下さい。\n") ;
			}

			if(rt & KETA_ERR){
				strcat(str,"正常な桁数を入力して下さい。\n") ;
			}
			AfxMessageBox(str) ;
		}

	}else{
		AfxMessageBox("どれかひとつはチェック入れて下さい。") ;
	}
}

int CMemoryDlg::StartCheck() 
{
	int max_val ;
	int wk ;
	int rt = NORMAL ;

	//出題数チェック
	wk = GetDlgItemInt(IDC_PR_NUM,NULL,0);
	if(wk < MIN_VAL || wk > PR_NUM_MAX){
		rt |= NUM_ERR ;
	}else{
		num = wk ;
		SetDlgItemInt(IDC_PR_NUM,num,0);
	}

	//桁数チェック
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
		// なんか書きたいね～
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
	GetDlgItem(IDC_KETA_STR)->SetWindowText("(1～15)") ;

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
	GetDlgItem(IDC_KETA_STR)->SetWindowText("(1～3)") ;

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
		GetDlgItem(IDC_START)->SetWindowText("ストップ");

		// とりあえず表示を消す（じゃないとチラつくから）
		GetDlgItem(IDC_SHOW)->SetWindowText("");

		// 一発目
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
	GetDlgItem(IDC_START)->SetWindowText("開始");
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
	static HFONT font ;	//表示文字構造体
	LOGFONT viewfont ;	//表示文字構造体

	::memset(&wk,0,sizeof(wk)) ;
	::memset(&viewfont,0,sizeof(viewfont)) ;

	//表示フォント作成
	viewfont.lfCharSet = DEFAULT_CHARSET ;
	viewfont.lfWeight = SHOW_WEIGHT ;
	viewfont.lfHeight = SHOW_HEIGHT ;
	font = CreateFontIndirect(&viewfont) ;
	SendDlgItemMessage(IDC_SHOW,WM_SETFONT,(WPARAM)font,MAKELPARAM(TRUE,0)) ;

	//1秒に一回更新するみたいで、1秒より早く呼び出されると前回値と全く同じ値になってしまう。
	//ので、小細工して毎回違う値を拾うようにする。
	srand( (unsigned)time( NULL )*(count+1)*(number+number) );
	for(i=0;i<keta;i++){
		val = rand() ;
		int str_type = GetKeyGenType(val) ;
		GetKeyGenVal(wk, str_type, number, val);

		strcat(str,wk) ;
	}
	sprintf(record[count],"%s",str) ;
	count++ ;

	// fontを削除しなきゃいけないんだけど、ココでやると作ったフォントが無効になってしまう。
	// しかるべき場所でfontの作成&削除をする必要あり。
	//DeleteObject(font) ;

}

int CMemoryDlg::GetKeyGenType(int val) 
{
	int str_type = NUMBER;

	// typeを決定 除算した値をキーに優先順位を決める
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
	static int old_number = -1 ;	// 1桁のときの前回値一致時を考慮してstaticに。。

	// 各type毎に処理
	switch(str_type){
	case NUMBER :
		//number generate
		number = val % 10 ;

		// 同一文字内被り対策
		if(number == old_number){
			MatchProc(val,&number) ;
		}
		sprintf(wk,"%d",number) ;
		break ;

	case ENG_SMALL :
		number = 0x61 + (val % 26) ;

		// 同一文字内被り対策
		if(number == old_number){
			MatchProc(val,&number) ;
			number += 0x61 ;
		}
		sprintf(wk,"%c",number) ;
		break ;

	case ENG_LARGE :
		number = 0x41 + (val % 26) ;

		// 同一文字内被り対策
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
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	//スクロールバーを触ったとき

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

		sprintf(help_str,"　　右脳左脳活性化ソフト　ヘルプ\n"
		"\n"
		"○出題数\t出題する数\n"
		"○桁数\t１問につき出題される桁数\n"
		"\n"
		"○プレイモード選択（計算 / 暗記）\n"
		"　・計算\t出題された数を全て加算する\n"
		"　・暗記\t出題された文字を全て暗記する\n"
		"\n"
		"○プレイタイプ選択（数字 / 英小字 / 英大字）\n"
		"　・数字\t出題する文字に数字を追加\n"
		"　・英小字\t出題する文字に英小字を追加\n"
		"　・英大字\t出題する文字に英大字を追加\n"
		"\n"
		"○表示速度　問題の表示速度を10ms単位で調整\n"
		"\n"
		"○ボタン\n"
		"　・開始\tスタート\n"
		"　・解答\t答え合わせ\n"
		"　・終了\tアプリの終了\n"
		"　・ヘルプ\t今押したボタン\n") ;

		MessageBox(help_str,"ヘルプ",MB_OK);
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
