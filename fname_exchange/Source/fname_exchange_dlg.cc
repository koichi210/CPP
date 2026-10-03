// fname_exchange_dlg.cc : メインダイアログ（選んだファイル/フォルダの名前を一括で変換する）

#include "stdafx.h"
#include "fname_exchange.h"
#include "fname_exchange_dlg.h"
#include "common_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr bool		kDefaultIgnoreAlert	= true;
	constexpr int		kMaxDigits			= 5;	// 桁数コンボの選択肢（1〜5桁）
	constexpr UINT		kMinRemainLength	= 5;	// 指定文字数削除の後に残すべき最小のバイト数
	constexpr size_t	kMaxUndoSteps		= 100;	// 復元できる実行回数
	constexpr size_t	kMaxFilesPerStep	= 250;	// 1回の実行で復元情報に残せるファイル数
	constexpr int		kListCharWidth		= 6;	// 一覧の横スクロール幅を決める1文字の幅(px)
	constexpr LPCTSTR	kBrowseTitle		= _T("Select a destination folder");

	const DlgItemText kItemTexts[] =
	{
		{ IDST_DIR,            IDSTR_DIR },
		{ IDBT_BROWSE,         IDSTR_BROWSE },
		{ IDST_FILE_LIST,      IDSTR_FILE_LIST },
		{ IDBT_GET_FILE,       IDSTR_GET_FILE },
		{ IDBT_ALL_CHECK,      IDSTR_ALL_CHECK },
		{ IDBT_ALL_UNCHECK,    IDSTR_ALL_UNCHECK },
		{ IDGR_SYSTEM_SET,     IDSTR_SYSTEM_SET },
		{ IDCH_IGNORE_ALERT,   IDSTR_IGNORE_ALERT },
		{ IDCH_COMP_BIG_SMALL, IDSTR_COMP_BIG_SMALL },
		{ IDGR_HOWTO_CHANGE,   IDSTR_HOWTO_CHANGE },
		{ IDRB_ENUM,           IDSTR_ENUM },
		{ IDST_FIRST_NUM,      IDSTR_FIRST_NUM },
		{ IDCH_KEEP_NAME,      IDSTR_KEEP_NAME },
		{ IDRB_DEL_NUM,        IDSTR_DEL_NUM },
		{ IDST_DEL_BEF_NUM,    IDSTR_DEL_BEF_NUM },
		{ IDST_DEL_AFT_NUM,    IDSTR_DEL_AFT_NUM },
		{ IDRB_DEL_DIST,       IDSTR_DEL_DIST },
		{ IDRB_ADD,            IDSTR_ADD },
		{ IDCH_ADD_BEF,        IDSTR_ADD_BEF },
		{ IDCH_ADD_AFT,        IDSTR_ADD_AFT },
		{ IDRB_DEL,            IDSTR_DEL },
		{ IDRB_REP,            IDSTR_REP },
		{ IDST_NAME1,          IDSTR_NAME_ADD },
		{ IDST_NAME2,          IDSTR_NAME_REP_AFT },
		{ IDBT_EXE,            IDSTR_EXE },
		{ IDBT_UNDO,           IDSTR_UNDO },
		{ IDBT_END,            IDSTR_END },
	};

	// 変換方法のラジオボタン（ID は IDRB_ENUM〜IDRB_ALL_SBCS の連番）
	struct ConvertButton
	{
		UINT		id;
		ConvertType	type;
	};
	const ConvertButton kConvertButtons[] =
	{
		{ IDRB_ENUM,     ConvertType::kEnum },
		{ IDRB_ALL_SBCS, ConvertType::kAllSbcs },
		{ IDRB_ALL_DBCS, ConvertType::kAllDbcs },
		{ IDRB_DEL_DIST, ConvertType::kDeleteExt },
		{ IDRB_DEL_NUM,  ConvertType::kDeleteCount },
		{ IDRB_ADD,      ConvertType::kAdd },
		{ IDRB_DEL,      ConvertType::kDelete },
		{ IDRB_REP,      ConvertType::kReplace },
	};

	UINT CountDigits(UINT number)
	{
		UINT digits = 0;
		for (; number != 0; number /= 10)
		{
			digits++;
		}
		return digits;
	}
}

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class AboutDlg : public CDialog
{
public:
	AboutDlg() : CDialog(IDD) {}

	enum { IDD = IDD_ABOUTBOX };
};

