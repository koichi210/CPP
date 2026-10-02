// fname_exchangeDlg.cpp : メインダイアログ（選んだファイル/フォルダの名前を一括で変換する）

#include "stdafx.h"
#include "fname_exchange.h"
#include "fname_exchangeDlg.h"
#include "CommonUtil.h"

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

	const DLGITEMTEXT kItemTexts[] =
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
		{ IDRB_ENUM,     ConvertType::Enum },
		{ IDRB_ALL_SBCS, ConvertType::AllSbcs },
		{ IDRB_ALL_DBCS, ConvertType::AllDbcs },
		{ IDRB_DEL_DIST, ConvertType::DeleteExt },
		{ IDRB_DEL_NUM,  ConvertType::DeleteCount },
		{ IDRB_ADD,      ConvertType::Add },
		{ IDRB_DEL,      ConvertType::Delete },
		{ IDRB_REP,      ConvertType::Replace },
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

class CAboutDlg : public CDialog
{
public:
	CAboutDlg() : CDialog(IDD) {}

	enum { IDD = IDD_ABOUTBOX };
};

/////////////////////////////////////////////////////////////////////////////
// CFnameExchangeDlg

CFnameExchangeDlg::CFnameExchangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD, pParent)
	, m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
	, m_target(Target::File)
	, m_type(ConvertType::Enum)
	, m_ignoreAlert(kDefaultIgnoreAlert)
	, m_caseSensitive(false)
	, m_firstNumber(0)
	, m_nextNumber(0)
	, m_digits(0)
	, m_keepName(false)
	, m_deleteHead(0)
	, m_deleteTail(0)
	, m_addBefore(false)
	, m_addAfter(false)
{
}

void CFnameExchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDLB_FILE, m_list);
}

BEGIN_MESSAGE_MAP(CFnameExchangeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_BROWSE, &CFnameExchangeDlg::OnBrowse)
	ON_BN_CLICKED(IDBT_GET_FILE, &CFnameExchangeDlg::OnGetFile)
	ON_BN_CLICKED(IDRB_FILE, &CFnameExchangeDlg::OnTargetFile)
	ON_BN_CLICKED(IDRB_FOLDER, &CFnameExchangeDlg::OnTargetFolder)
	ON_BN_CLICKED(IDBT_ALL_CHECK, &CFnameExchangeDlg::OnAllCheck)
	ON_BN_CLICKED(IDBT_ALL_UNCHECK, &CFnameExchangeDlg::OnAllUncheck)
	ON_BN_CLICKED(IDCH_IGNORE_ALERT, &CFnameExchangeDlg::OnIgnoreAlert)
	ON_BN_CLICKED(IDCH_COMP_BIG_SMALL, &CFnameExchangeDlg::OnCaseSensitive)
	ON_CONTROL_RANGE(BN_CLICKED, IDRB_ENUM, IDRB_ALL_SBCS, &CFnameExchangeDlg::OnConvertType)
	ON_BN_CLICKED(IDBT_EXE, &CFnameExchangeDlg::OnExecute)
	ON_BN_CLICKED(IDBT_UNDO, &CFnameExchangeDlg::OnUndo)
	ON_BN_CLICKED(IDBT_END, &CFnameExchangeDlg::OnEnd)
END_MESSAGE_MAP()

BOOL CFnameExchangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// システムメニューに「バージョン情報」を追加（ID はシステムコマンドの範囲内である必要がある）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString aboutMenu;
		aboutMenu.LoadString(IDS_ABOUTBOX);
		if (!aboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, aboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

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
	m_type = ConvertType::Enum;

	InitDigitsCombo();
	m_undoSteps.clear();
	UpdateControls();

	return TRUE;
}

// 桁数の選択肢：「自動」「1 ケタ」〜「5 ケタ」。選択位置がそのまま桁数になる
void CFnameExchangeDlg::InitDigitsCombo()
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

void CFnameExchangeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 最小化時のアイコン描画（ダイアログアプリでは自前で描く必要がある）
void CFnameExchangeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CFnameExchangeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFnameExchangeDlg::OnBrowse()
{
	CString path;
	if (BrowseFolder(m_hWnd, kBrowseTitle, path))
	{
		SetDlgItemText(IDET_DIR, path);
		OnGetFile();
	}
}

// 対象ディレクトリ直下のファイル（またはフォルダ）を一覧に出す
void CFnameExchangeDlg::OnGetFile()
{
	CString dir;
	GetDlgItemText(IDET_DIR, dir);
	if (dir.IsEmpty())
	{
		return;
	}
	const CString pattern = dir + _T("\\*.*");

	m_list.ResetContent();

	int maxLength = 0;
	if (m_target == Target::File)
	{
		m_list.Dir(DDL_READWRITE, pattern);
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
			m_list.AddString(name);
			if (maxLength < name.GetLength())
			{
				maxLength = name.GetLength();
			}
		}
	}
	m_list.SetHorizontalExtent(maxLength * kListCharWidth);
}

