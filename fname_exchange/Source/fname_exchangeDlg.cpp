// fname_exchangeDlg.cpp : インプリメンテーション ファイル
//

#include <afxtempl.h>
#include "stdafx.h"
#include "fname_exchange.h"
#include "fname_exchangeDlg.h"

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
// CFname_exchangeDlg ダイアログ

CFname_exchangeDlg::CFname_exchangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFname_exchangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFname_exchangeDlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Target = TARGET_FILE;
	m_ignore_alert = TRUE;
	m_comp = FALSE;
}

void CFname_exchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFname_exchangeDlg)
	DDX_Control(pDX, IDLB_FILE, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFname_exchangeDlg, CDialog)
	//{{AFX_MSG_MAP(CFname_exchangeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_SETFOCUS(IDET_DIR, OnSetfocus)
	ON_BN_CLICKED(IDBT_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDBT_GET_FILE, OnGetFile)
	ON_BN_CLICKED(IDRB_FILE, OnFile)
	ON_BN_CLICKED(IDRB_FOLDER, OnFolder)
	ON_BN_CLICKED(IDBT_ALL_CHECK, OnAllCheck)
	ON_BN_CLICKED(IDBT_ALL_UNCHECK, OnAllUncheck)
	ON_BN_CLICKED(IDCH_IGNORE_ALERT, OnIgnoreAlert)
	ON_BN_CLICKED(IDCH_COMP_BIG_SMALL, OnCompBigSmall)
	ON_BN_CLICKED(IDRB_ENUM, OnRADIOEnum)
	ON_BN_CLICKED(IDRB_ALL_DBCS, OnRADIOAllDbcs)
	ON_BN_CLICKED(IDRB_ALL_SBCS, OnRADIOAllSbcs)
	ON_BN_CLICKED(IDRB_DEL_NUM, OnRADIODelEnum)
	ON_BN_CLICKED(IDRB_DEL_DIST, OnRADIODelDist)
	ON_BN_CLICKED(IDRB_ADD, OnRadioAdd)
	ON_BN_CLICKED(IDRB_DEL, OnRadioDel)
	ON_BN_CLICKED(IDRB_REP, OnRadioRep)
	ON_BN_CLICKED(IDBT_EXE, OnExecute)
	ON_BN_CLICKED(IDBT_UNDO, OnUndo)
	ON_BN_CLICKED(IDBT_END, OnEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFname_exchangeDlg メッセージ ハンドラ

BOOL CFname_exchangeDlg::OnInitDialog()
{
	long ind;
	CString str;

	CDialog::OnInitDialog();

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

	str.LoadString(IDSTR_FILE_EXCHANGE);
	SetWindowText(str);
	SetDlgItemTextAll(this->m_hWnd, tbl, sizeof(tbl) / sizeof(tbl[0]));

    CheckRadioButton(IDRB_FILE, IDRB_FOLDER,IDRB_FILE);
	SetDlgItemText(IDET_FIRST_NUM, "1");
    CheckRadioButton(IDRB_ENUM,IDRB_REP,IDRB_ENUM);
	CheckDlgButton(IDCH_KEEP_NAME, BST_CHECKED);
	CheckDlgButton(IDCH_ADD_BEF, BST_CHECKED);
	CheckDlgButton(IDCH_IGNORE_ALERT, DEFAULT_ALERT);
	m_type = ENUM;

	//makelist box
	SendDlgItemMessage(IDCB_KETA, CB_RESETCONTENT, 0, 0L);

	str.LoadString(IDSTR_AUTO);
	SendDlgItemMessage(IDCB_KETA, CB_RESETCONTENT, 0, 0L);
	ind = SendDlgItemMessage(IDCB_KETA, CB_ADDSTRING, 0,(LPARAM)str.GetString());
	SendDlgItemMessage(IDCB_KETA, CB_SETITEMDATA, (WPARAM)0, ind);
	for (int i=1; i <= KETA_MAX; i++)
	{
		str.Format(IDSTR_KETA,i);
		ind = SendDlgItemMessage(IDCB_KETA, CB_ADDSTRING, 0,(LPARAM)str.GetString());
		SendDlgItemMessage(IDCB_KETA, CB_SETITEMDATA, (WPARAM)i, ind);
	}
    SendDlgItemMessage(IDCB_KETA,CB_SETCURSEL,0,0L);

	InitUndo();
	Refresh();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CFname_exchangeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFname_exchangeDlg::OnPaint() 
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
HCURSOR CFname_exchangeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CFname_exchangeDlg::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}


void CFname_exchangeDlg::OnSetfocus()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//TODO:フォーカスが当たったら、選択状態を反転したい
}