/////////////////////////////////////////////////////////////////////////////
// FnameExchangeDlg

FnameExchangeDlg::FnameExchangeDlg(CWnd* parent /*=nullptr*/)
	: CDialog(IDD, parent)
	, icon_(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
	, target_(Target::kFile)
	, type_(ConvertType::kEnum)
	, ignore_alert_(kDefaultIgnoreAlert)
	, case_sensitive_(false)
	, first_number_(0)
	, next_number_(0)
	, digits_(0)
	, keep_name_(false)
	, delete_head_(0)
	, delete_tail_(0)
	, add_before_(false)
	, add_after_(false)
{
}

void FnameExchangeDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
	DDX_Control(dx, IDLB_FILE, list_);
}

BEGIN_MESSAGE_MAP(FnameExchangeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_BROWSE, &FnameExchangeDlg::OnBrowse)
	ON_BN_CLICKED(IDBT_GET_FILE, &FnameExchangeDlg::OnGetFile)
	ON_BN_CLICKED(IDRB_FILE, &FnameExchangeDlg::OnTargetFile)
	ON_BN_CLICKED(IDRB_FOLDER, &FnameExchangeDlg::OnTargetFolder)
	ON_BN_CLICKED(IDBT_ALL_CHECK, &FnameExchangeDlg::OnAllCheck)
	ON_BN_CLICKED(IDBT_ALL_UNCHECK, &FnameExchangeDlg::OnAllUncheck)
	ON_BN_CLICKED(IDCH_IGNORE_ALERT, &FnameExchangeDlg::OnIgnoreAlert)
	ON_BN_CLICKED(IDCH_COMP_BIG_SMALL, &FnameExchangeDlg::OnCaseSensitive)
	ON_CONTROL_RANGE(BN_CLICKED, IDRB_ENUM, IDRB_ALL_SBCS, &FnameExchangeDlg::OnConvertType)
	ON_BN_CLICKED(IDBT_EXE, &FnameExchangeDlg::OnExecute)
	ON_BN_CLICKED(IDBT_UNDO, &FnameExchangeDlg::OnUndo)
	ON_BN_CLICKED(IDBT_END, &FnameExchangeDlg::OnEnd)
END_MESSAGE_MAP()

BOOL FnameExchangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// システムメニューに「バージョン情報」を追加（ID はシステムコマンドの範囲内である必要がある）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
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

	CString title;
	title.LoadString(IDSTR_FILE_EXCHANGE);
	SetWindowText(title);
	SetDlgItemTextAll(m_hWnd, kItemTexts, static_cast<int>(_countof(kItemTexts)));

	CheckRadioButton(IDRB_FILE, IDRB_FOLDER, IDRB_FILE);
	SetDlgItemText(IDET_FIRST_NUM, _T("1"));
	CheckRadioButton(IDRB_ENUM, IDRB_REP, IDRB_ENUM);
	CheckDlgButton(IDCH_KEEP_NAME, BST_CHECKED);
	CheckDlgButton(IDCH_ADD_BEF, BST_CHECKED);
	CheckDlgButton(IDCH_IGNORE_ALERT, kDefaultIgnoreAlert ? BST_CHECKED : BST_UNCHECKED);
	type_ = ConvertType::kEnum;

	InitDigitsCombo();
	undo_steps_.clear();
	UpdateControls();

	return TRUE;
}

