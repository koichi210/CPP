// back_up_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "back_up.h"
#include "back_up_dlg.h"
#include "common_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// リストのカラム
	enum
	{
		SUBITEM_ENABLE_BK,
		SUBITEM_NUMBER,
		SUBITEM_SRC_PATH,
		SUBITEM_DST_PATH,
		SUBITEM_SUBDIRECTORY,
		SUBITEM_DIFF_FILE,
		SUBITEM_OVERWRITE,
	};

	constexpr int LC_VAL_WIDTH = 25;	// リストのカラム幅（No・オプション）
	constexpr int LC_STR_WIDTH = 170;	// リストのカラム幅（パス）

	constexpr LPCTSTR STR_ON		= _T("有");
	constexpr LPCTSTR STR_OFF		= _T("無");
	constexpr LPCTSTR STR_ENABLE	= _T("○");
	constexpr LPCTSTR STR_DISABLE	= _T("×");

	constexpr LPCTSTR SET_FILE_NAME	= _T("\\BackUp.dat");
	constexpr LPCTSTR BAT_FILE_NAME	= _T("\\BackUp.bat");

	constexpr LPCTSTR BROWSE_TITLE	= _T("目的のフォルダを選択して下ちぃ。。");

	// マイドキュメント直下のファイルパス（取得できなければ空）
	CString GetDocumentsFilePath(LPCTSTR fileName)
	{
		TCHAR path[MAX_PATH];
		if (SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, 0, path) != S_OK)
		{
			return CString();
		}
		return CString(path) + fileName;
	}
}

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class CAboutDlg : public CDialog
{
public:
	enum { IDD = IDD_ABOUTBOX };

	CAboutDlg() : CDialog(IDD) {}
};

/////////////////////////////////////////////////////////////////////////////
// CBackUpDlg

CBackUpDlg::CBackUpDlg(CWnd* pParent)
	: CDialog(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBackUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CBackUpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, &CBackUpDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BACKUP_START, &CBackUpDlg::OnBackupStart)
	ON_BN_CLICKED(IDC_DIFF, &CBackUpDlg::OnDiff)
	ON_BN_CLICKED(IDC_BROWSE_SRC, &CBackUpDlg::OnBrowseSrc)
	ON_BN_CLICKED(IDC_BROWSE_DEST, &CBackUpDlg::OnBrowseDest)
	ON_BN_CLICKED(IDC_SAVE_SETTING, &CBackUpDlg::OnBnClickedSaveSetting)
	ON_BN_CLICKED(IDC_SUBDIR, &CBackUpDlg::OnBnClickedSubdir)
	ON_BN_CLICKED(IDC_OVERWRITE, &CBackUpDlg::OnBnClickedOverWrite)
	ON_BN_CLICKED(IDC_SELECT_BACKUP, &CBackUpDlg::OnBnClickedEnableBK)
	ON_EN_CHANGE(IDC_EDIT_SRC, &CBackUpDlg::OnEnChangeEditSrc)
	ON_EN_CHANGE(IDC_EDIT_DST, &CBackUpDlg::OnEnChangeEditDst)
	ON_BN_CLICKED(IDC_ALL_CLEAR, &CBackUpDlg::OnBnClickedAllClear)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_END_NONE, IDC_END_SHUTDOWN, &CBackUpDlg::OnEndOption)
END_MESSAGE_MAP()

BOOL CBackUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CheckRadioButton(IDC_END_NONE, IDC_END_SHUTDOWN, IDC_END_NONE);
	InitListCtrl();
	Refresh();

	return TRUE;
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

// 最小化時のアイコン描画（ダイアログがメインウィンドウのため自前で描く）
void CBackUpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CBackUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBackUpDlg::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV && pNMLV->iItem != m_nCurIdx)
	{
		m_nCurIdx = pNMLV->iItem;
		Refresh();
	}
	*pResult = 0;
}

