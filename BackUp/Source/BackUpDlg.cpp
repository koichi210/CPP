// BackUpDlg.cpp : 実装ファイル
//

#include <shlwapi.h>
#include <shlobj.h>
#include "stdafx.h"
#include "BackUp.h"
#include "BackUpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBackUpDlg ダイアログ




CBackUpDlg::CBackUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackUpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_idx = 0;
	m_end = END_NONE;
	m_FileOut = FALSE;

	m_bkDefault.bBkEnable = TRUE;
	m_bkDefault.opt = SUBDIR_MASK | DIFF_MASK;
	m_bkDefault.strDstPath = "";
	m_bkDefault.strSrcPath = "";
}

void CBackUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL, m_listctl);
}

BEGIN_MESSAGE_MAP(CBackUpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BACKUP_START, OnBackupStart)
	ON_BN_CLICKED(IDC_DIFF, OnDiff)
	ON_BN_CLICKED(IDC_BROWSE_SRC, OnBrowseSrc)
	ON_BN_CLICKED(IDC_BROWSE_DEST, OnBrowseDest)
	ON_BN_CLICKED(IDC_SAVE_SETTING, OnBnClickedSaveSetting)
	ON_BN_CLICKED(IDC_SUBDIR, OnBnClickedSubdir)
	ON_BN_CLICKED(IDC_OVERWRITE, OnBnClickedOverWrite)
	ON_BN_CLICKED(IDC_SELECT_BACKUP, OnBnClickedEnableBK)
	ON_EN_CHANGE(IDC_EDIT_SRC, OnEnChangeEditSrc)
	ON_EN_CHANGE(IDC_EDIT_DST, OnEnChangeEditDst)
	ON_BN_CLICKED(IDC_ALL_CLEAR, OnBnClickedAllClear)
	ON_BN_CLICKED(IDC_END_NONE, OnBnClickedEndNone)
	ON_BN_CLICKED(IDC_END_REBOOT, OnBnClickedEndReboot)
	ON_BN_CLICKED(IDC_END_SHUTDOWN, OnBnClickedEndShutdown)
	ON_BN_CLICKED(IDC_END_APP, OnBnClickedEndApp)
END_MESSAGE_MAP()


// CBackUpDlg メッセージ ハンドラ

BOOL CBackUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// 初期化
	CheckRadioButton( IDC_END_NONE, IDC_END_SHUTDOWN, IDC_END_NONE );
	LCSetData();
	Refresh();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CBackUpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CBackUpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CBackUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBackUpDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ( pNMLV )
	{
		if ( pNMLV->iItem != m_idx )
		{
			m_idx = pNMLV->iItem;
			Refresh();
		}
	}
	*pResult = 0;
}


