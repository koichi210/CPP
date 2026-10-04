// back_up_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "back_up.h"
#include "back_up_dlg.h"
#include "common_util.h"

#include <algorithm>
#include <iterator>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// リストのカラム
	enum
	{
		kSubitemEnableBk,
		kSubitemNumber,
		kSubitemSrcPath,
		kSubitemDstPath,
		kSubitemSubdirectory,
		kSubitemDiffFile,
		kSubitemOverwrite,
	};

	constexpr int kLcValWidth = 25;	// リストのカラム幅（No・オプション）
	constexpr int kLcStrWidth = 170;	// リストのカラム幅（パス）

	constexpr LPCTSTR kStrOn		= _T("有");
	constexpr LPCTSTR kStrOff		= _T("無");
	constexpr LPCTSTR kStrEnable	= _T("○");
	constexpr LPCTSTR kStrDisable	= _T("×");

	constexpr LPCTSTR kSetFileName	= _T("BackUp.dat");
	constexpr LPCTSTR kBatFileName	= _T("BackUp.bat");

	constexpr LPCTSTR kBrowseTitle	= _T("目的のフォルダを選択して下ちぃ。。");

	// マイドキュメント直下のファイルパス（取得できなければ空）
	CString GetDocumentsFilePath(LPCTSTR file_name)
	{
		TCHAR path[MAX_PATH];
		if (SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, 0, path) != S_OK)
		{
			return CString();
		}
		CString file_path(path);
		AppendPath(file_path, file_name);
		return file_path;
	}
}

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class AboutDlg : public CDialog
{
public:
	enum { IDD = IDD_ABOUTBOX };

	AboutDlg() : CDialog(IDD) {}
};

/////////////////////////////////////////////////////////////////////////////
// BackUpDlg

BackUpDlg::BackUpDlg(CWnd* parent)
	: CDialog(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void BackUpDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
	DDX_Control(dx, IDC_LISTCTRL, list_ctrl_);
}

BEGIN_MESSAGE_MAP(BackUpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, &BackUpDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BACKUP_START, &BackUpDlg::OnBackupStart)
	ON_BN_CLICKED(IDC_DIFF, &BackUpDlg::OnDiff)
	ON_BN_CLICKED(IDC_BROWSE_SRC, &BackUpDlg::OnBrowseSrc)
	ON_BN_CLICKED(IDC_BROWSE_DEST, &BackUpDlg::OnBrowseDest)
	ON_BN_CLICKED(IDC_SAVE_SETTING, &BackUpDlg::OnBnClickedSaveSetting)
	ON_BN_CLICKED(IDC_SUBDIR, &BackUpDlg::OnBnClickedSubdir)
	ON_BN_CLICKED(IDC_OVERWRITE, &BackUpDlg::OnBnClickedOverWrite)
	ON_BN_CLICKED(IDC_SELECT_BACKUP, &BackUpDlg::OnBnClickedEnableBK)
	ON_EN_CHANGE(IDC_EDIT_SRC, &BackUpDlg::OnEnChangeEditSrc)
	ON_EN_CHANGE(IDC_EDIT_DST, &BackUpDlg::OnEnChangeEditDst)
	ON_BN_CLICKED(IDC_ALL_CLEAR, &BackUpDlg::OnBnClickedAllClear)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_END_NONE, IDC_END_SHUTDOWN, &BackUpDlg::OnEndOption)
END_MESSAGE_MAP()

BOOL BackUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	static_assert((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX && IDM_ABOUTBOX < 0xF000, "IDM_ABOUTBOX はシステムコマンドの範囲内にする");
	CMenu* sys_menu = GetSystemMenu(FALSE);
	if (sys_menu != nullptr)
	{
		CString about_menu;
		about_menu.LoadString(IDS_ABOUTBOX);
		if (!about_menu.IsEmpty())
		{
			sys_menu->AppendMenu(MF_SEPARATOR);
			sys_menu->AppendMenu(MF_STRING, IDM_ABOUTBOX, about_menu);
		}
	}

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	CheckRadioButton(IDC_END_NONE, IDC_END_SHUTDOWN, IDC_END_NONE);
	InitListCtrl();
	Refresh();

	return TRUE;
}

void BackUpDlg::OnSysCommand(UINT id, LPARAM l_param)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg dlg_about;
		dlg_about.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(id, l_param);
	}
}