// 桁数の選択肢：「自動」「1 ケタ」〜「5 ケタ」。選択位置がそのまま桁数になる
void FnameExchangeDlg::InitDigitsCombo()
{
	SendDlgItemMessage(IDCB_KETA, CB_RESETCONTENT, 0, 0);

	CString text;
	text.LoadString(IDSTR_AUTO);
	SendDlgItemMessage(IDCB_KETA, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.GetString()));
	for (int i = 1; i <= kMaxDigits; i++)
	{
		text.Format(IDSTR_KETA, i);
		SendDlgItemMessage(IDCB_KETA, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.GetString()));
	}
	SendDlgItemMessage(IDCB_KETA, CB_SETCURSEL, 0, 0);
}

void FnameExchangeDlg::OnSysCommand(UINT id, LPARAM param)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg about_dlg;
		about_dlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(id, param);
	}
}

// 最小化時のアイコン描画（ダイアログアプリでは自前で描く必要がある）
void FnameExchangeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - icon_width + 1) / 2;
		const int y = (rect.Height() - icon_height + 1) / 2;
		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR FnameExchangeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void FnameExchangeDlg::OnBrowse()
{
	CString path;
	if (BrowseFolder(m_hWnd, kBrowseTitle, path))
	{
		SetDlgItemText(IDET_DIR, path);
		OnGetFile();
	}
}

// 対象ディレクトリ直下のファイル（またはフォルダ）を一覧に出す
void FnameExchangeDlg::OnGetFile()
{
	CString dir;
	GetDlgItemText(IDET_DIR, dir);
	if (dir.IsEmpty())
	{
		return;
	}
	const CString pattern = dir + _T("\\*.*");

	list_.ResetContent();

	int max_length = 0;
	if (target_ == Target::kFile)
	{
		list_.Dir(DDL_READWRITE, pattern);
	}
	else
	{
		CFileFind finder;
		BOOL found = finder.FindFile(pattern);
		while (found)
		{
			found = finder.FindNextFile();
			if (!finder.IsDirectory() || finder.IsDots())
			{
				continue;
			}

			const CString name = finder.GetFileName();
			list_.AddString(name);
			if (max_length < name.GetLength())
			{
				max_length = name.GetLength();
			}
		}
	}
	list_.SetHorizontalExtent(max_length * kListCharWidth);
}

void FnameExchangeDlg::OnTargetFile()
{
	target_ = Target::kFile;
	OnGetFile();
}

void FnameExchangeDlg::OnTargetFolder()
{
	target_ = Target::kFolder;
	OnGetFile();
}

void FnameExchangeDlg::OnAllCheck()
{
	list_.SetSel(-1, TRUE);
}

void FnameExchangeDlg::OnAllUncheck()
{
	list_.SetSel(-1, FALSE);
}

void FnameExchangeDlg::OnIgnoreAlert()
{
	ignore_alert_ = (IsDlgButtonChecked(IDCH_IGNORE_ALERT) == BST_CHECKED);
}

void FnameExchangeDlg::OnCaseSensitive()
{
	case_sensitive_ = (IsDlgButtonChecked(IDCH_COMP_BIG_SMALL) == BST_CHECKED);
}

void FnameExchangeDlg::OnConvertType(UINT id)
{
	for (const ConvertButton& button : kConvertButtons)
	{
		if (button.id == id)
		{
			type_ = button.type;
			UpdateControls();
			return;
		}
	}
}

void FnameExchangeDlg::OnEnd()
{
	CDialog::OnOK();
}

void FnameExchangeDlg::OnExecute()
{
	ReadSettings();
	const std::vector<CString> names = GetSelectedNames();
	if (names.empty())
	{
		ShowError(IDSTR_NO_SELECT);
		return;
	}

	// 復元情報が上限に達していたら、いちばん古いものを捨てる
	if (undo_steps_.size() >= kMaxUndoSteps)
	{
		undo_steps_.erase(undo_steps_.begin());
	}
	undo_steps_.emplace_back();

	const int file_count = static_cast<int>(names.size());
	for (const CString& name : names)
	{
		CString old_path = dir_;
		AppendPath(old_path, name);

		const CString new_path = MakeNewPath(old_path, file_count);
		if (!new_path.IsEmpty())
		{
			RenameFile(old_path, new_path);
		}
	}

	OnGetFile();
	EnableItem(IDBT_UNDO, true);
}