void CFname_exchangeDlg::OnBrowse() 
{
	CString title,path;
	title = "Select a destination folder";

	BOOL rt = Browse(this->m_hWnd, title, &path);
	if ( rt )
	{
		SetDlgItemText(IDET_DIR, path);
		OnGetFile();
	}
}

void CFname_exchangeDlg::OnGetFile() 
{
	TCHAR pathName[MAX_PATH] = { 0 };
	size_t szPathLength;
	int szLen;
	int nMax=0;

	// 指定フォルダ内のファイルを表示する
	GetDlgItemText(IDET_DIR,pathName,sizeof(pathName));
	if(lstrcmp(pathName,"") == 0)
	{
		return;
	}

	szPathLength = strlen(pathName);
	szPathLength++; // "\\"
	strcat_s(pathName,sizeof(pathName), "\\*.*");

	// 表示内容をリセットする
	m_list.ResetContent();

	if ( m_Target == TARGET_FILE )
	{
		// m_list.Dir でファイル
		m_list.Dir(DDL_READWRITE,pathName);
//		m_list.Dir(DDL_DIRECTORY,pathName);
	}
	else // ( m_Target == TARGET_DIRECOTY )
	{
		// cFileFind でフォルダを取得
		CFileFind cFind;
		CString strFolder;
		BOOL bContinue = cFind.FindFile(pathName);

		while( bContinue )
		{
			bContinue = cFind.FindNextFile();

			if( cFind.IsDirectory() )
			{
				strFolder = cFind.GetFilePath();
				strFolder.Delete(0, szPathLength);
				if ( (strcmp(strFolder, ".") == 0 ) ||
					 (strcmp(strFolder, "..") == 0 ) )
				{
					continue;
				}

				m_list.AddString(strFolder);
				szLen = strFolder.GetLength();
				if ( nMax < szLen )
				{
					nMax = szLen;
				}
			}
		}
	}
	m_list.SetHorizontalExtent(nMax * HORIZONTAIL_OFFSET);
}

void CFname_exchangeDlg::OnFile()
{
	m_Target = TARGET_FILE;
	OnGetFile();
}

void CFname_exchangeDlg::OnFolder()
{
	m_Target = TARGET_DIRECTOY;
	OnGetFile();
}

void CFname_exchangeDlg::OnAllCheck() 
{
	m_list.SetSel(-1,TRUE);
}

void CFname_exchangeDlg::OnAllUncheck() 
{
	m_list.SetSel(-1,FALSE);
}

void CFname_exchangeDlg::OnIgnoreAlert()
{
	UINT check = IsDlgButtonChecked(IDCH_IGNORE_ALERT);
	if( check == BST_CHECKED )
	{
		m_ignore_alert = TRUE;
	}
	else
	{
		m_ignore_alert = FALSE;
	}
}

void CFname_exchangeDlg::OnCompBigSmall()
{
	UINT check = IsDlgButtonChecked(IDCH_COMP_BIG_SMALL);
	if( check == BST_CHECKED )
	{
		m_comp = TRUE;
	}
	else
	{
		m_comp = FALSE;
	}
}

void CFname_exchangeDlg::OnEnd() 
{
	CDialog::OnOK();
}

void CFname_exchangeDlg::OnRADIOEnum() 
{
	m_type = ENUM;
	Refresh();
}

void CFname_exchangeDlg::OnRADIOAllDbcs()
{
	m_type = ALLDBCS;
	Refresh();
}