void CFnameExchangeDlg::OnTargetFile()
{
	m_target = Target::File;
	OnGetFile();
}

void CFnameExchangeDlg::OnTargetFolder()
{
	m_target = Target::Folder;
	OnGetFile();
}

void CFnameExchangeDlg::OnAllCheck()
{
	m_list.SetSel(-1, TRUE);
}

void CFnameExchangeDlg::OnAllUncheck()
{
	m_list.SetSel(-1, FALSE);
}

void CFnameExchangeDlg::OnIgnoreAlert()
{
	m_ignoreAlert = (IsDlgButtonChecked(IDCH_IGNORE_ALERT) == BST_CHECKED);
}

void CFnameExchangeDlg::OnCaseSensitive()
{
	m_caseSensitive = (IsDlgButtonChecked(IDCH_COMP_BIG_SMALL) == BST_CHECKED);
}

void CFnameExchangeDlg::OnConvertType(UINT nID)
{
	for (const ConvertButton& button : kConvertButtons)
	{
		if (button.id == nID)
		{
			m_type = button.type;
			UpdateControls();
			return;
		}
	}
}

void CFnameExchangeDlg::OnEnd()
{
	CDialog::OnOK();
}

void CFnameExchangeDlg::OnExecute()
{
	ReadSettings();
	const std::vector<CString> names = GetSelectedNames();
	if (names.empty())
	{
		ShowError(IDSTR_NO_SELECT);
		return;
	}

	// 復元情報が上限に達していたら、いちばん古いものを捨てる
	if (m_undoSteps.size() >= kMaxUndoSteps)
	{
		m_undoSteps.erase(m_undoSteps.begin());
	}
	m_undoSteps.emplace_back();

	const int fileCount = static_cast<int>(names.size());
	for (const CString& name : names)
	{
		CString oldPath = m_dir;
		AppendPath(oldPath, name);

		const CString newPath = MakeNewPath(oldPath, fileCount);
		if (!newPath.IsEmpty())
		{
			RenameFile(oldPath, newPath);
		}
	}

	OnGetFile();
	EnableItem(IDBT_UNDO, true);
}

void CFnameExchangeDlg::OnUndo()
{
	Undo();
	OnGetFile();
	UpdateControls();
}

void CFnameExchangeDlg::UpdateControls()
{
	const bool isEnum = (m_type == ConvertType::Enum);
	EnableItem(IDST_FIRST_NUM, isEnum);
	EnableItem(IDET_FIRST_NUM, isEnum);
	EnableItem(IDCH_KEEP_NAME, isEnum);
	EnableItem(IDCB_KETA, isEnum);

	const bool isDeleteCount = (m_type == ConvertType::DeleteCount);
	EnableItem(IDST_DEL_BEF_NUM, isDeleteCount);
	EnableItem(IDET_DEL_BEF_NUM, isDeleteCount);
	EnableItem(IDST_DEL_AFT_NUM, isDeleteCount);
	EnableItem(IDET_DEL_AFT_NUM, isDeleteCount);

	const bool isAdd = (m_type == ConvertType::Add);
	EnableItem(IDCH_ADD_BEF, isAdd);
	EnableItem(IDCH_ADD_AFT, isAdd);

	const bool useText1 = (m_type == ConvertType::Delete || m_type == ConvertType::Replace);
	EnableItem(IDST_NAME1, useText1);
	EnableItem(IDET_NAME1, useText1);

	const bool useText2 = (m_type == ConvertType::Add || m_type == ConvertType::Replace);
	EnableItem(IDST_NAME2, useText2);
	EnableItem(IDET_NAME2, useText2);

	// 見出しは使う変換方法のときだけ差し替える（使わない欄は前の見出しのまま無効表示）
	CString label;
	switch (m_type)
	{
	case ConvertType::Add:
		label.LoadString(IDSTR_NAME_ADD);
		SetDlgItemText(IDST_NAME2, label);
		break;
	case ConvertType::Delete:
		label.LoadString(IDSTR_NAME_DEL);
		SetDlgItemText(IDST_NAME1, label);
		break;
	case ConvertType::Replace:
		label.LoadString(IDSTR_REP_BEF);
		SetDlgItemText(IDST_NAME1, label);
		label.LoadString(IDSTR_NAME_REP_AFT);
		SetDlgItemText(IDST_NAME2, label);
		break;
	default:
		break;
	}

	EnableItem(IDBT_UNDO, !m_undoSteps.empty());
}

