// macro_tool_dlg.cc : 設定ダイアログ（イベントの一覧編集・ファイル読み書き・記録）

#include "stdafx.h"
#include "macro_tool.h"
#include "macro_tool_dlg.h"
#include "util.h"
#include "input_simulator.h"
#include "common_util.h"

#include <cstdio>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr bool kUseKeyHook = false;	// キーボードの記録は未完成

#ifdef _DEBUG
	constexpr TCHAR kHookDllName[] = _T("../Release/EventHookd.dll");
#else
	constexpr TCHAR kHookDllName[] = _T("EventHookd.dll");
#endif
	constexpr TCHAR kHookLogFileName[] = _T("MacroLog.txt");

	// 設定ファイル（INI 形式）
	constexpr TCHAR kSectionTable[] = _T("TABLE");
	constexpr TCHAR kSectionCommon[] = _T("COMMON");
	constexpr TCHAR kKeyTableParam[] = _T("Param");
	constexpr TCHAR kKeyRepeatNum[] = _T("RepeatNum");
	constexpr TCHAR kKeyRepeatDelayMsec[] = _T("DelayMsec");
	// 1行の書式。末尾の改行も従来どおり書き込む
	// 実行回数, 遅延(ms), イベント種別, X, Y, マウス操作, 修飾キー, キー種別, キー文字列, コメント
	constexpr TCHAR kEventFormat[] = _T("%d,%d,%d,%d,%d,%d,%d,%d,%s,%s\n");
	constexpr size_t kEventFieldCount = 10;
	constexpr TCHAR kInvalidKeyText[] = _T("(null)");

	constexpr TCHAR kFileFilter[] = _T("Files (*.txt)|*.txt|All Files (*.*)|*.*|");
	constexpr TCHAR kFileDialogTitle[] = _T("ファイルを選択");
	constexpr TCHAR kDefaultExt[] = _T(".txt");

	constexpr TCHAR kSettingTitle[] = _T("設定");
	constexpr TCHAR kTotalTimeFormat[] = _T("　（総計： %02dh %02dm %02ds %03dmsec）");
	constexpr TCHAR kMsgSelectLastRow[] = _T("最終行を選択しました。処理を中断します");

	constexpr int kCheckCrisisMsec = 2000;		// 事故防止のため、1周がこれ以下で繰り返す設定は警告する
	constexpr int kMaxMousePointValue = 9999;
	constexpr int kMaxSleepValue = 99999999;
	constexpr int kExecuteNone = 0;
	constexpr int kExecuteMax = 32767;

	// 一覧の列
	enum ListColumn
	{
		kColumnNo,
		kColumnExecute,
		kColumnSleep,
		kColumnEvent,
		kColumnDetail,
		kColumnComment,
	};

	// 一覧の「詳細設定」列の文字列
	CString FormatEventDetail(const MacroEvent& ev)
	{
		CString text;
		if (ev.kind == EventKind::kKey)
		{
			if (ev.key.key_kind == kKeyKindUser)
			{
				if (ev.key.text[0] != '\0')
				{
					text.Format(_T("[%s]"), ev.key.text);
				}
			}
			else if (0 <= ev.key.key_kind && ev.key.key_kind < kKeyKindCount)
			{
				text = kKeyKindNames[ev.key.key_kind];
			}

			if (ev.key.modifiers & kModifierShift)	text += _T("　Shift");
			if (ev.key.modifiers & kModifierCtrl)	text += _T("　Ctrl");
			if (ev.key.modifiers & kModifierAlt)	text += _T("　Alt");
		}
		else if (ev.kind == EventKind::kMouse)
		{
			const bool valid = (0 <= ev.mouse.operation && ev.mouse.operation < kMouseOpCount);
			text.Format(_T("%s X[%4d] Y[%4d]"),
				valid ? kMouseOperationNames[ev.mouse.operation] : _T(""), ev.mouse.pt.x, ev.mouse.pt.y);
		}
		return text;
	}

	CString FormatEventString(const MacroEvent& ev)
	{
		CString text;
		text.Format(kEventFormat,
			ev.exec_count,
			ev.sleep_msec,
			static_cast<int>(ev.kind),
			ev.mouse.pt.x,
			ev.mouse.pt.y,
			ev.mouse.operation,
			ev.key.modifiers,
			ev.key.key_kind,
			ev.key.text,
			ev.comment);
		return text;
	}

	// 区切りは ',' と ';'。空のフィールドも1つと数え、足りないフィールドは空とみなす
	MacroEvent ParseEventString(const CString& line)
	{
		std::vector<CString> fields;
		int start = 0;
		for (int i = 0; i <= line.GetLength(); i++)
		{
			if (i == line.GetLength() || line[i] == _T(',') || line[i] == _T(';'))
			{
				fields.push_back(line.Mid(start, i - start));
				start = i + 1;
			}
		}
		while (fields.size() < kEventFieldCount)
		{
			fields.emplace_back();
		}

		MacroEvent ev{};
		ev.exec_count		= _ttoi(fields[0]);
		ev.sleep_msec		= _ttoi(fields[1]);
		ev.kind				= static_cast<EventKind>(_ttoi(fields[2]));
		ev.mouse.pt.x		= _ttoi(fields[3]);
		ev.mouse.pt.y		= _ttoi(fields[4]);
		ev.mouse.operation	= _ttoi(fields[5]);
		ev.key.modifiers	= _ttoi(fields[6]);
		ev.key.key_kind		= _ttoi(fields[7]);
		strcpy_s(ev.key.text, fields[8]);
		strcpy_s(ev.comment, fields[9]);
		return ev;
	}

	bool IsValidEvent(const MacroEvent& ev)
	{
		if (!(kExecuteNone <= ev.exec_count && ev.exec_count <= kExecuteMax))
		{
			return false;
		}
		if (ev.kind != EventKind::kMouse && ev.kind != EventKind::kKey)
		{
			return false;
		}
		if (ev.kind == EventKind::kMouse && !(0 <= ev.mouse.operation && ev.mouse.operation < kMouseOpCount))
		{
			return false;
		}
		if (ev.kind == EventKind::kKey && !(0 <= ev.key.key_kind && ev.key.key_kind < kKeyKindCount))
		{
			return false;
		}
		return strcmp(ev.key.text, kInvalidKeyText) != 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// AboutDlg : バージョン情報

class AboutDlg : public CDialog
{
public:
	AboutDlg() : CDialog(IDD) {}

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual BOOL OnInitDialog() override
	{
		CDialog::OnInitDialog();

		CString version;
		version.LoadString(IDS_VERSION);
		SetDlgItemText(IDC_VERSION, version);
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// MacroToolDlg

MacroToolDlg::MacroToolDlg(CWnd* parent, const CString& file_name, const std::vector<MacroEvent>& events,
	UINT& repeat_count, UINT& repeat_delay_msec)
	: CDialog(IDD, parent)
	, events_(kMaxEventCount)
	, clipboard_()
	, file_name_(file_name)
	, repeat_count_(repeat_count)
	, repeat_delay_msec_(repeat_delay_msec)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	hook_dll_ = LoadLibrary(kHookDllName);
	if (hook_dll_)
	{
		if constexpr (kUseKeyHook)
		{
			start_key_hook_ = reinterpret_cast<HookFunc>(GetProcAddress(hook_dll_, "StartKeyHook"));
			stop_key_hook_ = reinterpret_cast<HookFunc>(GetProcAddress(hook_dll_, "StopKeyHook"));
		}
		start_mouse_hook_ = reinterpret_cast<HookFunc>(GetProcAddress(hook_dll_, "StartMouseHook"));
		stop_mouse_hook_ = reinterpret_cast<HookFunc>(GetProcAddress(hook_dll_, "StopMouseHook"));
		debug_mode_ = reinterpret_cast<DebugModeFunc>(GetProcAddress(hook_dll_, "DebugMode"));
	}

	// 呼び出し元の設定を引き継ぐ
	for (int i = 0; i < kMaxEventCount && i < static_cast<int>(events.size()); i++)
	{
		if (events[i].kind == EventKind::kNone)
		{
			break;
		}
		events_[i] = events[i];
	}
}

MacroToolDlg::~MacroToolDlg()
{
	if (recording_)
	{
		StopRecord();
	}
}

void MacroToolDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
	DDX_Control(dx, IDC_LISTCTRL, list_);
	DDX_Control(dx, IDCB_MOUSE, mouse_combo_);
	DDX_Control(dx, IDET_KEY, key_edit_);
	DDX_Control(dx, IDCB_KEY, key_combo_);
}

BEGIN_MESSAGE_MAP(MacroToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, &MacroToolDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDBT_LIST_INSERT, &MacroToolDlg::OnListInsert)
	ON_BN_CLICKED(IDBT_LIST_DELETE, &MacroToolDlg::OnListDelete)
	ON_BN_CLICKED(IDBT_LIST_COPY, &MacroToolDlg::OnListCopy)
	ON_BN_CLICKED(IDBT_LIST_PASTE, &MacroToolDlg::OnListPaste)
	ON_BN_CLICKED(IDBT_ALLCLEAR, &MacroToolDlg::OnAllClear)
	ON_EN_CHANGE(IDET_EXECUTE, &MacroToolDlg::OnEnChangeExecute)
	ON_EN_CHANGE(IDET_SLEEP, &MacroToolDlg::OnEnChangeSleep)
	ON_EN_CHANGE(IDET_COMMENT, &MacroToolDlg::OnEnChangeComment)
	ON_BN_CLICKED(IDRB_MOUSE, &MacroToolDlg::OnMouse)
	ON_CBN_SELCHANGE(IDCB_MOUSE, &MacroToolDlg::OnCbnSelchangeMouse)
	ON_EN_CHANGE(IDET_MOUSE_X, &MacroToolDlg::OnEnChangeMouseX)
	ON_EN_CHANGE(IDET_MOUSE_Y, &MacroToolDlg::OnEnChangeMouseY)
	ON_BN_CLICKED(IDRB_KEY, &MacroToolDlg::OnKey)
	ON_CBN_SELCHANGE(IDCB_KEY, &MacroToolDlg::OnCbnSelchangeKey)
	ON_EN_CHANGE(IDET_KEY, &MacroToolDlg::OnEnChangeKey)
	ON_BN_CLICKED(IDCH_KEY_SHIFT, &MacroToolDlg::OnKeyShift)
	ON_BN_CLICKED(IDCH_KEY_CTRL, &MacroToolDlg::OnKeyCtrl)
	ON_BN_CLICKED(IDCH_KEY_ALT, &MacroToolDlg::OnKeyAlt)
	ON_EN_CHANGE(IDET_REPEAT_NUM, &MacroToolDlg::OnEnChangeRepeatNum)
	ON_EN_CHANGE(IDET_REPEAT_TIME, &MacroToolDlg::OnEnChangeRepeatTime)
	ON_BN_CLICKED(IDBT_READ, &MacroToolDlg::OnRead)
	ON_BN_CLICKED(IDBT_WRITE, &MacroToolDlg::OnWrite)
	ON_BN_CLICKED(IDBT_RECORD, &MacroToolDlg::OnRecord)
	ON_BN_CLICKED(IDBT_HELP, &MacroToolDlg::OnHelp)
END_MESSAGE_MAP()

std::vector<MacroEvent> MacroToolDlg::GetEvents() const
{
	std::vector<MacroEvent> events(kMaxEventCount);
	std::copy_if(events_.begin(), events_.end(), events.begin(),
		[](const MacroEvent& ev) { return ev.kind != EventKind::kNone; });
	return events;
}

BOOL MacroToolDlg::OnInitDialog()
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

	InitControls();

	return TRUE;
}