void FnameExchangeDlg::OnUndo()
{
	Undo();
	OnGetFile();
	UpdateControls();
}

void FnameExchangeDlg::UpdateControls()
{
	const bool is_enum = (type_ == ConvertType::kEnum);
	EnableItem(IDST_FIRST_NUM, is_enum);
	EnableItem(IDET_FIRST_NUM, is_enum);
	EnableItem(IDCH_KEEP_NAME, is_enum);
	EnableItem(IDCB_KETA, is_enum);

	const bool is_delete_count = (type_ == ConvertType::kDeleteCount);
	EnableItem(IDST_DEL_BEF_NUM, is_delete_count);
	EnableItem(IDET_DEL_BEF_NUM, is_delete_count);
	EnableItem(IDST_DEL_AFT_NUM, is_delete_count);
	EnableItem(IDET_DEL_AFT_NUM, is_delete_count);

	const bool is_add = (type_ == ConvertType::kAdd);
	EnableItem(IDCH_ADD_BEF, is_add);
	EnableItem(IDCH_ADD_AFT, is_add);

	const bool use_text1 = (type_ == ConvertType::kDelete || type_ == ConvertType::kReplace);
	EnableItem(IDST_NAME1, use_text1);
	EnableItem(IDET_NAME1, use_text1);

	const bool use_text2 = (type_ == ConvertType::kAdd || type_ == ConvertType::kReplace);
	EnableItem(IDST_NAME2, use_text2);
	EnableItem(IDET_NAME2, use_text2);

	// 見出しは使う変換方法のときだけ差し替える（使わない欄は前の見出しのまま無効表示）
	CString label;
	switch (type_)
	{
	case ConvertType::kAdd:
		label.LoadString(IDSTR_NAME_ADD);
		SetDlgItemText(IDST_NAME2, label);
		break;
	case ConvertType::kDelete:
		label.LoadString(IDSTR_NAME_DEL);
		SetDlgItemText(IDST_NAME1, label);
		break;
	case ConvertType::kReplace:
		label.LoadString(IDSTR_REP_BEF);
		SetDlgItemText(IDST_NAME1, label);
		label.LoadString(IDSTR_NAME_REP_AFT);
		SetDlgItemText(IDST_NAME2, label);
		break;
	default:
		break;
	}

	EnableItem(IDBT_UNDO, !undo_steps_.empty());
}

void FnameExchangeDlg::EnableItem(int id, bool enable)
{
	GetDlgItem(id)->EnableWindow(enable ? TRUE : FALSE);
}

// 選んでいる変換方法の設定だけ画面から読む
void FnameExchangeDlg::ReadSettings()
{
	first_number_ = 0;
	next_number_ = 0;
	digits_ = 0;
	keep_name_ = false;
	delete_head_ = 0;
	delete_tail_ = 0;
	add_before_ = false;
	add_after_ = false;
	text1_.Empty();
	text2_.Empty();

	GetDlgItemText(IDET_DIR, dir_);

	switch (type_)
	{
	case ConvertType::kEnum:
		first_number_ = GetDlgItemInt(IDET_FIRST_NUM, nullptr, FALSE);
		next_number_ = first_number_;
		keep_name_ = (IsDlgButtonChecked(IDCH_KEEP_NAME) == BST_CHECKED);
		digits_ = static_cast<int>(SendDlgItemMessage(IDCB_KETA, CB_GETCURSEL, 0, 0));
		break;

	case ConvertType::kDeleteCount:
		delete_head_ = GetDlgItemInt(IDET_DEL_BEF_NUM, nullptr, FALSE);
		delete_tail_ = GetDlgItemInt(IDET_DEL_AFT_NUM, nullptr, FALSE);
		break;

	case ConvertType::kAdd:
		GetDlgItemText(IDET_NAME2, text2_);
		add_before_ = (IsDlgButtonChecked(IDCH_ADD_BEF) == BST_CHECKED);
		add_after_ = (IsDlgButtonChecked(IDCH_ADD_AFT) == BST_CHECKED);
		break;

	case ConvertType::kDelete:
		GetDlgItemText(IDET_NAME1, text1_);
		break;

	case ConvertType::kReplace:
		GetDlgItemText(IDET_NAME1, text1_);
		GetDlgItemText(IDET_NAME2, text2_);
		break;

	default:
		break;
	}
}