void CFnameExchangeDlg::EnableItem(int id, bool enable)
{
	GetDlgItem(id)->EnableWindow(enable ? TRUE : FALSE);
}

// 選んでいる変換方法の設定だけ画面から読む
void CFnameExchangeDlg::ReadSettings()
{
	m_firstNumber = 0;
	m_nextNumber = 0;
	m_digits = 0;
	m_keepName = false;
	m_deleteHead = 0;
	m_deleteTail = 0;
	m_addBefore = false;
	m_addAfter = false;
	m_text1.Empty();
	m_text2.Empty();

	GetDlgItemText(IDET_DIR, m_dir);

	switch (m_type)
	{
	case ConvertType::Enum:
		m_firstNumber = GetDlgItemInt(IDET_FIRST_NUM, nullptr, FALSE);
		m_nextNumber = m_firstNumber;
		m_keepName = (IsDlgButtonChecked(IDCH_KEEP_NAME) == BST_CHECKED);
		m_digits = static_cast<int>(SendDlgItemMessage(IDCB_KETA, CB_GETCURSEL, 0, 0));
		break;

	case ConvertType::DeleteCount:
		m_deleteHead = GetDlgItemInt(IDET_DEL_BEF_NUM, nullptr, FALSE);
		m_deleteTail = GetDlgItemInt(IDET_DEL_AFT_NUM, nullptr, FALSE);
		break;

	case ConvertType::Add:
		GetDlgItemText(IDET_NAME2, m_text2);
		m_addBefore = (IsDlgButtonChecked(IDCH_ADD_BEF) == BST_CHECKED);
		m_addAfter = (IsDlgButtonChecked(IDCH_ADD_AFT) == BST_CHECKED);
		break;

	case ConvertType::Delete:
		GetDlgItemText(IDET_NAME1, m_text1);
		break;

	case ConvertType::Replace:
		GetDlgItemText(IDET_NAME1, m_text1);
		GetDlgItemText(IDET_NAME2, m_text2);
		break;

	default:
		break;
	}
}

std::vector<CString> CFnameExchangeDlg::GetSelectedNames()
{
	std::vector<CString> names;
	const int count = m_list.GetSelCount();
	if (count <= 0)
	{
		return names;
	}

	std::vector<int> indexes(count);
	m_list.GetSelItems(count, indexes.data());
	for (int index : indexes)
	{
		CString name;
		m_list.GetText(index, name);
		names.push_back(name);
	}
	return names;
}

CString CFnameExchangeDlg::MakeNewPath(const CString& oldPath, int fileCount)
{
	CString file;
	CString ext;
	SplitPath(oldPath, nullptr, nullptr, &file, &ext);

	switch (m_type)
	{
	case ConvertType::Enum:
		return BuildPath(MakeEnumName(file, fileCount), ext);

	case ConvertType::AllSbcs:
		return BuildPath(ZenkakuToHankaku(file), ext);

	case ConvertType::AllDbcs:
		return BuildPath(HankakuToZenkaku(file), ext);

	case ConvertType::DeleteExt:
		return BuildPath(file, CString());

	case ConvertType::DeleteCount:
	{
		const CString name = MakeDeleteCountName(file);
		return name.IsEmpty() ? CString() : BuildPath(name, ext);
	}

	case ConvertType::Add:
	{
		const CString name = MakeAddName(file);
		return name.IsEmpty() ? CString() : BuildPath(name, ext);
	}

	case ConvertType::Delete:
		if (m_text1.IsEmpty())
		{
			ShowError(IDSTR_ERR_INPUT_DEL_STR);
			return CString();
		}
		return BuildPath(ReplaceString(file, m_text1, nullptr, m_caseSensitive), ext);

	case ConvertType::Replace:
		if (m_text1.IsEmpty() || m_text2.IsEmpty())
		{
			ShowError(IDSTR_ERR_INPUT_REP_STR);
			return CString();
		}
		return BuildPath(ReplaceString(file, m_text1, m_text2, m_caseSensitive), ext);

	default:
		return CString();
	}
}

// 番号は「最初の値 + 件数」の桁数（または指定桁数の大きい方）まで 0 で埋める
CString CFnameExchangeDlg::MakeEnumName(const CString& file, int fileCount)
{
	int digits = static_cast<int>(CountDigits(m_firstNumber + static_cast<UINT>(fileCount)));
	if (digits < m_digits)
	{
		digits = m_digits;
	}
	const int currentDigits = (m_nextNumber == 0) ? 1 : static_cast<int>(CountDigits(m_nextNumber));

	CString name;
	for (int i = currentDigits; i < digits; i++)
	{
		name += _T('0');
	}
	CString number;
	number.Format(_T("%d"), m_nextNumber);
	name += number;
	m_nextNumber++;

	if (m_keepName)
	{
		name += _T(' ');
		name += file;
	}
	return name;
}