void MacroToolDlg::InitControls()
{
	list_.SetRowCount(kMaxEventCount);
	list_.EnableFullRowSelect();
	list_.AddColumn(kColumnNo, 25, _T("No"));
	list_.AddColumn(kColumnExecute, 45, _T("実行"));
	list_.AddColumn(kColumnSleep, 45, _T("遅延時間"));
	list_.AddColumn(kColumnEvent, 40, _T("イベント"));
	list_.AddColumn(kColumnDetail, 165, _T("詳細設定"));
	list_.AddColumn(kColumnComment, 46, _T("コメント"));
	list_.FillColumn(kColumnNo, _T(""));

	key_edit_.DisableCopyAndPaste(TRUE);

	CheckRadioButton(IDRB_MOUSE, IDRB_KEY, IDRB_MOUSE);

	SetDlgItemInt(IDET_REPEAT_NUM, repeat_count_);
	SetDlgItemInt(IDET_REPEAT_TIME, repeat_delay_msec_);

	mouse_combo_.ResetContent();
	for (int i = 0; i < kMouseOpCount; i++)
	{
		mouse_combo_.InsertString(i, kMouseOperationNames[i]);
		mouse_combo_.SetItemData(i, i);
	}
	mouse_combo_.SetCurSel(0);

	key_combo_.ResetContent();
	for (int i = 0; i < kKeyKindCount; i++)
	{
		key_combo_.InsertString(i, kKeyKindNames[i]);
		key_combo_.SetItemData(i, i);
	}
	key_combo_.SetCurSel(0);

	if (!hook_dll_)
	{
		GetDlgItem(IDBT_RECORD)->EnableWindow(FALSE);
	}

	UpdateListControl(TRUE);
	UpdateControl();
}

