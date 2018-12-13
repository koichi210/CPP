// StrMathDlg.cpp : インプリメンテーション ファイル
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
// CStrMathDlg ダイアログ

CStrMathDlg::CStrMathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStrMathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStrMathDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStrMathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrMathDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
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
// CStrMathDlg メッセージ ハンドラ

BOOL CStrMathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	CheckDlgButton(IDC_2KETA, BST_CHECKED);
	CheckDlgButton(IDC_SUM, BST_CHECKED);
	keta = 2;
	mark = SUM;
	num1 = 0;
	num2 = 0;
	srand((unsigned)time(NULL));

	{
		static HFONT font;	//表示文字構造体
		LOGFONT viewfont;	//表示文字構造体

		::memset(&viewfont, 0, sizeof(viewfont));
		//表示フォント作成
		viewfont.lfCharSet = DEFAULT_CHARSET;
		viewfont.lfWeight = SHOW_WEIGHT;
		viewfont.lfHeight = SHOW_HEIGHT;
		font = CreateFontIndirect(&viewfont);
		SendDlgItemMessage(IDC_VALUE1, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(IDC_VALUE2, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(IDC_MARK, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CStrMathDlg::OnPaint() 
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
		// 答え
		if(mark == SUM){
			ans = num1 + num2;
		}else{
			ans = num1 - num2;
		}

		// 入力値
		ians = GetDlgItemInt(IDC_IANS, NULL, 0);

		if(ans == ians){
			sprintf(str, "正解！！");
		}else{
			sprintf(str, "残念。。", ans);
		}
		MessageBox(str, "解答", MB_OK);
	}else{
		MessageBox("スタートを押して下さい。", "Caution", MB_OK);
	}
}

void CStrMathDlg::OnHlp() 
{
	char str[STR_BUFF];

	memset(str, 0, sizeof(str));
	sprintf(str, "ひらがなで足し算／引き算をします\n\n"
		"1 出題桁数を選択\n"
		"2 演算種別を選択\n"
		"3 スタート釦押下により出題される\n"
		"これにより脳が鍛えられます！！！\n");
	MessageBox(str, "ヘルプ", MB_OK);
}
