// zodiacDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "zodiac.h"
#include "zodiacDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZodiacDlg ダイアログ

CZodiacDlg::CZodiacDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZodiacDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZodiacDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZodiacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZodiacDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CZodiacDlg, CDialog)
	//{{AFX_MSG_MAP(CZodiacDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AGE, OnAge)
	ON_BN_CLICKED(IDC_CHINEZODIAC, OnChineZodiac)
	ON_BN_CLICKED(IDC_VIEW, OnView)
	ON_BN_CLICKED(IDC_BIRTH, OnBirth)
	ON_CBN_SELCHANGE(IDC_YEAR, OnSelchangeYear)
	ON_BN_CLICKED(IDC_ALL_VIEW, OnAllView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZodiacDlg メッセージ ハンドラ

BOOL CZodiacDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// make Year
	int ind;
	char str[STR_BUFF];
	time_t now_time;
	struct tm * local;
	int idx = 0;

	memset(str,0,sizeof(str));

	time(&now_time);
	local = localtime(&now_time);

	SendDlgItemMessage(IDC_YEAR, CB_RESETCONTENT, 0L, 0L);
	for( int i=MIN_YEAR; i <= MAX_YEAR; i++ )
	{
		_itoa_s(i, str, sizeof(str), 10);
		ind = SendDlgItemMessage(IDC_YEAR, CB_INSERTSTRING, -1, (LPARAM)str);
		SendDlgItemMessage(IDC_YEAR, CB_SETITEMDATA, (WPARAM)ind, i);

		if ( local->tm_year + MIN_YEAR == i )
		{
			idx = ind;
		}
	}
	SendDlgItemMessage(IDC_YEAR, CB_SETCURSEL, idx);

	GetDlgItemText(IDC_YEAR,str, sizeof (str));
	m_year = atoi(str);

	// default mode 
	CheckDlgButton(IDC_AGE, BST_CHECKED);
	OnAge();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CZodiacDlg::OnPaint() 
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
HCURSOR CZodiacDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CZodiacDlg::OnSelchangeYear() 
{
	char str[STR_BUFF];

	GetDlgItemText(IDC_YEAR, str, sizeof (str));
	m_year = atoi(str);
	Refresh();
}


void CZodiacDlg::OnBirth()
{
	if ( chk_states != FIX_BIRTH )
	{
		chk_states = FIX_BIRTH;
		Refresh();
	}	
}


void CZodiacDlg::OnAge() 
{
	if ( chk_states != FIX_AGE )
	{
		chk_states = FIX_AGE;
		Refresh();
	}
}


void CZodiacDlg::OnChineZodiac() 
{
	if ( chk_states != FIX_ZODIAC )
	{
		chk_states = FIX_ZODIAC;
		Refresh();
	}	
}


void CZodiacDlg::Refresh()
{
	char str[STR_BUFF];
	int nCurrent;

	memset(str, 0, sizeof(str));
	SendDlgItemMessage(IDC_LIST, CB_RESETCONTENT, 0L, 0L);

	switch(chk_states){
	case FIX_BIRTH :
		for( int i=MIN_YEAR; i <= m_year; i++ )
		{
			sprintf(str, BIRTH_FORMAT, i);
			SetListData(str, i);
		}
		nCurrent = DEFAULT_BIRTH - MIN_YEAR;
		break ;

	case FIX_AGE :
		for( int i=MIN_AGE; i <= MAX_AGE; i++ )
		{
			sprintf(str, AGE_FORMAT, i);
			SetListData(str, i);
		}
		nCurrent = DEFAULT_AGE - MIN_AGE;
		break ;

	case FIX_ZODIAC :
		// no break
	default :
		for( int i=0; i < CHINEZODIAC_NUM; i++ )
		{
			sprintf(str, "%s", zod_name[i]);
			SetListData(str, i);
		}
		nCurrent = 0;
		break ;

	}
	SendDlgItemMessage(IDC_LIST, CB_SETCURSEL, nCurrent);
}

void CZodiacDlg::SetListData(char *str, int idx)
{
	int ind = SendDlgItemMessage(IDC_LIST, CB_INSERTSTRING, -1, (LPARAM)str);
	SendDlgItemMessage(IDC_LIST, CB_SETITEMDATA, (WPARAM)ind, idx);
}

void CZodiacDlg::OnView()
{
	char str[VIEW_BUFF];
	char zod[STR_BUFF];
	char wk[STR_BUFF];
	int year;
	int age;
	int birth;
	int i;

	GetDlgItemText(IDC_YEAR,wk, sizeof (wk));
	year = atoi(wk);

	memset(str, 0, sizeof(str));
	memset(zod, 0, sizeof(zod));
	memset(wk, 0, sizeof(wk));
	if ( chk_states == FIX_ZODIAC )
	{
		GetDlgItemText(IDC_LIST, zod, sizeof (zod));
		sprintf(str, "%s年のヒト\r\n\r\n"
					 "生まれた年    年齢\r\n", zod);

		for( i=0; i < CHINEZODIAC_NUM; i++ )
		{
			if ( strcmp(zod, zod_name[i]) == 0 )
			{
				break ;
			}
		}

		year = MIN_YEAR + i;
		for( ; year<m_year; year += CHINEZODIAC_NUM )
		{
			age = m_year - year;

			sprintf(wk, BIRTH_FORMAT, year);
			strcat(str, wk);

			strcat(str, "        ");

			sprintf(wk, AGE_FORMAT, age);
			strcat(str, wk);
			strcat(str, "\r\n");
		}
	}
	else
	{
		if ( chk_states == FIX_AGE )
		{
			// 年齢
			GetDlgItemText(IDC_LIST, wk, sizeof (wk));

			// 生まれた年
			sscanf(wk, AGE_FORMAT, &age);
			birth = year - age;

			// 干支
			GetZodiac(birth, zod);
		}
		else if( chk_states == FIX_BIRTH )
		{
			// 生まれた年
			GetDlgItemText(IDC_LIST, wk, sizeof (wk));
			sscanf(wk, BIRTH_FORMAT, &birth);

			// 干支
			GetZodiac(birth, zod);

			// 年齢
			age = year - birth;
		}
		sprintf(str,"%d年生まれ\r\n"
					"今年は%d才\r\n"
					"%s年です。\r\n", birth, age, zod);
	}
	SetDlgItemText(IDC_PREVIEW, str);
}


int CZodiacDlg::GetZodiac(int year, char * zod)
{
	if ( year < MIN_YEAR || year > m_year )
	{
		AfxMessageBox("プログラムエラー",MB_OK);
		return -1;
	}

	year -= MIN_YEAR;
	year %= CHINEZODIAC_NUM;

	strcpy(zod, zod_name[year]);
	return 0;
}


void CZodiacDlg::OnAllView() 
{
	char str[VIEW_ALL_BUFF];
	char wk[STR_BUFF];
	int year;
	int i;

	memset(str, 0, sizeof(str));

	GetDlgItemText(IDC_YEAR, wk, sizeof (wk));
	year = atoi(wk);

	for( i=0; i < CHINEZODIAC_NUM; i++)
	{
		sprintf(wk, "%14s", zod_name[i]);
		strcat(str, wk);
	}
	strcat(str, "\r\n");

	for( i=MIN_YEAR; i <= m_year; i++ )
	{
		if ( (i-MIN_YEAR) % CHINEZODIAC_NUM == 0 )
		{
			strcat(str, "\r\n");
		}
		sprintf(wk, "%11d年", i);
		strcat(str, wk);
	}

	MessageBox(str, "早見表", MB_OK);
}