void MacroToolDlg::OnSysCommand(UINT id, LPARAM l_param)
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

void MacroToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// アイコンをクライアント領域の中央に描く
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

HCURSOR MacroToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// ダイアログ外にドラッグしてもカーソル位置を表示し続けるためにキャプチャする
void MacroToolDlg::OnLButtonDown(UINT flags, CPoint point)
{
	SetCapture();
	CDialog::OnLButtonDown(flags, point);
}

void MacroToolDlg::OnLButtonUp(UINT flags, CPoint point)
{
	ReleaseCapture();
	CDialog::OnLButtonUp(flags, point);
}

void MacroToolDlg::OnMouseMove(UINT flags, CPoint point)
{
	POINT pt;
	GetCursorPos(&pt);

	CString text;
	text.Format(_T("(%4d:%4d)"), pt.x, pt.y);
	SetDlgItemText(IDLB_STATES, text);

	CDialog::OnMouseMove(flags, point);
}

// 隠し機能：ダブルクリックでフック DLL のログ出力を切り替える
void MacroToolDlg::OnLButtonDblClk(UINT flags, CPoint point)
{
	debug_ = !debug_;

	CString text;
	text.Format(_T("DebugMode=%d"), debug_);
	MessageBox(text);

	if (debug_mode_)
	{
		debug_mode_(debug_);
	}

	CDialog::OnLButtonDblClk(flags, point);
}