std::vector<CString> FnameExchangeDlg::GetSelectedNames()
{
	std::vector<CString> names;
	const int count = list_.GetSelCount();
	if (count <= 0)
	{
		return names;
	}

	std::vector<int> indexes(count);
	list_.GetSelItems(count, indexes.data());
	for (int index : indexes)
	{
		CString name;
		list_.GetText(index, name);
		names.push_back(name);
	}
	return names;
}

CString FnameExchangeDlg::MakeNewPath(const CString& old_path, int file_count)
{
	CString file;
	CString ext;
	SplitPath(old_path, nullptr, nullptr, &file, &ext);

	switch (type_)
	{
	case ConvertType::kEnum:
		return BuildPath(MakeEnumName(file, file_count), ext);

	case ConvertType::kAllSbcs:
		return BuildPath(ZenkakuToHankaku(file), ext);

	case ConvertType::kAllDbcs:
		return BuildPath(HankakuToZenkaku(file), ext);

	case ConvertType::kDeleteExt:
		return BuildPath(file, CString());

	case ConvertType::kDeleteCount:
	{
		const CString name = MakeDeleteCountName(file);
		return name.IsEmpty() ? CString() : BuildPath(name, ext);
	}

	case ConvertType::kAdd:
	{
		const CString name = MakeAddName(file);
		return name.IsEmpty() ? CString() : BuildPath(name, ext);
	}

	case ConvertType::kDelete:
		if (text1_.IsEmpty())
		{
			ShowError(IDSTR_ERR_INPUT_DEL_STR);
			return CString();
		}
		return BuildPath(ReplaceString(file, text1_, nullptr, case_sensitive_), ext);

	case ConvertType::kReplace:
		if (text1_.IsEmpty() || text2_.IsEmpty())
		{
			ShowError(IDSTR_ERR_INPUT_REP_STR);
			return CString();
		}
		return BuildPath(ReplaceString(file, text1_, text2_, case_sensitive_), ext);

	default:
		return CString();
	}
}

// 番号は「最後に使う番号（最初の値 + 件数 - 1）」の桁数（または指定桁数の大きい方）まで 0 で埋める
CString FnameExchangeDlg::MakeEnumName(const CString& file, int file_count)
{
	const UINT last_number = first_number_ + static_cast<UINT>(file_count) - 1;
	int digits = static_cast<int>(CountDigits(last_number));
	if (digits < digits_)
	{
		digits = digits_;
	}
	const int current_digits = (next_number_ == 0) ? 1 : static_cast<int>(CountDigits(next_number_));

	CString name;
	for (int i = current_digits; i < digits; i++)
	{
		name += _T('0');
	}
	CString number;
	number.Format(_T("%d"), next_number_);
	name += number;
	next_number_++;

	if (keep_name_)
	{
		name += _T(' ');
		name += file;
	}
	return name;
}