void CFname_exchangeDlg::OnRADIOAllSbcs()
{
	m_type = ALLSBCS;
	Refresh();
}

void CFname_exchangeDlg::OnRADIODelEnum() 
{
	m_type = DELNUM;
	Refresh();
}

void CFname_exchangeDlg::OnRADIODelDist() 
{
	m_type = DELDIST;
	Refresh();
	
}

void CFname_exchangeDlg::OnRadioAdd() 
{
	m_type = ADD;
	Refresh();
}

void CFname_exchangeDlg::OnRadioDel() 
{
	m_type = DEL;
	Refresh();
}

void CFname_exchangeDlg::OnRadioRep() 
{
	m_type = REP;
	Refresh();
}

void CFname_exchangeDlg::OnExecute() 
{
	// 変数を初期化
	// 画面上の設定値を取得
	GetSetting();
	GetFileList();
	if( m_list_cnt == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_NO_SELECT);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	//ファイル名変換処理
	ExChangeProc();

	//復元情報を更新
	m_undo.curnum++;
	m_undo.maxnum = m_undo.curnum;
	OnGetFile();

	//TODO : これ以上復元できない場合にはDisableにしないと。。
	GetDlgItem(IDBT_UNDO)->EnableWindow(TRUE);
}

void CFname_exchangeDlg::OnUndo() 
{
	UndoProc();
	OnGetFile();
	Refresh();
}

void CFname_exchangeDlg::Refresh() 
{
	BOOL bEnable=FALSE;
	CString str;

	//通し番号付加
	bEnable = FALSE;
	if ( m_type == ENUM )
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_FIRST_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDET_FIRST_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDCH_KEEP_NAME)->EnableWindow(bEnable);
	GetDlgItem(IDCB_KETA)->EnableWindow(bEnable);

	//指定文字数の削除
	bEnable = FALSE;
	if ( m_type == DELNUM )
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_DEL_BEF_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDET_DEL_BEF_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDST_DEL_AFT_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDET_DEL_AFT_NUM)->EnableWindow(bEnable);

	//文字の追加[先頭から][後部から]
	bEnable = FALSE;
	if ( m_type == ADD )
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDCH_ADD_BEF)->EnableWindow(bEnable);
	GetDlgItem(IDCH_ADD_AFT)->EnableWindow(bEnable);

	//名称1
	bEnable = FALSE;
	if(m_type == DEL || m_type == REP)
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_NAME1)->EnableWindow(bEnable);
	GetDlgItem(IDET_NAME1)->EnableWindow(bEnable);

	//名称2
	bEnable = FALSE;
	if(m_type == ADD || m_type == REP)
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDST_NAME2)->EnableWindow(bEnable);
	GetDlgItem(IDET_NAME2)->EnableWindow(bEnable);

	// 名称1、名称2
	switch(m_type)
	{
	case ADD :
		str.LoadString(IDSTR_NAME_ADD);
		GetDlgItem(IDST_NAME2)->SetWindowText(str);
		break;
	case DEL :
		str.LoadString(IDSTR_NAME_DEL);
		GetDlgItem(IDST_NAME1)->SetWindowText(str);
		break;
	case REP :
		str.LoadString(IDSTR_REP_BEF);
		GetDlgItem(IDST_NAME1)->SetWindowText(str);
		str.LoadString(IDSTR_NAME_REP_AFT);
		GetDlgItem(IDST_NAME2)->SetWindowText(str);
		break ;
	default :
		break ;

	// 復元
	bEnable = FALSE;
	if(m_undo.curnum > 0)
	{
		bEnable = TRUE;
	}
	GetDlgItem(IDBT_UNDO)->EnableWindow(bEnable);
	}
}

void CFname_exchangeDlg::GetFileList() 
{
	CArray <int, int> selections;

	UpdateData(TRUE);

	m_list_cnt = m_list.GetSelCount();
	selections.SetSize(m_list_cnt);
	m_list.GetSelItems((int) m_list_cnt, selections.GetData());

	m_list_cnt = selections.GetSize();

	for (int i=0; i < m_list_cnt; i++)
	{
		m_list.GetText(selections.GetAt(i), m_file_name[i]);
	}
}