// 最小化時のアイコン描画（ダイアログがメインウィンドウのため自前で描く）
void BackUpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		dc.DrawIcon((rect.Width() - icon_width + 1) / 2, (rect.Height() - icon_height + 1) / 2, icon_);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR BackUpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void BackUpDlg::OnLvnItemchangedList(NMHDR* nmhdr, LRESULT* result)
{
	LPNMLISTVIEW nmlv = reinterpret_cast<LPNMLISTVIEW>(nmhdr);

	if (nmlv && nmlv->iItem != cur_index_)
	{
		cur_index_ = nmlv->iItem;
		Refresh();
	}
	*result = 0;
}

void BackUpDlg::InitListCtrl()
{
	LVCOLUMN lv_col;
	lv_col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	lv_col.fmt  = LVCFMT_LEFT;

	lv_col.cx = kLcValWidth;
	InsertListColumn(lv_col, kSubitemEnableBk, _T("バックアップ有効"));
	InsertListColumn(lv_col, kSubitemNumber, _T("No"));
	lv_col.cx = kLcStrWidth;
	InsertListColumn(lv_col, kSubitemSrcPath, _T("元フォルダ"));
	InsertListColumn(lv_col, kSubitemDstPath, _T("先フォルダ"));
	lv_col.cx = kLcValWidth;
	InsertListColumn(lv_col, kSubitemSubdirectory, _T("サブディレクトリも対象"));
	InsertListColumn(lv_col, kSubitemDiffFile, _T("差分ファイルのみ対象"));
	InsertListColumn(lv_col, kSubitemOverwrite, _T("上書きの確認を表示"));

	cur_index_ = 0;
	ReadSetting();

	for (int i = 0; i < kMaxEntry; i++)
	{
		SetListRow(i, true);
	}

	list_ctrl_.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_ctrl_.SetItemState(cur_index_, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}

// 1行分の表示を entries_[row] に合わせる（insert なら行を作る）
void BackUpDlg::SetListRow(int row, bool insert)
{
	// 行の追加・設定中に LVN_ITEMCHANGED → Refresh が走り得るため、値を複製して使う
	const BackupSetting entry = entries_[row];

	CString number;
	number.Format(_T("%d"), row + 1);

	const struct
	{
		int		sub_item;
		LPCTSTR	text;
	} columns[] =
	{
		{ kSubitemEnableBk,		entry.bk_enable ? kStrEnable : kStrDisable },
		{ kSubitemNumber,		number.GetString() },
		{ kSubitemSrcPath,		entry.src_path.GetString() },
		{ kSubitemDstPath,		entry.dst_path.GetString() },
		{ kSubitemSubdirectory,	(entry.opt & BackupSetting::kOptSubdir) ? kStrOn : kStrOff },
		{ kSubitemDiffFile,		(entry.opt & BackupSetting::kOptDiff) ? kStrOn : kStrOff },
		{ kSubitemOverwrite,	(entry.opt & BackupSetting::kOptOverwrite) ? kStrOn : kStrOff },
	};

	// 行を作るときだけ選択状態も（非選択に）設定する
	LVITEM lv_item = {};
	lv_item.mask      = insert ? (LVIF_TEXT | LVIF_STATE) : LVIF_TEXT;
	lv_item.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lv_item.state     = 0;
	lv_item.iItem     = row;
	for (const auto& column : columns)
	{
		lv_item.iSubItem = column.sub_item;
		lv_item.pszText  = const_cast<LPTSTR>(column.text);
		// 先頭カラムで行を作り、残りはその行に設定する
		if (insert && column.sub_item == kSubitemEnableBk)
			list_ctrl_.InsertItem(&lv_item);
		else
			list_ctrl_.SetItem(&lv_item);
	}
}

void BackUpDlg::InsertListColumn(LVCOLUMN lv_col, int sub_item, LPCTSTR name)
{
	lv_col.iSubItem = sub_item;
	lv_col.pszText  = const_cast<LPTSTR>(name);
	list_ctrl_.InsertColumn(sub_item, &lv_col);
}

void BackUpDlg::OnBackupStart()
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

	switch (end_action_)
	{
	case EndAction::kShutdown:
		system("shutdown -s -t 0");
		break;
	case EndAction::kReboot:
		system("shutdown -r -t 0");
		break;
	case EndAction::kApp:
		CDialog::OnOK();
		break;
	default:
		break;
	}
}