void MacroToolDlg::OnLvnItemchangedList(NMHDR* nmhdr, LRESULT* result)
{
	LPNMLISTVIEW nmlv = reinterpret_cast<LPNMLISTVIEW>(nmhdr);
	if (nmlv && nmlv->iItem != index_)
	{
		index_ = nmlv->iItem;
		UpdateControl();
	}
	*result = 0;
}

/////////////////////////////////////////////////////////////////////////////
// 一覧の編集

void MacroToolDlg::OnListInsert()
{
	const int last_index = kMaxEventCount - 1;
	if (index_ == last_index)
	{
		MessageBox(kMsgSelectLastRow);
		return;
	}

	// 選択行から後ろを1行ずつ下げる（最終行は押し出されて消える）
	std::copy_backward(events_.begin() + index_, events_.end() - 1, events_.end());
	CurrentEvent() = MacroEvent{};
	UpdateListControl(TRUE);
}

void MacroToolDlg::OnListDelete()
{
	// 選択行より後ろを1行ずつ上げ、空いた最終行は未設定にする
	std::copy(events_.begin() + index_ + 1, events_.end(), events_.begin() + index_);
	events_.back() = MacroEvent{};
	UpdateListControl(TRUE);
}

void MacroToolDlg::OnListCopy()
{
	clipboard_ = CurrentEvent();
}

void MacroToolDlg::OnListPaste()
{
	CurrentEvent() = clipboard_;
	UpdateListControl();
	UpdateControl();
}