void CBackUpDlg::LCSetData()
{
	LV_COLUMN	lvCol;
	LV_ITEM		lvItem;
	char		str[MAX_PATH];

	// Column共通設定
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	lvCol.fmt  = LVCFMT_LEFT;

	// Item共通設定
	lvItem.mask      = LVIF_TEXT | LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state     = 0;

	// ラベル挿入
	lvCol.cx   = LC_VAL_WIDTH;
	InsertListCtrl(lvCol,SUBITEM_ENABLE_BK,STR_ENABLE_BK);
	InsertListCtrl(lvCol,SUBITEM_NUMBER,STR_NUMBER);
	lvCol.cx   = LC_STR_WIDTH;
	InsertListCtrl(lvCol,SUBITEM_SRC_PATH,STR_ORG_PATH);
	InsertListCtrl(lvCol,SUBITEM_DST_PATH,STR_NEW_PATH);
	lvCol.cx   = LC_VAL_WIDTH;
	InsertListCtrl(lvCol,SUBITEM_SUBDIRECTORY,STR_SUBDIRECTORY);
	InsertListCtrl(lvCol,SUBITEM_DIFF_FILE,STR_DIFF_FILE);
	InsertListCtrl(lvCol,SUBITEM_OVERWRITE,STR_OVERWRITE);

	m_idx = 0;
	ReadSetting();
	for( int i=0,idx=0; i < MAXNUM_IDX; i++ )
	{
		lvItem.iItem = idx;

		//バックアップ有効
		if ( m_bkStruct[i].bBkEnable )
			sprintf_s(str,sizeof(str),STR_ENABLE);
		else
			sprintf_s(str,sizeof(str),STR_DISABLE);
		lvItem.iSubItem  = SUBITEM_ENABLE_BK;
		lvItem.pszText   = str;
		m_listctl.InsertItem(&lvItem);

		//No
		sprintf_s(str,sizeof(str),"%d",i+1);
		lvItem.iSubItem  = SUBITEM_NUMBER;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		//元パス
		lvItem.iSubItem  = SUBITEM_SRC_PATH;
		lvItem.pszText   = (LPSTR)m_bkStruct[i].strSrcPath.GetString();
		m_listctl.SetItem(&lvItem);

		//先パス
		lvItem.iSubItem  = SUBITEM_DST_PATH;
		lvItem.pszText   = (LPSTR)m_bkStruct[i].strDstPath.GetString();
		m_listctl.SetItem(&lvItem);

		// サブディレクトリも対象
		if ( m_bkStruct[i].opt & SUBDIR_MASK )
			sprintf_s(str,sizeof(str),STR_ON);
		else
			sprintf_s(str,sizeof(str),STR_OFF);
		lvItem.iSubItem  = SUBITEM_SUBDIRECTORY;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		//差分のみ対象
		if ( m_bkStruct[i].opt & DIFF_MASK )
			sprintf_s(str,sizeof(str),STR_ON);
		else
			sprintf_s(str,sizeof(str),STR_OFF);
		lvItem.iSubItem  = SUBITEM_DIFF_FILE;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		//上書き確認する
		if ( m_bkStruct[i].opt & OVERWRITE_MASK )
			sprintf_s(str,sizeof(str),STR_ON);
		else
			sprintf_s(str,sizeof(str),STR_OFF);
		lvItem.iSubItem  = SUBITEM_OVERWRITE;
		lvItem.pszText   = str;
		m_listctl.SetItem(&lvItem);

		idx++;
	}

	m_listctl.SetExtendedStyle(LVS_EX_FULLROWSELECT); //行で選択するように指定
	m_listctl.SetItemState(m_idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}

void CBackUpDlg::InsertListCtrl(LV_COLUMN lvCol, int idx, CString name)
{
	lvCol.iSubItem = idx;
	lvCol.pszText  = (LPSTR)name.GetString();
	m_listctl.InsertColumn(idx, &lvCol);
}

void CBackUpDlg::OnBackupStart()
{
	char str[MAX_PATH];
	BOOL bErrFind = FALSE;

	strcpy_s(str,sizeof(str),"バックアップ中");
	SetDlgItemText(IDC_LABEL, str);
	
	bErrFind = BackUpProc();
	if ( bErrFind == FALSE )
	{
		strcpy_s(str,sizeof(str),"バックアップ正常終了");
	}
	else
	{
		strcpy_s(str,sizeof(str),"バックアップに失敗しました");
	}
	SetDlgItemText(IDC_LABEL, str);

	if ( m_end == END_SHUTDOWN )
	{
		system("shutdown -s -t 0");
	}
	else if ( m_end == END_REBOOT )
	{
		system("shutdown -r -t 0");
	}
	else if ( m_end == END_APP )
	{
		CDialog::OnOK();
	}
}

BOOL CBackUpDlg::GetDirectory(TCHAR * dir) 
{
	LPITEMIDLIST pidlSelected = NULL;
	BROWSEINFO browseInfo = { 0 };
	char folderName[MAX_PATH] = { 0 };
	BOOL rt = FALSE;

	browseInfo.hwndOwner = this->m_hWnd;
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = folderName;
	CString title;
	title="目的のフォルダを選択して下ちぃ。。";
	browseInfo.lpszTitle = title.GetBuffer(10);
	browseInfo.ulFlags = 0;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0; 
	browseInfo.ulFlags = 0x41;

	title.ReleaseBuffer();

	pidlSelected = SHBrowseForFolder(&browseInfo);
	if (pidlSelected != NULL)
	{
		SHGetPathFromIDList(pidlSelected, dir);
		rt = TRUE;
	}
	return rt;
}

BOOL CBackUpDlg::BackUpProc()
{
	BOOL bReturn = FALSE;
	TCHAR cmdName[MAX_PATH];
	TCHAR ExeName[MAX_PATH];
	CString cmd;

	for(int idx=0; idx < MAXNUM_IDX; idx++)
	{
		if(m_bkStruct[idx].bBkEnable == FALSE ||
			m_bkStruct[idx].strSrcPath.Compare("") == 0 ||
			m_bkStruct[idx].strDstPath.Compare("") == 0 )
		{
			continue;
		}

		memset(cmdName,0,sizeof(cmdName));
		memset(ExeName,0,sizeof(ExeName));

		strcpy_s(cmdName,sizeof(cmdName), "xcopy");
		if(m_bkStruct[idx].opt & SUBDIR_MASK)
		{
			strcat_s(cmdName,sizeof(cmdName)," /E");	//	/E　ディレクトリごとコピー
		}

		if(m_bkStruct[idx].opt & DIFF_MASK)
		{
			strcat_s(cmdName,sizeof(cmdName)," /D");	//	/D　新しいファイルのみコピー
		}

		if(m_bkStruct[idx].opt & OVERWRITE_MASK)
		{
			strcat_s(cmdName,sizeof(cmdName)," /-Y");	//	/-Y　上書きの確認を表示
		}
		else
		{
			strcat_s(cmdName,sizeof(cmdName)," /Y");	//	/-Y　上書きの確認を表示しない
		}

		strcat_s(cmdName,sizeof(cmdName)," /I");	//	/I　受け側ディレクトリを新規作成

		strcat_s(cmdName,sizeof(cmdName)," /H");	//	/H　隠しファイルやシステムファイルも対象

		strcat_s(cmdName,sizeof(cmdName)," /R");	//	/R	読み取り専用でも上書き

//		strcat_s(cmdName,sizeof(cmdName)," /Q");	//	/Q　コピー中、ファイル名を表示しない
//		strcat_s(cmdName,sizeof(cmdName)," /F");	//	/F　コピー中、送り側と受け側の全ファイル名表示
//		strcat_s(cmdName,sizeof(cmdName)," /L");	//	/L　コピー対象のファイル名を表示⇒コピーせず表示のみ

		sprintf_s(ExeName,sizeof(ExeName),"%s \"%s\" \"%s\"\n",cmdName,m_bkStruct[idx].strSrcPath,m_bkStruct[idx].strDstPath);

		if ( m_FileOut )
		{
			cmd.Append(ExeName);
		}
		else
		{
			int rt;
			rt = system(ExeName);
			if ( bReturn == FALSE && rt )
			{
				bReturn = rt;
			}
		}
	}

	if ( m_FileOut )
	{
		WriteBatchFile(cmd);
	}

	return bReturn ;
}


void CBackUpDlg::OnBrowseSrc()
{
	TCHAR pathName[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_EDIT_SRC, pathName,sizeof(pathName));
	if(GetDirectory(pathName) == TRUE){
		SetDlgItemText(IDC_EDIT_SRC, pathName);
		m_bkStruct[m_idx].strSrcPath.SetString(pathName);
		UpdateSrcPath(pathName);
	}
}

void CBackUpDlg::OnBrowseDest()
{
	TCHAR pathName[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_EDIT_SRC, pathName,sizeof(pathName));
	if(GetDirectory(pathName) == TRUE){
		SetDlgItemText(IDC_EDIT_DST, pathName);
		m_bkStruct[m_idx].strDstPath.SetString(pathName);
		UpdateDstPath(pathName);
	}
}

void CBackUpDlg::OnEnChangeEditSrc()
{
	char str[MAX_PATH];

	GetDlgItemText(IDC_EDIT_SRC, str, sizeof(str));
	m_bkStruct[m_idx].strSrcPath.SetString(str);
	m_listctl.SetItemText(m_idx, SUBITEM_SRC_PATH, str);

}

void CBackUpDlg::OnEnChangeEditDst()
{
	char str[MAX_PATH];

	GetDlgItemText(IDC_EDIT_DST, str, sizeof(str));
	m_bkStruct[m_idx].strDstPath.SetString(str);
	m_listctl.SetItemText(m_idx, SUBITEM_DST_PATH, str);

}

void CBackUpDlg::OnBnClickedEnableBK()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_SELECT_BACKUP) == BST_CHECKED);
	m_bkStruct[m_idx].bBkEnable = bOn;
	UpdateEnableBK(bOn);
}