void CFname_exchangeDlg::GetSetting() 
{
	m_first_num = 0;
	m_cur_num = 0;
	m_list_cnt = 0;
	m_Bef_Del_num = 0;
	m_Aft_Del_num = 0;
	m_opt=0;
	m_keta=0;
	memset(m_dir,0,sizeof(m_dir));
	memset(m_name1,0,sizeof(m_name1));
	memset(m_name2,0,sizeof(m_name2));

	GetDlgItemText(IDET_DIR, m_dir, sizeof(m_dir));

	switch(m_type)
	{
	case ENUM:
		m_first_num = GetDlgItemInt(IDET_FIRST_NUM, NULL, 0);
		m_cur_num = m_first_num;
		if(IsDlgButtonChecked(IDCH_KEEP_NAME) == BST_CHECKED){
			m_opt += KEEP;
		}
		m_keta = SendDlgItemMessage(IDCB_KETA, CB_GETCURSEL, 0, 0);
		break;

	case DELNUM:
		m_Bef_Del_num = GetDlgItemInt(IDET_DEL_BEF_NUM, NULL, 0);
		m_Aft_Del_num = GetDlgItemInt(IDET_DEL_AFT_NUM, NULL, 0);
		break;

//	case DELDIST:
//		break;

	case ADD:
		GetDlgItemText(IDET_NAME2, m_name2, sizeof(m_name2));
		if(IsDlgButtonChecked(IDCH_ADD_BEF) == BST_CHECKED){
			m_opt += BEF;
		}
		if(IsDlgButtonChecked(IDCH_ADD_AFT) == BST_CHECKED){
			m_opt += AFT;
		}
		break;

	case DEL:
		GetDlgItemText(IDET_NAME1, m_name1, sizeof(m_name1));
		break;

	case REP:
		GetDlgItemText(IDET_NAME1, m_name1, sizeof(m_name1));
		GetDlgItemText(IDET_NAME2, m_name2, sizeof(m_name2));
		break;

	default :
		break;
	}
}

void CFname_exchangeDlg::ExChangeProc()
{
	// リストコントロールで選択された数だけループ
	for(int i=0; i < m_list_cnt; i++)
	{
		// オリジナルのファイル名
		m_oname = m_dir;
		AppendPath(&m_oname, m_file_name[i]);

		//新しいファイル名取得
		switch(m_type)
		{
		case ENUM:		EnumProc();		break;	// 通し番号付加
		case ALLSBCS:	SbcsProc();		break;	// すべて半角
		case ALLDBCS:	DbcsProc();		break;	// すべて全角
		case DELNUM:	DelNumProc();	break;	// 指定文字数削除
		case DELDIST:	DelDistProc();	break;	// 拡張子削除
		case ADD:		AddProc();		break;	// 文字列追加
		case DEL:		DelProc();		break;	// 文字列削除
		case REP:		RepProc();		break;	// 文字列置換
		default :		break;
		}

		//ファイル名変換
		MoveFileProc();
	}
}

void CFname_exchangeDlg::EnumProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// 拡張子
	CString num;
	UINT number = m_first_num;
	int dst_keta = GetKeta(m_first_num + m_list_cnt); //目的のケタ数
	int cur_keta;	//カレントのケタ数

	if ( dst_keta < m_keta )
	{
		dst_keta = m_keta;
	}

	// 目的のファイルパス（ディレクトリ名）
	m_nname = m_dir;
	m_nname += "\\";

	// 桁数だけ0を詰める
	if ( m_cur_num == 0 )
	{
		cur_keta = 1;
	}
	else
	{
		cur_keta = GetKeta(m_cur_num);
	}

	for(int j=0; j < dst_keta - cur_keta; j++)
	{
		m_nname += "0";
	}

	// 目的のファイルパス（ファイル名）
	num.Format("%d", m_cur_num);
	m_nname += num;
	m_cur_num++;

	// パスを分解
	SplitPath(m_oname,&dir,&file,&ext);

	// オリジナルのファイル名を残す場合
	if ( m_opt == KEEP )
	{
		m_nname += " ";
		m_nname += file;
	}

	// add distination
	AppendExt(&m_nname, ext);
}