void MacroToolDlg::OnAllClear()
{
	std::fill(events_.begin(), events_.end(), MacroEvent{});
	UpdateListControl(TRUE);
	UpdateControl();
}

/////////////////////////////////////////////////////////////////////////////
// 選択行の設定

// 未設定の行を編集し始めたら、その種別のイベントにする
void MacroToolDlg::EnsureEventKind(EventKind kind)
{
	if (CurrentEvent().kind == EventKind::kNone)
	{
		CurrentEvent().kind = kind;
		UpdateControl();
	}
}

// ラジオボタンで種別を選んだとき
void MacroToolDlg::SelectEventKind(EventKind kind)
{
	if (CurrentEvent().kind == EventKind::kNone)
	{
		CurrentEvent().exec_count = 1;
	}
	CurrentEvent().kind = kind;

	UpdateControl();
	UpdateListControl();
}

// 上限を超えた値は上限に書き換える
int MacroToolDlg::GetClampedDlgItemInt(UINT id, int max_value, BOOL is_signed)
{
	const int value = static_cast<int>(GetDlgItemInt(id, nullptr, is_signed));
	if (value > max_value)
	{
		SetDlgItemInt(id, max_value, is_signed);
		return max_value;
	}
	return value;
}

void MacroToolDlg::OnEnChangeExecute()
{
	CurrentEvent().exec_count = GetClampedDlgItemInt(IDET_EXECUTE, kExecuteMax, FALSE);
	SetTitleBar();
	UpdateListControl();
}

void MacroToolDlg::OnEnChangeSleep()
{
	CurrentEvent().sleep_msec = GetClampedDlgItemInt(IDET_SLEEP, kMaxSleepValue, FALSE);
	SetTitleBar();
	UpdateListControl();
}

void MacroToolDlg::OnEnChangeComment()
{
	CString text;
	GetDlgItemText(IDET_COMMENT, text);
	strcpy_s(CurrentEvent().comment, text);
	UpdateListControl();
}

void MacroToolDlg::OnMouse()
{
	SelectEventKind(EventKind::kMouse);
}

void MacroToolDlg::OnCbnSelchangeMouse()
{
	CurrentEvent().mouse.operation = mouse_combo_.GetCurSel();
	EnsureEventKind(EventKind::kMouse);
	UpdateListControl();
}

void MacroToolDlg::OnEnChangeMouseX()
{
	CurrentEvent().mouse.pt.x = GetClampedDlgItemInt(IDET_MOUSE_X, kMaxMousePointValue, TRUE);
	EnsureEventKind(EventKind::kMouse);
	UpdateListControl();
}

void MacroToolDlg::OnEnChangeMouseY()
{
	CurrentEvent().mouse.pt.y = GetClampedDlgItemInt(IDET_MOUSE_Y, kMaxMousePointValue, TRUE);
	EnsureEventKind(EventKind::kMouse);
	UpdateListControl();
}

void MacroToolDlg::OnKey()
{
	SelectEventKind(EventKind::kKey);
}

void MacroToolDlg::OnCbnSelchangeKey()
{
	CurrentEvent().key.key_kind = key_combo_.GetCurSel();
	EnsureEventKind(EventKind::kKey);
	UpdateListControl();
}

void MacroToolDlg::OnEnChangeKey()
{
	CString text;
	GetDlgItemText(IDET_KEY, text);
	strcpy_s(CurrentEvent().key.text, text);
	EnsureEventKind(EventKind::kKey);
	UpdateListControl();
}

void MacroToolDlg::SetModifier(UINT check_id, DWORD modifier)
{
	if (IsDlgButtonChecked(check_id) == BST_CHECKED)
	{
		CurrentEvent().key.modifiers |= modifier;
	}
	else
	{
		CurrentEvent().key.modifiers &= ~modifier;
	}
	EnsureEventKind(EventKind::kKey);
	UpdateListControl();
}

void MacroToolDlg::OnKeyShift()	{ SetModifier(IDCH_KEY_SHIFT, kModifierShift); }
void MacroToolDlg::OnKeyCtrl()		{ SetModifier(IDCH_KEY_CTRL, kModifierCtrl); }
void MacroToolDlg::OnKeyAlt()		{ SetModifier(IDCH_KEY_ALT, kModifierAlt); }