void CBackUpDlg::OnBnClickedSubdir()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_SUBDIR) == BST_CHECKED);
	if ( bOn )
		m_bkStruct[m_idx].opt |= SUBDIR_MASK;
	else
		m_bkStruct[m_idx].opt &= ~SUBDIR_MASK;
	UpdateSubDirectory(m_bkStruct[m_idx].opt);
}

void CBackUpDlg::OnDiff()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_DIFF) == BST_CHECKED);
	if ( bOn )
	{
		CheckDlgButton(IDC_OVERWRITE, BST_UNCHECKED);
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(FALSE);
		m_bkStruct[m_idx].opt |= DIFF_MASK;
		m_bkStruct[m_idx].opt &= ~OVERWRITE_MASK;
		UpdateOverWrite(m_bkStruct[m_idx].opt);
	}
	else
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(TRUE);
		m_bkStruct[m_idx].opt &= ~DIFF_MASK;
	}

	UpdateDiffFile(m_bkStruct[m_idx].opt);
}

void CBackUpDlg::OnBnClickedOverWrite()
{
	BOOL bOn;
		
	bOn = (IsDlgButtonChecked(IDC_OVERWRITE) == BST_CHECKED);
	if ( bOn )
	{
		m_bkStruct[m_idx].opt |= OVERWRITE_MASK;
	}
	else
	{
		m_bkStruct[m_idx].opt &= ~OVERWRITE_MASK;
	}
	UpdateOverWrite(m_bkStruct[m_idx].opt);
}