// 有効な設定ごとに xcopy を実行する。write_batch_only なら実行せずバッチファイルに書き出す
// 戻り値: すべての xcopy が成功したか
bool BackUpDlg::RunBackup(bool write_batch_only)
{
	bool success = true;
	CString batch;

	for (const BackupSetting& entry : entries_)
	{
		if (entry.bk_enable == FALSE || entry.src_path.IsEmpty() || entry.dst_path.IsEmpty())
		{
			continue;
		}

		CString cmd_name = _T("xcopy");
		if (entry.opt & BackupSetting::kOptSubdir)
		{
			cmd_name += _T(" /E");		// ディレクトリごとコピー
		}
		if (entry.opt & BackupSetting::kOptDiff)
		{
			cmd_name += _T(" /D");		// 新しいファイルのみコピー
		}
		if (entry.opt & BackupSetting::kOptOverwrite)
		{
			cmd_name += _T(" /-Y");		// 上書きの確認を表示
		}
		else
		{
			cmd_name += _T(" /Y");		// 上書きの確認を表示しない
		}
		cmd_name += _T(" /I");			// 受け側ディレクトリを新規作成
		cmd_name += _T(" /H");			// 隠しファイルやシステムファイルも対象
		cmd_name += _T(" /R");			// 読み取り専用でも上書き

		CString command;
		command.Format(_T("%s \"%s\" \"%s\"\n"), cmd_name.GetString(), entry.src_path.GetString(), entry.dst_path.GetString());

		if (write_batch_only)
		{
			batch += command;
		}
		else if (system(command) != 0)
		{
			success = false;
		}
	}

	if (write_batch_only)
	{
		WriteBatchFile(batch);
	}

	return success;
}

void BackUpDlg::OnBrowseSrc()
{
	CString path;
	if (BrowseFolder(m_hWnd, kBrowseTitle, path))
	{
		entries_[cur_index_].src_path = path;
		UpdatePath(IDC_EDIT_SRC, kSubitemSrcPath, path);
	}
}

void BackUpDlg::OnBrowseDest()
{
	CString path;
	if (BrowseFolder(m_hWnd, kBrowseTitle, path))
	{
		entries_[cur_index_].dst_path = path;
		UpdatePath(IDC_EDIT_DST, kSubitemDstPath, path);
	}
}

void BackUpDlg::OnEnChangeEditSrc()
{
	CString str;
	GetDlgItemText(IDC_EDIT_SRC, str);
	entries_[cur_index_].src_path = str;
	list_ctrl_.SetItemText(cur_index_, kSubitemSrcPath, str);
}

void BackUpDlg::OnEnChangeEditDst()
{
	CString str;
	GetDlgItemText(IDC_EDIT_DST, str);
	entries_[cur_index_].dst_path = str;
	list_ctrl_.SetItemText(cur_index_, kSubitemDstPath, str);
}

void BackUpDlg::OnBnClickedEnableBK()
{
	BOOL is_on = (IsDlgButtonChecked(IDC_SELECT_BACKUP) == BST_CHECKED);
	entries_[cur_index_].bk_enable = is_on;
	UpdateEnableBK(is_on);
}

void BackUpDlg::OnBnClickedSubdir()
{
	SetOption(BackupSetting::kOptSubdir, IsDlgButtonChecked(IDC_SUBDIR) == BST_CHECKED);
	UpdateSubDirectory(entries_[cur_index_].opt);
}

// 差分コピー時は上書き確認を使わない
void BackUpDlg::OnDiff()
{
	const bool is_on = (IsDlgButtonChecked(IDC_DIFF) == BST_CHECKED);
	SetOption(BackupSetting::kOptDiff, is_on);
	if (is_on)
	{
		SetOption(BackupSetting::kOptOverwrite, false);
		UpdateOverWrite(entries_[cur_index_].opt);
	}

	// 上書き確認のチェックボックスの有効・無効もここで切り替わる
	UpdateDiffFile(entries_[cur_index_].opt);
}

void BackUpDlg::OnBnClickedOverWrite()
{
	SetOption(BackupSetting::kOptOverwrite, IsDlgButtonChecked(IDC_OVERWRITE) == BST_CHECKED);
	UpdateOverWrite(entries_[cur_index_].opt);
}

void BackUpDlg::SetOption(DWORD mask, bool is_on)
{
	DWORD& opt = entries_[cur_index_].opt;
	if (is_on)
		opt |= mask;
	else
		opt &= ~mask;
}

// チェックボックスとリストの表示をそろえる
void BackUpDlg::UpdateCheckItem(int ctrl_id, int sub_item, bool is_on, LPCTSTR text_on, LPCTSTR text_off)
{
	CheckDlgButton(ctrl_id, is_on ? BST_CHECKED : BST_UNCHECKED);
	list_ctrl_.SetItemText(cur_index_, sub_item, is_on ? text_on : text_off);
}