void CFname_exchangeDlg::SbcsProc()
{
	CString dir;
	CString ofile;	// old file name
	CString nfile;	// new file name
	CString ext;	// 拡張子

	LPSTR pfname = NULL;
	int len = 0;

	// パスを分解
	SplitPath(m_oname,&dir,&ofile,&ext);

	// 目的のファイルパス
	m_nname = dir;
	m_nname += "\\";

	// 文字列の削除
	ReplaceString2(ofile, &nfile, TRUE);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DbcsProc()
{
	CString dir;
	CString ofile;	// old file name
	CString nfile;	// new file name
	CString ext;	// 拡張子

	LPSTR pfname = NULL;
	int len = 0;

	// パスを分解
	SplitPath(m_oname,&dir,&ofile,&ext);

	// 目的のファイルパス
	m_nname = dir;
	m_nname += "\\";

	// 文字列の削除
	ReplaceString2(ofile, &nfile, FALSE);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DelNumProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// 拡張子
	UINT len;

	if( ! (m_Bef_Del_num || m_Aft_Del_num) )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_DEL_NUM);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	// パスを分解
	SplitPath(m_oname,&dir,&file,&ext);

	// 指定文字数削除したときのファイル長を確認
	len = strlen(file);
	if ( len < (m_Bef_Del_num + m_Aft_Del_num + DIST_MAX) )
	{
		if ( ! m_ignore_alert )
		{
			CString str,title;
			str.LoadString(IDSTR_ERR_SHORT_NAME);
			title.LoadString(IDSTR_ERROR);
			MessageBox(str,title,MB_OK);
		}
		return;
	}

	// 後部からn文字削除
	file.Delete(len-m_Aft_Del_num,m_Aft_Del_num);

	// 先頭からn文字削除
	file.Delete(0,m_Bef_Del_num);

	// 目的のファイルパス（ファイル名）
	m_nname = m_dir;
	AppendPath(&m_nname,file);

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DelDistProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// 拡張子

	// パスを分解
	SplitPath(m_oname,&dir,&file,&ext);

	//目的のファイル名
	m_nname = dir + file;
}

void CFname_exchangeDlg::AddProc()
{
	CString dir;
	CString file;	// file name
	CString ext;	// 拡張子

	// TODO : 警告はGetSettingにもっていこう
	if ( m_opt == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_SEL_INSERT);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}
	else if( strlen(m_name2) == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_INPUT_ADD_STR);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	// パスを分解
	SplitPath(m_oname,&dir,&file,&ext);

	// 目的のファイルパス
	m_nname = dir;
	m_nname += "\\";

	// 先頭に追加する文字列
	if( m_opt & BEF )
	{
		m_nname += m_name2;
	}
	
	//オリジナルの文字列
	m_nname += file;

	// 後部に追加する文字列
	if( m_opt & AFT )
	{
		m_nname += m_name2;
	}

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::DelProc()
{
	CString dir;
	CString ofile;	// file name
	CString nfile;	// 新ファイル名
	CString ext;	// 拡張子

	// TODO: 警告はあっちへもっていこう
	if(strlen(m_name1) == 0)
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_INPUT_DEL_STR);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}
	
	// パスを分解
	SplitPath(m_oname,&dir,&ofile,&ext);

	// 目的のファイルパス
	m_nname = m_dir;
	m_nname += "\\";

	// 文字列の削除
	ReplaceString(ofile, &nfile, m_name1, NULL, m_comp);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::RepProc()
{
	CString dir;
	CString ofile;	// old file name
	CString nfile;	// new file name
	CString ext;	// 拡張子

	// TODO: あっちへもっていこう
	if(strlen(m_name1) == 0 || strlen(m_name2) == 0 )
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_INPUT_REP_STR);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	LPSTR pfname = NULL;
	int len = 0;

	// パスを分解
	SplitPath(m_oname,&dir,&ofile,&ext);

	// 目的のファイルパス
	m_nname = dir;
	m_nname += "\\";

	// 文字列の削除
	ReplaceString(ofile, &nfile, m_name1, m_name2, m_comp);
	m_nname += nfile;

	// add distination
	AppendExt(&m_nname,ext);
}