/////////////////////////////////////////////////////////////////////////////
// 共通設定

void MacroToolDlg::OnEnChangeRepeatNum()
{
	repeat_count_ = GetDlgItemInt(IDET_REPEAT_NUM, nullptr, FALSE);
}

void MacroToolDlg::OnEnChangeRepeatTime()
{
	repeat_delay_msec_ = GetDlgItemInt(IDET_REPEAT_TIME, nullptr, FALSE);
	SetTitleBar();
}

/////////////////////////////////////////////////////////////////////////////
// ファイル

void MacroToolDlg::OnRead()
{
	if (SelectFile(TRUE))
	{
		LoadFile(file_name_);
	}
}

void MacroToolDlg::LoadFile(const CString& file_name)
{
	std::fill(events_.begin(), events_.end(), MacroEvent{});

	repeat_count_ = _ttoi(GetIniFileParam(file_name, kSectionCommon, kKeyRepeatNum));
	repeat_delay_msec_ = _ttoi(GetIniFileParam(file_name, kSectionCommon, kKeyRepeatDelayMsec));

	int count = 0;
	for (int i = 0; i < kMaxEventCount; i++)
	{
		CString key_name;
		key_name.Format(_T("%s%d"), kKeyTableParam, i);
		const CString line = GetIniFileParam(file_name, kSectionTable, key_name);
		if (line.IsEmpty())
		{
			break;
		}

		const MacroEvent ev = ParseEventString(line);
		if (!IsValidEvent(ev))
		{
			continue;
		}

		// 同じ座標で「押す」の直後に「離す」が来たら、前の行を「クリック」にまとめる
		// （キーの行もマウスの欄に値を持っているので、両方マウスの行のときだけ）
		if (count > 0 && ev.kind == EventKind::kMouse && events_[count - 1].kind == EventKind::kMouse)
		{
			MacroMouse& prev = events_[count - 1].mouse;
			if (prev.pt == ev.mouse.pt)
			{
				if (ev.mouse.operation == kMouseOpLUp && prev.operation == kMouseOpLDown)
				{
					prev.operation = kMouseOpLClick;
					continue;
				}
				if (ev.mouse.operation == kMouseOpRUp && prev.operation == kMouseOpRDown)
				{
					prev.operation = kMouseOpRClick;
					continue;
				}
			}
		}

		events_[count++] = ev;
	}

	UpdateListControl(TRUE);
	UpdateControl();

	SetDlgItemInt(IDET_REPEAT_NUM, repeat_count_);
	SetDlgItemInt(IDET_REPEAT_TIME, repeat_delay_msec_);
}

void MacroToolDlg::OnWrite()
{
	if (!SelectFile(FALSE))
	{
		return;
	}

	SetIniFileParam(file_name_, kSectionCommon, kKeyRepeatNum, static_cast<int>(repeat_count_));
	SetIniFileParam(file_name_, kSectionCommon, kKeyRepeatDelayMsec, static_cast<int>(repeat_delay_msec_));

	// 未設定の行は飛ばし、番号を詰めて書く
	int index = 0;
	for (const MacroEvent& ev : events_)
	{
		if (ev.kind == EventKind::kNone)
		{
			continue;
		}

		CString key_name;
		key_name.Format(_T("%s%d"), kKeyTableParam, index);
		SetIniFileParam(file_name_, kSectionTable, key_name, FormatEventString(ev));
		index++;
	}
}