CString FnameExchangeDlg::MakeDeleteCountName(const CString& file)
{
	if (delete_head_ == 0 && delete_tail_ == 0)
	{
		ShowError(IDSTR_ERR_DEL_NUM);
		return CString();
	}

	// 文字数はバイト単位で数える
	const UINT length = static_cast<UINT>(file.GetLength());
	if (length < delete_head_ + delete_tail_ + kMinRemainLength)
	{
		if (!ignore_alert_)
		{
			ShowError(IDSTR_ERR_SHORT_NAME);
		}
		return CString();
	}

	CString name = file;
	name.Delete(static_cast<int>(length - delete_tail_), static_cast<int>(delete_tail_));
	name.Delete(0, static_cast<int>(delete_head_));
	return name;
}

CString FnameExchangeDlg::MakeAddName(const CString& file)
{
	if (!add_before_ && !add_after_)
	{
		ShowError(IDSTR_ERR_SEL_INSERT);
		return CString();
	}
	if (text2_.IsEmpty())
	{
		ShowError(IDSTR_ERR_INPUT_ADD_STR);
		return CString();
	}

	CString name;
	if (add_before_)
	{
		name += text2_;
	}
	name += file;
	if (add_after_)
	{
		name += text2_;
	}
	return name;
}

// 対象ディレクトリ + 名前 + 拡張子（ext は SplitPath で得た "." 付きのもの）
// 名前が空でも対象ディレクトリの外のパスにならないよう、名前と拡張子をつないでから連結する
CString FnameExchangeDlg::BuildPath(const CString& name, const CString& ext) const
{
	CString path = dir_;
	AppendPath(path, name + ext);
	return path;
}

void FnameExchangeDlg::RenameFile(const CString& old_path, const CString& new_path)
{
	CString title;
	CString message;

	if (old_path.CompareNoCase(new_path) == 0)
	{
		if (!ignore_alert_)
		{
			message.Format(IDSTR_ERR_FAIL_OVERLAP, old_path.GetString(), new_path.GetString());
			title.LoadString(IDSTR_ERROR);
			MessageBox(message, title, MB_OK);
		}
		return;
	}

	if (!::MoveFile(old_path, new_path))
	{
		const DWORD error = ::GetLastError();
		if (!ignore_alert_)
		{
			message.Format(IDSTR_ERR_FAIL_CHANGE_NAME, static_cast<int>(error), old_path.GetString(), new_path.GetString());
			title.LoadString(IDSTR_ERROR);
			MessageBox(message, title, MB_OK);
		}
		return;
	}

	std::vector<RenameRecord>& step = undo_steps_.back();
	if (step.size() < kMaxFilesPerStep)
	{
		step.push_back({ old_path, new_path });
		return;
	}

	message.LoadString(IDSTR_WRN_CACHE_FULL);
	title.LoadString(IDSTR_WRN);
	if (MessageBox(message, title, MB_YESNO) == IDYES)
	{
		// 以降の変更は新しい1回分として記録する
		undo_steps_.clear();
		undo_steps_.emplace_back();
	}
}

void FnameExchangeDlg::Undo()
{
	if (undo_steps_.empty())
	{
		ShowError(IDSTR_ERR_NOT_UNDO);
		return;
	}

	const std::vector<RenameRecord> step = std::move(undo_steps_.back());
	undo_steps_.pop_back();

	// 同じ名前を順に使い回した変換（例: 1→0, 2→1）も戻せるよう、後ろから戻す
	for (auto it = step.rbegin(); it != step.rend(); ++it)
	{
		if (!::MoveFile(it->new_path, it->old_path) && !ignore_alert_)
		{
			CString message;
			CString title;
			message.Format(IDSTR_ERR_FAIL_UNDO, it->old_path.GetString(), it->new_path.GetString());
			title.LoadString(IDSTR_ERROR);
			MessageBox(message, title, MB_OK);
		}
	}
}

void FnameExchangeDlg::ShowError(UINT message_id)
{
	CString message;
	CString title;
	message.LoadString(message_id);
	title.LoadString(IDSTR_ERROR);
	MessageBox(message, title, MB_OK);
}