void CBackUpDlg::UpdateEnableBK(BOOL bChk, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( bChk == TRUE )
	{
		CheckDlgButton(IDC_SELECT_BACKUP, BST_CHECKED);
		str.SetString(STR_ENABLE);
	}
	else
	{
		CheckDlgButton(IDC_SELECT_BACKUP, BST_UNCHECKED);
		str.SetString(STR_DISABLE);
	}

	m_listctl.SetItemText(nItem, SUBITEM_ENABLE_BK, str);
}

void CBackUpDlg::UpdateSubDirectory(DWORD opt, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( opt & SUBDIR_MASK )
	{
		CheckDlgButton(IDC_SUBDIR, BST_CHECKED);
		str.SetString(STR_ON);
	}
	else
	{
		CheckDlgButton(IDC_SUBDIR, BST_UNCHECKED);
		str.SetString(STR_OFF);
	}

	m_listctl.SetItemText(nItem, SUBITEM_SUBDIRECTORY, str);
}

void CBackUpDlg::UpdateDiffFile(DWORD opt, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( opt & DIFF_MASK )
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(FALSE);
		CheckDlgButton(IDC_DIFF, BST_CHECKED);
		str.SetString(STR_ON);
	}
	else
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(TRUE);
		CheckDlgButton(IDC_DIFF, BST_UNCHECKED);
		str.SetString(STR_OFF);
	}

	m_listctl.SetItemText(nItem, SUBITEM_DIFF_FILE, str);
}

void CBackUpDlg::UpdateOverWrite(DWORD opt, int idx)
{
	CString str;
	int nItem = GetIdx(idx);

	if ( opt & OVERWRITE_MASK )
	{
		CheckDlgButton(IDC_OVERWRITE, BST_CHECKED);
		str.SetString(STR_ON);
	}
	else
	{
		CheckDlgButton(IDC_OVERWRITE, BST_UNCHECKED);
		str.SetString(STR_OFF);
	}

	m_listctl.SetItemText(nItem, SUBITEM_OVERWRITE, str);
}

void CBackUpDlg::UpdateSrcPath(char *str, int idx)
{
	int nItem = GetIdx(idx);

	SetDlgItemText(IDC_EDIT_SRC, str);
	m_listctl.SetItemText(nItem, SUBITEM_SRC_PATH, str);
}

void CBackUpDlg::UpdateDstPath(char *str, int idx)
{
	int nItem = GetIdx(idx);

	SetDlgItemText(IDC_EDIT_DST, str);
	m_listctl.SetItemText(nItem, SUBITEM_DST_PATH, str);
}

void CBackUpDlg::Refresh()
{
	UpdateEnableBK(m_bkStruct[m_idx].bBkEnable);
	UpdateSubDirectory(m_bkStruct[m_idx].opt);
	UpdateDiffFile(m_bkStruct[m_idx].opt);
	UpdateOverWrite(m_bkStruct[m_idx].opt);
	UpdateSrcPath((LPSTR)m_bkStruct[m_idx].strSrcPath.GetString());
	UpdateDstPath((LPSTR)m_bkStruct[m_idx].strDstPath.GetString());
}