BOOL MacroToolDlg::SelectFile(BOOL is_open)
{
	CFileDialog dlg(is_open, nullptr, file_name_, OFN_HIDEREADONLY, kFileFilter);
	dlg.m_ofn.lpstrTitle = kFileDialogTitle;
	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	file_name_ = dlg.GetPathName();
	if (!is_open)
	{
		// 拡張子が無ければ .txt を付けて保存する
		CString ext;
		SplitPath(file_name_, nullptr, nullptr, nullptr, &ext);
		if (ext.IsEmpty())
		{
			file_name_ += kDefaultExt;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 記録

void MacroToolDlg::OnRecord()
{
	TCHAR temp_dir[MAX_PATH];
	GetTempPath(MAX_PATH, temp_dir);
	CString log_path(temp_dir);
	AppendPath(log_path, kHookLogFileName);

	if (!recording_)
	{
		CString title;
		title.Format(_T("%s(Record中)"), kSettingTitle);
		SetWindowText(title);
		SetDlgItemText(IDBT_RECORD, _T("記録終了"));

		::remove(log_path);	// 前回のログを消しておく
		StartRecord();
	}
	else
	{
		SetTitleBar();
		SetDlgItemText(IDBT_RECORD, _T("記録"));
		StopRecord();

		LoadFile(log_path);
	}
}

void MacroToolDlg::StartRecord()
{
	bool started;
	if constexpr (kUseKeyHook)
	{
		started = start_key_hook_ && start_key_hook_() && start_mouse_hook_ && start_mouse_hook_();
	}
	else
	{
		started = start_mouse_hook_ && start_mouse_hook_();
	}

	if (started)
	{
		recording_ = TRUE;
	}
	else
	{
		MessageBox(_T("StartMouseHook() fail"));
	}
}

void MacroToolDlg::StopRecord()
{
	bool stopped;
	if constexpr (kUseKeyHook)
	{
		stopped = stop_key_hook_ && stop_key_hook_() && stop_mouse_hook_ && stop_mouse_hook_();
	}
	else
	{
		stopped = stop_mouse_hook_ && stop_mouse_hook_();
	}

	if (stopped)
	{
		recording_ = FALSE;
	}
	else
	{
		MessageBox(_T("StopMouseHook() fail"));
	}
}

// 「ヘルプ」ボタンは動作確認用：NumLock / CapsLock / ScrollLock を順に点滅させる
void MacroToolDlg::OnHelp()
{
	const BYTE lock_keys[] = { VK_NUMLOCK, VK_CAPITAL, VK_SCROLL };
	for (int i = 0; i < 10; i++)
	{
		for (BYTE key : lock_keys)
		{
			InputSimulator::FunctionKeyAction(key, TRUE);
			Sleep(100);
			InputSimulator::FunctionKeyAction(key, FALSE);
			Sleep(100);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 設定欄の表示更新

void MacroToolDlg::SetDlgItemNumberIfChanged(UINT id, int value)
{
	CString current;
	GetDlgItemText(id, current);
	if (_ttoi(current) != value)
	{
		SetDlgItemInt(id, value, TRUE);
	}
}

void MacroToolDlg::SetDlgItemTextIfChanged(UINT id, LPCTSTR text)
{
	CString current;
	GetDlgItemText(id, current);
	if (current.Compare(text) != 0)
	{
		SetDlgItemText(id, text);
	}
}

void MacroToolDlg::CheckDlgButtonIfChanged(UINT id, bool check)
{
	const UINT state = check ? BST_CHECKED : BST_UNCHECKED;
	if (IsDlgButtonChecked(id) != state)
	{
		CheckDlgButton(id, state);
	}
}

// 値を書き換えると EN_CHANGE で選択行が更新されるので、変わったものだけ書き換える
void MacroToolDlg::UpdateControl()
{
	SetDlgItemNumberIfChanged(IDET_EXECUTE, CurrentEvent().exec_count);
	UpdateControlEvent();
	UpdateControlDetail();
	SetDlgItemNumberIfChanged(IDET_SLEEP, static_cast<int>(CurrentEvent().sleep_msec));

	SetTitleBar();
}

void MacroToolDlg::UpdateControlEvent()
{
	const int check_id = (CurrentEvent().kind == EventKind::kKey) ? IDRB_KEY : IDRB_MOUSE;
	if (GetCheckedRadioButton(IDRB_MOUSE, IDRB_KEY) != check_id)
	{
		CheckRadioButton(IDRB_MOUSE, IDRB_KEY, check_id);
	}
}

void MacroToolDlg::UpdateControlDetail()
{
	SetDlgItemTextIfChanged(IDET_COMMENT, CurrentEvent().comment);
	SetDlgItemNumberIfChanged(IDET_MOUSE_X, CurrentEvent().mouse.pt.x);
	SetDlgItemNumberIfChanged(IDET_MOUSE_Y, CurrentEvent().mouse.pt.y);
	SetDlgItemTextIfChanged(IDET_KEY, CurrentEvent().key.text);

	CheckDlgButtonIfChanged(IDCH_KEY_SHIFT, (CurrentEvent().key.modifiers & kModifierShift) != 0);
	CheckDlgButtonIfChanged(IDCH_KEY_CTRL, (CurrentEvent().key.modifiers & kModifierCtrl) != 0);
	CheckDlgButtonIfChanged(IDCH_KEY_ALT, (CurrentEvent().key.modifiers & kModifierAlt) != 0);

	mouse_combo_.SetCurSel(CurrentEvent().mouse.operation);
	key_combo_.SetCurSel(CurrentEvent().key.key_kind);
}

/////////////////////////////////////////////////////////////////////////////
// 一覧の表示更新

// update_all = FALSE なら選択行だけ
// （セルを書き換えると LVN_ITEMCHANGED で選択行が変わりうるので、選択行はセルごとに取り直す）
void MacroToolDlg::UpdateListControl(BOOL update_all)
{
	const int columns[] = { kColumnExecute, kColumnSleep, kColumnEvent, kColumnDetail, kColumnComment };
	if (update_all)
	{
		// 全行を書き換える間は再描画を止める
		list_.SetRedraw(FALSE);
		for (int row = 0; row < kMaxEventCount; row++)
		{
			for (int column : columns)
			{
				UpdateListCell(row, column);
			}
		}
		list_.SetRedraw(TRUE);
		list_.Invalidate();
	}
	else
	{
		for (int column : columns)
		{
			UpdateListCell(index_, column);
		}
	}
}

void MacroToolDlg::UpdateListCell(int row, int column)
{
	const MacroEvent& ev = events_[row];

	CString text;
	if (ev.kind != EventKind::kNone)
	{
		switch (column)
		{
		case kColumnExecute:
			text.Format(_T("%d"), ev.exec_count);
			break;
		case kColumnSleep:
			text.Format(_T("%d"), ev.sleep_msec);
			break;
		case kColumnEvent:
			if (ev.kind == EventKind::kKey)
			{
				text = _T("キー");
			}
			else if (ev.kind == EventKind::kMouse)
			{
				text = _T("マウス");
			}
			break;
		case kColumnDetail:
			text = FormatEventDetail(ev);
			break;
		case kColumnComment:
			text = ev.comment;
			break;
		}
	}
	list_.SetItemText(row, column, text);
}

/////////////////////////////////////////////////////////////////////////////
// タイトル・確認

// 1周にかかる時間(ms)
int MacroToolDlg::GetTotalTime() const
{
	int msec = static_cast<int>(repeat_delay_msec_);
	for (const MacroEvent& ev : events_)
	{
		if (ev.kind != EventKind::kNone)
		{
			msec += static_cast<int>(ev.sleep_msec * ev.exec_count);
		}
	}
	return msec;
}

void MacroToolDlg::SetTitleBar()
{
	int rest = GetTotalTime();
	const int msec = rest % 1000;	rest /= 1000;
	const int sec = rest % 60;		rest /= 60;
	const int min = rest % 60;		rest /= 60;
	const int hour = rest % 24;

	CString total;
	total.Format(kTotalTimeFormat, hour, min, sec, msec);
	SetWindowText(CString(kSettingTitle) + total);
}

// 2回以上繰り返すのに1周が短すぎる設定は、見直すか確認する
BOOL MacroToolDlg::ConfirmSettings()
{
	if (repeat_count_ <= 1 || GetTotalTime() > kCheckCrisisMsec)
	{
		return TRUE;
	}

	const int ret = MessageBox(
		_T("下記が設定されました。設定を見直しますか？\n　・2回以上の実行回数\n　・１回の実行時間が2秒未満"),
		_T("Warning"), MB_YESNO);
	return ret == IDNO;
}

void MacroToolDlg::OnOK()
{
	if (!ConfirmSettings())
	{
		return;		// 設定し直す
	}
	CDialog::OnOK();
}