void CFname_exchangeDlg::UndoProc()
{
	CString oname;
	CString nname;
	BOOL bReturn = TRUE;

	if(m_undo.curnum <= 0)
	{
		CString str,title;
		str.LoadString(IDSTR_ERR_NOT_UNDO);
		title.LoadString(IDSTR_ERROR);
		MessageBox(str,title,MB_OK);
		return;
	}

	m_undo.curnum--;
	for(int i=0; i < m_undo.exch[m_undo.curnum].num; i++)
	{
		oname = m_undo.exch[m_undo.curnum].ofname[i];
		nname = m_undo.exch[m_undo.curnum].nfname[i];
		bReturn = MoveFile(nname.GetString(), oname.GetString());
		if ( ! bReturn )
		{
			if ( ! m_ignore_alert )
			{
				CString str,title;
				str.Format(IDSTR_ERR_FAIL_UNDO,oname,nname);
				title.LoadString(IDSTR_ERROR);
				MessageBox(str,title,MB_OK);
			}
		}
	}
}

int CFname_exchangeDlg::GetKeta(int number)
{
	int keta;
	for( keta=0; number; keta++)
	{
		number /= 10;
	}
	return keta;
}

void CFname_exchangeDlg::MoveFileProc()
{
	int bReturn;

	// 大文字小文字を区別する
	bReturn = m_oname.CompareNoCase(m_nname);
	if ( bReturn )
	{
		bReturn = MoveFile(m_oname.GetString(), m_nname.GetString());
		if ( bReturn )
		{
			int unum = m_undo.curnum;	// 実行ボタン押した数
			int fnum = m_undo.exch[m_undo.curnum].num; // 1度に変更したファイルの数

			m_undo.exch[unum].ofname[fnum] = m_oname;
			m_undo.exch[unum].nfname[fnum] = m_nname;
			if ( m_undo.exch[m_undo.curnum].num < FILE_MAX )
			{
				m_undo.exch[unum].num++;
			}
			else
			{
				CString str,title;
				int rt;

				str.LoadString(IDSTR_WRN_CACHE_FULL);
				title.LoadString(IDSTR_WRN);
				rt = MessageBox(str,title,MB_YESNO);
				if ( rt == IDYES )
				{
					InitUndo();
				}
			}
		}
		else
		{
			if ( ! m_ignore_alert )
			{
				CString str,title;
				int err = GetLastError();
				str.Format(IDSTR_ERR_FAIL_CHANGE_NAME,err,m_oname,m_nname);
				title.LoadString(IDSTR_ERROR);
				MessageBox(str,title,MB_OK);
			}
		}
	}
	else
	{
		if ( ! m_ignore_alert )
		{
			CString str,title;
			int err = GetLastError();
			str.Format(IDSTR_ERR_FAIL_OVERLAP,m_oname,m_nname);
			title.LoadString(IDSTR_ERROR);
			MessageBox(str,title,MB_OK);
		}
	}
}

void CFname_exchangeDlg::InitUndo()
{
	m_undo.curnum=0;
	m_undo.maxnum=0;
	for(int i=0; i<UNDO_MAX;i++)
	{
		m_undo.exch[i].num=0;
	}
}