void CBackUpDlg::OnBnClickedAllClear()
{
	// TODO：バグってる？
	for(int i=0; i < MAXNUM_IDX; i++)
	{
		memcpy(&m_bkStruct[i], &m_bkDefault, sizeof(m_bkDefault));
	}
	Refresh();
}

void CBackUpDlg::OnBnClickedSaveSetting()
{
	BOOL bReturn = WriteSetting();

	if ( bReturn == TRUE )
	{
		SetDlgItemText(IDC_LABEL, STR_SAVE_SET_SUCCESS);
	}
	else
	{
		SetDlgItemText(IDC_LABEL, STR_SAVE_SET_ERROR);
	}
}

BOOL CBackUpDlg::WriteSetting()
{
	char path[MAX_PATH];
	BOOL bReturn = FALSE;

	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		CStdioFile	file;

		strcat_s(path,sizeof(path),SET_FILE_NAME);
		if ( file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText) )
		{
			char str[MAX_PATH];

			for(int i=0;i<MAXNUM_IDX;i++)
			{
				if( m_bkStruct[i].strSrcPath.Compare("") == 0 ||
					m_bkStruct[i].strDstPath.Compare("") == 0 )
				{
					continue;
				}

				sprintf_s(str, sizeof(str),"%d,%d,%s,%s\n",
					m_bkStruct[i].bBkEnable,
					m_bkStruct[i].opt,
					m_bkStruct[i].strSrcPath,
					m_bkStruct[i].strDstPath);
				file.WriteString(str);
				bReturn = TRUE;
			}
			file.Close();
		}
	}

	return bReturn;
}

BOOL CBackUpDlg::ReadSetting()
{
	char path[MAX_PATH];
	BOOL bReturn = FALSE;
	int idx=0;

	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		CStdioFile	file;

		strcat_s(path,sizeof(path),SET_FILE_NAME);
		if ( file.Open(path, CFile::modeRead | CFile::typeText) )
		{
			CString str;
			int curPos;

			while( file.ReadString(str) )
			{
				curPos=0;
				m_bkStruct[idx].bBkEnable = atoi(str.Tokenize(",",curPos));
				m_bkStruct[idx].opt = atoi(str.Tokenize(",",curPos));
				m_bkStruct[idx].strSrcPath = str.Tokenize(",",curPos);
				m_bkStruct[idx].strDstPath = str.Tokenize(",",curPos);
				idx++;
			}

			bReturn = TRUE;
			file.Close();
		}
	}

	for(;idx<MAXNUM_IDX;idx++)
	{
		memcpy(&m_bkStruct[idx],&m_bkDefault,sizeof(m_bkDefault));
	}

	return bReturn;
}

void CBackUpDlg::WriteBatchFile(CString cmd)
{
	char path[MAX_PATH];
	BOOL bReturn = FALSE;

	if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK )
	{
		CStdioFile	file;

		strcat_s(path,sizeof(path),BAT_FILE_NAME);
		if ( file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText) )
		{
			file.WriteString(cmd);
			file.Close();
		}
	}
}

int CBackUpDlg::GetIdx(int idx)
{
	int nItem = idx;

	if ( nItem == CURRENT_IDX )
		nItem = m_idx;

	return nItem;
}

BOOL CBackUpDlg::IsBlankData(BACKUP *BkStruct)
{
	BOOL bReturn = FALSE;

	if ( BkStruct->strSrcPath.Compare("") == 0 &&
		 BkStruct->strDstPath.Compare("") == 0 )
	{
		bReturn = TRUE;
	}

	return bReturn;
}

void CBackUpDlg::OnBnClickedEndNone()
{
	m_end = END_NONE;
}

void CBackUpDlg::OnBnClickedEndApp()
{
	m_end = END_APP;
}

void CBackUpDlg::OnBnClickedEndReboot()
{
	m_end = END_REBOOT;
}

void CBackUpDlg::OnBnClickedEndShutdown()
{
	m_end = END_SHUTDOWN;
}

void CBackUpDlg::OnCancel()
{
	m_FileOut = TRUE;
	BackUpProc();

	CDialog::OnCancel();
}