CString CFnameExchangeDlg::MakeDeleteCountName(const CString& file)
{
	if (m_deleteHead == 0 && m_deleteTail == 0)
	{
		ShowError(IDSTR_ERR_DEL_NUM);
		return CString();
	}

	// 文字数はバイト単位で数える
	const UINT length = static_cast<UINT>(file.GetLength());
	if (length < m_deleteHead + m_deleteTail + kMinRemainLength)
	{
		if (!m_ignoreAlert)
		{
			ShowError(IDSTR_ERR_SHORT_NAME);
		}
		return CString();
	}

	CString name = file;
	name.Delete(static_cast<int>(length - m_deleteTail), static_cast<int>(m_deleteTail));
	name.Delete(0, static_cast<int>(m_deleteHead));
	return name;
}

CString CFnameExchangeDlg::MakeAddName(const CString& file)
{
	if (!m_addBefore && !m_addAfter)
	{
		ShowError(IDSTR_ERR_SEL_INSERT);
		return CString();
	}
	if (m_text2.IsEmpty())
	{
		ShowError(IDSTR_ERR_INPUT_ADD_STR);
		return CString();
	}

	CString name;
	if (m_addBefore)
	{
		name += m_text2;
	}
	name += file;
	if (m_addAfter)
	{
		name += m_text2;
	}
	return name;
}

// 対象ディレクトリ + 名前 + 拡張子（ext は SplitPath で得た "." 付きのもの）
// 名前が空でも対象ディレクトリの外のパスにならないよう、名前と拡張子をつないでから連結する
CString CFnameExchangeDlg::BuildPath(const CString& name, const CString& ext) const
{
	CString path = m_dir;
	AppendPath(path, name + ext);
	return path;
}

void CFnameExchangeDlg::RenameFile(const CString& oldPath, const CString& newPath)
{
	CString title;
	CString message;

	if (oldPath.CompareNoCase(newPath) == 0)
	{
		if (!m_ignoreAlert)
		{
			message.Format(IDSTR_ERR_FAIL_OVERLAP, oldPath.GetString(), newPath.GetString());
			title.LoadString(IDSTR_ERROR);
			MessageBox(message, title, MB_OK);
		}
		return;
	}

	if (!::MoveFile(oldPath, newPath))
	{
		const DWORD error = ::GetLastError();
		if (!m_ignoreAlert)
		{
			message.Format(IDSTR_ERR_FAIL_CHANGE_NAME, static_cast<int>(error), oldPath.GetString(), newPath.GetString());
			title.LoadString(IDSTR_ERROR);
			MessageBox(message, title, MB_OK);
		}
		return;
	}

	std::vector<RenameRecord>& step = m_undoSteps.back();
	if (step.size() < kMaxFilesPerStep)
	{
		step.push_back({ oldPath, newPath });
		return;
	}

	message.LoadString(IDSTR_WRN_CACHE_FULL);
	title.LoadString(IDSTR_WRN);
	if (MessageBox(message, title, MB_YESNO) == IDYES)
	{
		// 以降の変更は新しい1回分として記録する
		m_undoSteps.clear();
		m_undoSteps.emplace_back();
	}
}

void CFnameExchangeDlg::Undo()
{
	if (m_undoSteps.empty())
	{
		ShowError(IDSTR_ERR_NOT_UNDO);
		return;
	}

	const std::vector<RenameRecord> step = std::move(m_undoSteps.back());
	m_undoSteps.pop_back();

	// 同じ名前を順に使い回した変換（例: 1→0, 2→1）も戻せるよう、後ろから戻す
	for (auto it = step.rbegin(); it != step.rend(); ++it)
	{
		if (!::MoveFile(it->newPath, it->oldPath) && !m_ignoreAlert)
		{
			CString message;
			CString title;
			message.Format(IDSTR_ERR_FAIL_UNDO, it->oldPath.GetString(), it->newPath.GetString());
			title.LoadString(IDSTR_ERROR);
			MessageBox(message, title, MB_OK);
		}
	}
}

void CFnameExchangeDlg::ShowError(UINT messageId)
{
	CString message;
	CString title;
	message.LoadString(messageId);
	title.LoadString(IDSTR_ERROR);
	MessageBox(message, title, MB_OK);
}