void CFname_exchangeDlg::ReplaceString2(CString oname, CString *nname, BOOL bDB2SB)
{
	char old_fname[MAX_PATH];
	char new_fname[MAX_PATH];
	char tmp[MAX_PATH];
	LPSTR pfname = NULL;
	UINT cp_num=0;	//コピする数
	UINT cp_start=0;//コピする先頭配列
	BOOL rt = FALSE;

	memset(new_fname, 0, sizeof(new_fname));
	memset(tmp, 0, sizeof(tmp));

	strcpy_s(old_fname,sizeof(old_fname),(LPSTR)oname.GetString());	// オリジナルファイル名をコピ
	if ( bDB2SB )
	{
		zen2han(old_fname);
	}
	else
	{
		han2zen(old_fname);
	}

	strcat_s(new_fname, sizeof(new_fname), old_fname);	//最終的な目的の文字列
	*nname = new_fname;

#if 0
	char old_fname[MAX_PATH];
	char new_fname[MAX_PATH];
	char tmp[MAX_PATH];
	LPSTR pfname = NULL;
	UINT cp_num=0;	//コピする数
	UINT cp_start=0;//コピする先頭配列
	BOOL rt = FALSE;

	memset(new_fname, 0, sizeof(new_fname));
	memset(tmp, 0, sizeof(tmp));

	strcpy_s(old_fname,sizeof(old_fname),(LPSTR)oname.GetString());	// オリジナルファイル名をコピ
	if ( bDB2SB )
	{
		for(UINT i=0; i<_tcslen(old_fname); i++)	// 1文字ずつ検索
		{
			for(UINT j=0; j<_tcslen(dbcs); j++)		// 1文字ずつ検索
			{
				rt = strncmp(&old_fname[i], &dbcs[j], 2);	// 指定文字数で目的の文字列を検索

				if(rt == 0)	// 指定文字数で目的の文字列を検索
				{
					strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//見つかる前までの文字列
					strcat_s(new_fname, sizeof(new_fname), tmp);	//最終的な目的の文字列
					strcat_s(new_fname, sizeof(new_fname), &sbcs[j]);

					cp_num=0;	//キャッシュのカウントをリセットする
					cp_start = i + 1;
				}
				else
				{
					cp_num++;
				}
			}
		}
	}
	else // bDBCS
	{
		for(UINT i=0; i<_tcslen(old_fname); i++)	// 1文字ずつ検索
		{
			for(UINT j=0; j<_tcslen(sbcs); j++)		// 1文字ずつ検索
			{
				rt = strncmp(&old_fname[i], &sbcs[j], 1);	// 指定文字数で目的の文字列を検索

				if(rt == 0)	// 指定文字数で目的の文字列を検索
				{
					strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//見つかる前までの文字列
					strcat_s(new_fname, sizeof(new_fname), tmp);	//最終的な目的の文字列
					strcat_s(new_fname, sizeof(new_fname), &dbcs[j]);

					cp_num=0;	//キャッシュのカウントをリセットする
					cp_start = i + 1;
				}
				else
				{
					cp_num++;
				}
			}
		}
	}

	//for(UINT i=0; i<strlen(old_fname); i++)	// 1文字ずつ検索
	//{
	//	if ( bDiff )
	//	{
	//		rt = strncmp(&old_fname[i], srch, strlen(srch));	// 指定文字数で目的の文字列を検索
	//	}
	//	else
	//	{
	//		rt = _strnicmp(&old_fname[i], srch, strlen(srch));	// 指定文字数で目的の文字列を検索
	//	}

	//	if(rt == 0)	// 指定文字数で目的の文字列を検索
	//	{
	//		strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//見つかる前までの文字列
	//		strcat_s(new_fname, sizeof(new_fname), tmp);	//最終的な目的の文字列
	//		if(rep != NULL)
	//		{
	//			// 置換対象文字があるなら、ここで実行
	//			strcat_s(new_fname, sizeof(new_fname), rep);
	//		}
	//		i += strlen(srch) - 1; //みつかった文字列分の検索は飛ばす
	//		cp_num=0;	//キャッシュのカウントをリセットする
	//		cp_start = i + 1;
	//	}
	//	else
	//	{
	//		cp_num++;
	//	}
	//}

	//// 対象文字列以降の文字を連結
	//if ( cp_num )
	//{
	//	strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//見つかる前までの文字列
	//	strcat_s(new_fname, sizeof(new_fname), tmp);	//最終的な目的の文字列
	//}
	//*nname = new_fname;
#endif
}