void CBackUpDlg::InitListCtrl()
{
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	lvCol.fmt  = LVCFMT_LEFT;

	lvCol.cx = LC_VAL_WIDTH;
	InsertListColumn(lvCol, SUBITEM_ENABLE_BK, _T("バックアップ有効"));
	InsertListColumn(lvCol, SUBITEM_NUMBER, _T("No"));
	lvCol.cx = LC_STR_WIDTH;
	InsertListColumn(lvCol, SUBITEM_SRC_PATH, _T("元フォルダ"));
	InsertListColumn(lvCol, SUBITEM_DST_PATH, _T("先フォルダ"));
	lvCol.cx = LC_VAL_WIDTH;
	InsertListColumn(lvCol, SUBITEM_SUBDIRECTORY, _T("サブディレクトリも対象"));
	InsertListColumn(lvCol, SUBITEM_DIFF_FILE, _T("差分ファイルのみ対象"));
	InsertListColumn(lvCol, SUBITEM_OVERWRITE, _T("上書きの確認を表示"));

	m_nCurIdx = 0;
	ReadSetting();

	LVITEM lvItem;
	lvItem.mask      = LVIF_TEXT | LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state     = 0;

	for (int i = 0; i < MAX_ENTRY; i++)
	{
		// 行の追加・設定中に LVN_ITEMCHANGED → Refresh が走り得るため、値を複製して使う
		const BACKUP entry = m_entries[i];

		CString number;
		number.Format(_T("%d"), i + 1);

		const struct
		{
			int		nSubItem;
			LPCTSTR	text;
		} columns[] =
		{
			{ SUBITEM_ENABLE_BK,	entry.bBkEnable ? STR_ENABLE : STR_DISABLE },
			{ SUBITEM_NUMBER,		number.GetString() },
			{ SUBITEM_SRC_PATH,		entry.strSrcPath.GetString() },
			{ SUBITEM_DST_PATH,		entry.strDstPath.GetString() },
			{ SUBITEM_SUBDIRECTORY,	(entry.opt & BACKUP::OPT_SUBDIR) ? STR_ON : STR_OFF },
			{ SUBITEM_DIFF_FILE,	(entry.opt & BACKUP::OPT_DIFF) ? STR_ON : STR_OFF },
			{ SUBITEM_OVERWRITE,	(entry.opt & BACKUP::OPT_OVERWRITE) ? STR_ON : STR_OFF },
		};

		lvItem.iItem = i;
		for (const auto& column : columns)
		{
			lvItem.iSubItem = column.nSubItem;
			lvItem.pszText  = const_cast<LPTSTR>(column.text);
			// 先頭カラムで行を作り、残りはその行に設定する
			if (column.nSubItem == SUBITEM_ENABLE_BK)
				m_listCtrl.InsertItem(&lvItem);
			else
				m_listCtrl.SetItem(&lvItem);
		}
	}

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrl.SetItemState(m_nCurIdx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}

void CBackUpDlg::InsertListColumn(LVCOLUMN lvCol, int nSubItem, LPCTSTR name)
{
	lvCol.iSubItem = nSubItem;
	lvCol.pszText  = const_cast<LPTSTR>(name);
	m_listCtrl.InsertColumn(nSubItem, &lvCol);
}

void CBackUpDlg::OnBackupStart()
{
	SetDlgItemText(IDC_LABEL, _T("バックアップ中"));

	if (RunBackup(false))
	{
		SetDlgItemText(IDC_LABEL, _T("バックアップ正常終了"));
	}
	else
	{
		SetDlgItemText(IDC_LABEL, _T("バックアップに失敗しました"));
	}

	switch (m_endAction)
	{
	case EndAction::Shutdown:
		system("shutdown -s -t 0");
		break;
	case EndAction::Reboot:
		system("shutdown -r -t 0");
		break;
	case EndAction::App:
		CDialog::OnOK();
		break;
	default:
		break;
	}
}

// 有効な設定ごとに xcopy を実行する。bWriteBatchOnly なら実行せずバッチファイルに書き出す
// 戻り値: すべての xcopy が成功したか
bool CBackUpDlg::RunBackup(bool bWriteBatchOnly)
{
	bool bSuccess = true;
	CString batch;

	for (const BACKUP& entry : m_entries)
	{
		if (entry.bBkEnable == FALSE || entry.strSrcPath.IsEmpty() || entry.strDstPath.IsEmpty())
		{
			continue;
		}

		CString cmdName = _T("xcopy");
		if (entry.opt & BACKUP::OPT_SUBDIR)
		{
			cmdName += _T(" /E");		// ディレクトリごとコピー
		}
		if (entry.opt & BACKUP::OPT_DIFF)
		{
			cmdName += _T(" /D");		// 新しいファイルのみコピー
		}
		if (entry.opt & BACKUP::OPT_OVERWRITE)
		{
			cmdName += _T(" /-Y");		// 上書きの確認を表示
		}
		else
		{
			cmdName += _T(" /Y");		// 上書きの確認を表示しない
		}
		cmdName += _T(" /I");			// 受け側ディレクトリを新規作成
		cmdName += _T(" /H");			// 隠しファイルやシステムファイルも対象
		cmdName += _T(" /R");			// 読み取り専用でも上書き

		CString command;
		command.Format(_T("%s \"%s\" \"%s\"\n"), cmdName.GetString(), entry.strSrcPath.GetString(), entry.strDstPath.GetString());

		if (bWriteBatchOnly)
		{
			batch += command;
		}
		else if (system(command) != 0)
		{
			bSuccess = false;
		}
	}

	if (bWriteBatchOnly)
	{
		WriteBatchFile(batch);
	}

	return bSuccess;
}

void CBackUpDlg::OnBrowseSrc()
{
	CString path;
	if (BrowseFolder(m_hWnd, BROWSE_TITLE, path))
	{
		m_entries[m_nCurIdx].strSrcPath = path;
		UpdatePath(IDC_EDIT_SRC, SUBITEM_SRC_PATH, path);
	}
}

void CBackUpDlg::OnBrowseDest()
{
	CString path;
	if (BrowseFolder(m_hWnd, BROWSE_TITLE, path))
	{
		m_entries[m_nCurIdx].strDstPath = path;
		UpdatePath(IDC_EDIT_DST, SUBITEM_DST_PATH, path);
	}
}

void CBackUpDlg::OnEnChangeEditSrc()
{
	CString str;
	GetDlgItemText(IDC_EDIT_SRC, str);
	m_entries[m_nCurIdx].strSrcPath = str;
	m_listCtrl.SetItemText(m_nCurIdx, SUBITEM_SRC_PATH, str);
}

void CBackUpDlg::OnEnChangeEditDst()
{
	CString str;
	GetDlgItemText(IDC_EDIT_DST, str);
	m_entries[m_nCurIdx].strDstPath = str;
	m_listCtrl.SetItemText(m_nCurIdx, SUBITEM_DST_PATH, str);
}

void CBackUpDlg::OnBnClickedEnableBK()
{
	BOOL bOn = (IsDlgButtonChecked(IDC_SELECT_BACKUP) == BST_CHECKED);
	m_entries[m_nCurIdx].bBkEnable = bOn;
	UpdateEnableBK(bOn);
}

void CBackUpDlg::OnBnClickedSubdir()
{
	SetOption(BACKUP::OPT_SUBDIR, IsDlgButtonChecked(IDC_SUBDIR) == BST_CHECKED);
	UpdateSubDirectory(m_entries[m_nCurIdx].opt);
}

// 差分コピー時は上書き確認を使わない
void CBackUpDlg::OnDiff()
{
	if (IsDlgButtonChecked(IDC_DIFF) == BST_CHECKED)
	{
		CheckDlgButton(IDC_OVERWRITE, BST_UNCHECKED);
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(FALSE);
		SetOption(BACKUP::OPT_DIFF, true);
		SetOption(BACKUP::OPT_OVERWRITE, false);
		UpdateOverWrite(m_entries[m_nCurIdx].opt);
	}
	else
	{
		GetDlgItem(IDC_OVERWRITE)->EnableWindow(TRUE);
		SetOption(BACKUP::OPT_DIFF, false);
	}

	UpdateDiffFile(m_entries[m_nCurIdx].opt);
}

void CBackUpDlg::OnBnClickedOverWrite()
{
	SetOption(BACKUP::OPT_OVERWRITE, IsDlgButtonChecked(IDC_OVERWRITE) == BST_CHECKED);
	UpdateOverWrite(m_entries[m_nCurIdx].opt);
}

void CBackUpDlg::SetOption(DWORD mask, bool bOn)
{
	DWORD& opt = m_entries[m_nCurIdx].opt;
	if (bOn)
		opt |= mask;
	else
		opt &= ~mask;
}

// チェックボックスとリストの表示をそろえる
void CBackUpDlg::UpdateCheckItem(int nCtrlId, int nSubItem, bool bOn, LPCTSTR pszOn, LPCTSTR pszOff)
{
	CheckDlgButton(nCtrlId, bOn ? BST_CHECKED : BST_UNCHECKED);
	m_listCtrl.SetItemText(m_nCurIdx, nSubItem, bOn ? pszOn : pszOff);
}

void CBackUpDlg::UpdateEnableBK(BOOL bChk)
{
	UpdateCheckItem(IDC_SELECT_BACKUP, SUBITEM_ENABLE_BK, bChk == TRUE, STR_ENABLE, STR_DISABLE);
}

void CBackUpDlg::UpdateSubDirectory(DWORD opt)
{
	UpdateCheckItem(IDC_SUBDIR, SUBITEM_SUBDIRECTORY, (opt & BACKUP::OPT_SUBDIR) != 0, STR_ON, STR_OFF);
}

void CBackUpDlg::UpdateDiffFile(DWORD opt)
{
	const bool bOn = (opt & BACKUP::OPT_DIFF) != 0;
	GetDlgItem(IDC_OVERWRITE)->EnableWindow(bOn ? FALSE : TRUE);
	UpdateCheckItem(IDC_DIFF, SUBITEM_DIFF_FILE, bOn, STR_ON, STR_OFF);
}

void CBackUpDlg::UpdateOverWrite(DWORD opt)
{
	UpdateCheckItem(IDC_OVERWRITE, SUBITEM_OVERWRITE, (opt & BACKUP::OPT_OVERWRITE) != 0, STR_ON, STR_OFF);
}

void CBackUpDlg::UpdatePath(int nEditId, int nSubItem, LPCTSTR path)
{
	SetDlgItemText(nEditId, path);
	m_listCtrl.SetItemText(m_nCurIdx, nSubItem, path);
}

void CBackUpDlg::Refresh()
{
	// エディットへの設定で EN_CHANGE が走り m_entries に書き戻されるため、値を複製して使う
	const BACKUP entry = m_entries[m_nCurIdx];

	UpdateEnableBK(entry.bBkEnable);
	UpdateSubDirectory(entry.opt);
	UpdateDiffFile(entry.opt);
	UpdateOverWrite(entry.opt);
	UpdatePath(IDC_EDIT_SRC, SUBITEM_SRC_PATH, entry.strSrcPath);
	UpdatePath(IDC_EDIT_DST, SUBITEM_DST_PATH, entry.strDstPath);
}

void CBackUpDlg::OnBnClickedAllClear()
{
	for (BACKUP& entry : m_entries)
	{
		entry = BACKUP();
	}
	Refresh();
}

void CBackUpDlg::OnBnClickedSaveSetting()
{
	if (WriteSetting())
	{
		SetDlgItemText(IDC_LABEL, _T("設定値を保存しました。"));
	}
	else
	{
		SetDlgItemText(IDC_LABEL, _T("設定値保存に失敗しました。"));
	}
}

// 書式: 有効,オプション,元フォルダ,先フォルダ（1行1件）
BOOL CBackUpDlg::WriteSetting()
{
	const CString path = GetDocumentsFilePath(SET_FILE_NAME);
	if (path.IsEmpty())
	{
		return FALSE;
	}

	CStdioFile file;
	if (!file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
		return FALSE;
	}

	BOOL bReturn = FALSE;
	for (const BACKUP& entry : m_entries)
	{
		if (entry.strSrcPath.IsEmpty() || entry.strDstPath.IsEmpty())
		{
			continue;
		}

		CString line;
		line.Format(_T("%d,%d,%s,%s\n"), entry.bBkEnable, entry.opt, entry.strSrcPath.GetString(), entry.strDstPath.GetString());
		file.WriteString(line);
		bReturn = TRUE;
	}
	file.Close();

	return bReturn;
}

BOOL CBackUpDlg::ReadSetting()
{
	BOOL bReturn = FALSE;
	int idx = 0;

	const CString path = GetDocumentsFilePath(SET_FILE_NAME);
	CStdioFile file;
	if (!path.IsEmpty() && file.Open(path, CFile::modeRead | CFile::typeText))
	{
		CString str;
		while (idx < MAX_ENTRY && file.ReadString(str))
		{
			int curPos = 0;
			BACKUP& entry = m_entries[idx];
			entry.bBkEnable  = _ttoi(str.Tokenize(_T(","), curPos));
			entry.opt        = _ttoi(str.Tokenize(_T(","), curPos));
			entry.strSrcPath = str.Tokenize(_T(","), curPos);
			entry.strDstPath = str.Tokenize(_T(","), curPos);
			idx++;
		}

		bReturn = TRUE;
		file.Close();
	}

	for (; idx < MAX_ENTRY; idx++)
	{
		m_entries[idx] = BACKUP();
	}

	return bReturn;
}

void CBackUpDlg::WriteBatchFile(const CString& cmd)
{
	const CString path = GetDocumentsFilePath(BAT_FILE_NAME);
	if (path.IsEmpty())
	{
		return;
	}

	CStdioFile file;
	if (file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
		file.WriteString(cmd);
		file.Close();
	}
}

// 終了オプションのラジオボタン（IDC_END_NONE からの並びが EndAction の順）
void CBackUpDlg::OnEndOption(UINT nID)
{
	m_endAction = static_cast<EndAction>(nID - IDC_END_NONE);
}

// 閉じるとき（×・Esc）は、現在の設定を実行用バッチファイルにも書き出す
void CBackUpDlg::OnCancel()
{
	RunBackup(true);

	CDialog::OnCancel();
}