void BackUpDlg::UpdateEnableBK(BOOL checked)
{
	UpdateCheckItem(IDC_SELECT_BACKUP, kSubitemEnableBk, checked == TRUE, kStrEnable, kStrDisable);
}

void BackUpDlg::UpdateSubDirectory(DWORD opt)
{
	UpdateCheckItem(IDC_SUBDIR, kSubitemSubdirectory, (opt & BackupSetting::kOptSubdir) != 0, kStrOn, kStrOff);
}

void BackUpDlg::UpdateDiffFile(DWORD opt)
{
	const bool is_on = (opt & BackupSetting::kOptDiff) != 0;
	GetDlgItem(IDC_OVERWRITE)->EnableWindow(is_on ? FALSE : TRUE);
	UpdateCheckItem(IDC_DIFF, kSubitemDiffFile, is_on, kStrOn, kStrOff);
}

void BackUpDlg::UpdateOverWrite(DWORD opt)
{
	UpdateCheckItem(IDC_OVERWRITE, kSubitemOverwrite, (opt & BackupSetting::kOptOverwrite) != 0, kStrOn, kStrOff);
}

void BackUpDlg::UpdatePath(int edit_id, int sub_item, LPCTSTR path)
{
	SetDlgItemText(edit_id, path);
	list_ctrl_.SetItemText(cur_index_, sub_item, path);
}

void BackUpDlg::Refresh()
{
	// エディットへの設定で EN_CHANGE が走り entries_ に書き戻されるため、値を複製して使う
	const BackupSetting entry = entries_[cur_index_];

	UpdateEnableBK(entry.bk_enable);
	UpdateSubDirectory(entry.opt);
	UpdateDiffFile(entry.opt);
	UpdateOverWrite(entry.opt);
	UpdatePath(IDC_EDIT_SRC, kSubitemSrcPath, entry.src_path);
	UpdatePath(IDC_EDIT_DST, kSubitemDstPath, entry.dst_path);
}

void BackUpDlg::OnBnClickedAllClear()
{
	std::fill(std::begin(entries_), std::end(entries_), BackupSetting());
	Refresh();
}

void BackUpDlg::OnBnClickedSaveSetting()
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
BOOL BackUpDlg::WriteSetting()
{
	const CString path = GetDocumentsFilePath(kSetFileName);
	if (path.IsEmpty())
	{
		return FALSE;
	}

	CStdioFile file;
	if (!file.Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
		return FALSE;
	}

	BOOL succeeded = FALSE;
	for (const BackupSetting& entry : entries_)
	{
		if (entry.src_path.IsEmpty() || entry.dst_path.IsEmpty())
		{
			continue;
		}

		CString line;
		line.Format(_T("%d,%d,%s,%s\n"), entry.bk_enable, entry.opt, entry.src_path.GetString(), entry.dst_path.GetString());
		file.WriteString(line);
		succeeded = TRUE;
	}
	file.Close();

	return succeeded;
}

BOOL BackUpDlg::ReadSetting()
{
	BOOL succeeded = FALSE;
	int idx = 0;

	const CString path = GetDocumentsFilePath(kSetFileName);
	CStdioFile file;
	if (!path.IsEmpty() && file.Open(path, CFile::modeRead | CFile::typeText))
	{
		CString str;
		while (idx < kMaxEntry && file.ReadString(str))
		{
			int cur_pos = 0;
			BackupSetting& entry = entries_[idx];
			entry.bk_enable  = _ttoi(str.Tokenize(_T(","), cur_pos));
			entry.opt        = _ttoi(str.Tokenize(_T(","), cur_pos));
			entry.src_path = str.Tokenize(_T(","), cur_pos);
			entry.dst_path = str.Tokenize(_T(","), cur_pos);
			idx++;
		}

		succeeded = TRUE;
		file.Close();
	}

	for (; idx < kMaxEntry; idx++)
	{
		entries_[idx] = BackupSetting();
	}

	return succeeded;
}

void BackUpDlg::WriteBatchFile(const CString& cmd)
{
	const CString path = GetDocumentsFilePath(kBatFileName);
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
void BackUpDlg::OnEndOption(UINT id)
{
	end_action_ = static_cast<EndAction>(id - IDC_END_NONE);
}

// 閉じるとき（×・Esc）は、現在の設定を実行用バッチファイルにも書き出す
void BackUpDlg::OnCancel()
{
	RunBackup(true);

	CDialog::OnCancel();
}
