// MacroToolDlg.cpp : 設定ダイアログ（イベントの一覧編集・ファイル読み書き・記録）

#include "stdafx.h"
#include "MacroTool.h"
#include "MacroToolDlg.h"
#include "Util.h"
#include "InputSimulator.h"
#include "CommonUtil.h"

#include <cstdio>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr bool USE_KEY_HOOK = false;	// キーボードの記録は未完成

#ifdef _DEBUG
	constexpr TCHAR HOOK_DLL_NAME[] = _T("../Release/EventHookd.dll");
#else
	constexpr TCHAR HOOK_DLL_NAME[] = _T("EventHookd.dll");
#endif
	constexpr TCHAR HOOK_LOG_FILE_NAME[] = _T("MacroLog.txt");

	// 設定ファイル（INI 形式）
	constexpr TCHAR SECTION_TABLE[] = _T("TABLE");
	constexpr TCHAR SECTION_COMMON[] = _T("COMMON");
	constexpr TCHAR KEY_TABLE_PARAM[] = _T("Param");
	constexpr TCHAR KEY_REPEAT_NUM[] = _T("RepeatNum");
	constexpr TCHAR KEY_REPEAT_DELAY_MSEC[] = _T("DelayMsec");
	// 1行の書式。末尾の改行も従来どおり書き込む
	// 実行回数, 遅延(ms), イベント種別, X, Y, マウス操作, 修飾キー, キー種別, キー文字列, コメント
	constexpr TCHAR EVENT_FORMAT[] = _T("%d,%d,%d,%d,%d,%d,%d,%d,%s,%s\n");
	constexpr size_t EVENT_FIELD_COUNT = 10;
	constexpr TCHAR INVALID_KEY_TEXT[] = _T("(null)");

	constexpr TCHAR FILE_FILTER[] = _T("Files (*.txt)|*.txt|All Files (*.*)|*.*|");
	constexpr TCHAR FILE_DIALOG_TITLE[] = _T("ファイルを選択");
	constexpr TCHAR DEFAULT_EXT[] = _T(".txt");

	constexpr TCHAR SETTING_TITLE[] = _T("設定");
	constexpr TCHAR TOTAL_TIME_FORMAT[] = _T("　（総計： %02dh %02dm %02ds %03dmsec）");
	constexpr TCHAR MSG_SELECT_LAST_ROW[] = _T("最終行を選択しました。処理を中断します");

	constexpr int CHECK_CRISIS_MSEC = 2000;		// 事故防止のため、1周がこれ以下で繰り返す設定は警告する
	constexpr int MAX_MOUSE_POINT_VALUE = 9999;
	constexpr int MAX_SLEEP_VALUE = 99999999;
	constexpr int EXECUTE_NONE = 0;
	constexpr int EXECUTE_MAX = 32767;

	// 一覧の列
	enum ListColumn
	{
		COLUMN_NO,
		COLUMN_EXECUTE,
		COLUMN_SLEEP,
		COLUMN_EVENT,
		COLUMN_DETAIL,
		COLUMN_COMMENT,
	};

	// 一覧の「詳細設定」列の文字列
	CString FormatEventDetail(const MACROEVENT& ev)
	{
		CString text;
		if (ev.kind == EventKind::Key)
		{
			if (ev.key.keyKind == KEYKIND_USER)
			{
				if (ev.key.text[0] != '\0')
				{
					text.Format(_T("[%s]"), ev.key.text);
				}
			}
			else if (0 <= ev.key.keyKind && ev.key.keyKind < KEYKIND_COUNT)
			{
				text = KEY_KIND_NAMES[ev.key.keyKind];
			}

			if (ev.key.modifiers & MODIFIER_SHIFT)	text += _T("　Shift");
			if (ev.key.modifiers & MODIFIER_CTRL)	text += _T("　Ctrl");
			if (ev.key.modifiers & MODIFIER_ALT)	text += _T("　Alt");
		}
		else if (ev.kind == EventKind::Mouse)
		{
			const bool valid = (0 <= ev.mouse.operation && ev.mouse.operation < MOUSEOP_COUNT);
			text.Format(_T("%s X[%4d] Y[%4d]"),
				valid ? MOUSE_OPERATION_NAMES[ev.mouse.operation] : _T(""), ev.mouse.pt.x, ev.mouse.pt.y);
		}
		return text;
	}

	CString FormatEventString(const MACROEVENT& ev)
	{
		CString text;
		text.Format(EVENT_FORMAT,
			ev.execCount,
			ev.sleepMsec,
			static_cast<int>(ev.kind),
			ev.mouse.pt.x,
			ev.mouse.pt.y,
			ev.mouse.operation,
			ev.key.modifiers,
			ev.key.keyKind,
			ev.key.text,
			ev.comment);
		return text;
	}

	// 区切りは ',' と ';'。空のフィールドも1つと数え、足りないフィールドは空とみなす
	MACROEVENT ParseEventString(const CString& line)
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
		while (fields.size() < EVENT_FIELD_COUNT)
		{
			fields.emplace_back();
		}

		MACROEVENT ev{};
		ev.execCount		= _ttoi(fields[0]);
		ev.sleepMsec		= _ttoi(fields[1]);
		ev.kind				= static_cast<EventKind>(_ttoi(fields[2]));
		ev.mouse.pt.x		= _ttoi(fields[3]);
		ev.mouse.pt.y		= _ttoi(fields[4]);
		ev.mouse.operation	= _ttoi(fields[5]);
		ev.key.modifiers	= _ttoi(fields[6]);
		ev.key.keyKind		= _ttoi(fields[7]);
		strcpy_s(ev.key.text, fields[8]);
		strcpy_s(ev.comment, fields[9]);
		return ev;
	}

	bool IsValidEvent(const MACROEVENT& ev)
	{
		if (!(EXECUTE_NONE <= ev.execCount && ev.execCount <= EXECUTE_MAX))
		{
			return false;
		}
		if (ev.kind != EventKind::Mouse && ev.kind != EventKind::Key)
		{
			return false;
		}
		if (ev.kind == EventKind::Mouse && !(0 <= ev.mouse.operation && ev.mouse.operation < MOUSEOP_COUNT))
		{
			return false;
		}
		if (ev.kind == EventKind::Key && !(0 <= ev.key.keyKind && ev.key.keyKind < KEYKIND_COUNT))
		{
			return false;
		}
		return strcmp(ev.key.text, INVALID_KEY_TEXT) != 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg : バージョン情報

class CAboutDlg : public CDialog
{
public:
	CAboutDlg() : CDialog(IDD) {}

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
// CMacroToolDlg

CMacroToolDlg::CMacroToolDlg(CWnd* pParent, const CString& fileName, const std::vector<MACROEVENT>& events,
	UINT& repeatCount, UINT& repeatDelayMsec)
	: CDialog(IDD, pParent)
	, m_events(MAX_EVENT_COUNT)
	, m_clipboard()
	, m_fileName(fileName)
	, m_repeatCount(repeatCount)
	, m_repeatDelayMsec(repeatDelayMsec)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hHookDll = LoadLibrary(HOOK_DLL_NAME);
	if (m_hHookDll)
	{
		if constexpr (USE_KEY_HOOK)
		{
			m_pfnStartKeyHook = reinterpret_cast<HookFunc>(GetProcAddress(m_hHookDll, "StartKeyHook"));
			m_pfnStopKeyHook = reinterpret_cast<HookFunc>(GetProcAddress(m_hHookDll, "StopKeyHook"));
		}
		m_pfnStartMouseHook = reinterpret_cast<HookFunc>(GetProcAddress(m_hHookDll, "StartMouseHook"));
		m_pfnStopMouseHook = reinterpret_cast<HookFunc>(GetProcAddress(m_hHookDll, "StopMouseHook"));
		m_pfnDebugMode = reinterpret_cast<DebugModeFunc>(GetProcAddress(m_hHookDll, "DebugMode"));
	}

	// 呼び出し元の設定を引き継ぐ
	for (int i = 0; i < MAX_EVENT_COUNT && i < static_cast<int>(events.size()); i++)
	{
		if (events[i].kind == EventKind::None)
		{
			break;
		}
		m_events[i] = events[i];
	}
}

CMacroToolDlg::~CMacroToolDlg()
{
	if (m_bRecording)
	{
		StopRecord();
	}
}

void CMacroToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL, m_list);
	DDX_Control(pDX, IDCB_MOUSE, m_mouseCombo);
	DDX_Control(pDX, IDET_KEY, m_keyEdit);
	DDX_Control(pDX, IDCB_KEY, m_keyCombo);
}

BEGIN_MESSAGE_MAP(CMacroToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, &CMacroToolDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDBT_LIST_INSERT, &CMacroToolDlg::OnListInsert)
	ON_BN_CLICKED(IDBT_LIST_DELETE, &CMacroToolDlg::OnListDelete)
	ON_BN_CLICKED(IDBT_LIST_COPY, &CMacroToolDlg::OnListCopy)
	ON_BN_CLICKED(IDBT_LIST_PASTE, &CMacroToolDlg::OnListPaste)
	ON_BN_CLICKED(IDBT_ALLCLEAR, &CMacroToolDlg::OnAllClear)
	ON_EN_CHANGE(IDET_EXECUTE, &CMacroToolDlg::OnEnChangeExecute)
	ON_EN_CHANGE(IDET_SLEEP, &CMacroToolDlg::OnEnChangeSleep)
	ON_EN_CHANGE(IDET_COMMENT, &CMacroToolDlg::OnEnChangeComment)
	ON_BN_CLICKED(IDRB_MOUSE, &CMacroToolDlg::OnMouse)
	ON_CBN_SELCHANGE(IDCB_MOUSE, &CMacroToolDlg::OnCbnSelchangeMouse)
	ON_EN_CHANGE(IDET_MOUSE_X, &CMacroToolDlg::OnEnChangeMouseX)
	ON_EN_CHANGE(IDET_MOUSE_Y, &CMacroToolDlg::OnEnChangeMouseY)
	ON_BN_CLICKED(IDRB_KEY, &CMacroToolDlg::OnKey)
	ON_CBN_SELCHANGE(IDCB_KEY, &CMacroToolDlg::OnCbnSelchangeKey)
	ON_EN_CHANGE(IDET_KEY, &CMacroToolDlg::OnEnChangeKey)
	ON_BN_CLICKED(IDCH_KEY_SHIFT, &CMacroToolDlg::OnKeyShift)
	ON_BN_CLICKED(IDCH_KEY_CTRL, &CMacroToolDlg::OnKeyCtrl)
	ON_BN_CLICKED(IDCH_KEY_ALT, &CMacroToolDlg::OnKeyAlt)
	ON_EN_CHANGE(IDET_REPEAT_NUM, &CMacroToolDlg::OnEnChangeRepeatNum)
	ON_EN_CHANGE(IDET_REPEAT_TIME, &CMacroToolDlg::OnEnChangeRepeatTime)
	ON_BN_CLICKED(IDBT_READ, &CMacroToolDlg::OnRead)
	ON_BN_CLICKED(IDBT_WRITE, &CMacroToolDlg::OnWrite)
	ON_BN_CLICKED(IDBT_RECORD, &CMacroToolDlg::OnRecord)
	ON_BN_CLICKED(IDBT_HELP, &CMacroToolDlg::OnHelp)
END_MESSAGE_MAP()

std::vector<MACROEVENT> CMacroToolDlg::GetEvents() const
{
	std::vector<MACROEVENT> events(MAX_EVENT_COUNT);
	int count = 0;
	for (const MACROEVENT& ev : m_events)
	{
		if (ev.kind != EventKind::None)
		{
			events[count++] = ev;
		}
	}
	return events;
}

BOOL CMacroToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	static_assert((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX && IDM_ABOUTBOX < 0xF000, "IDM_ABOUTBOX はシステムコマンドの範囲内にする");
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

	InitControls();

	return TRUE;
}

void CMacroToolDlg::InitControls()
{
	m_list.SetRowCount(MAX_EVENT_COUNT);
	m_list.EnableFullRowSelect();
	m_list.AddColumn(COLUMN_NO, 25, _T("No"));
	m_list.AddColumn(COLUMN_EXECUTE, 45, _T("実行"));
	m_list.AddColumn(COLUMN_SLEEP, 45, _T("遅延時間"));
	m_list.AddColumn(COLUMN_EVENT, 40, _T("イベント"));
	m_list.AddColumn(COLUMN_DETAIL, 165, _T("詳細設定"));
	m_list.AddColumn(COLUMN_COMMENT, 46, _T("コメント"));
	m_list.FillColumn(COLUMN_NO, _T(""));

	m_keyEdit.DisableCopyAndPaste(TRUE);

	CheckRadioButton(IDRB_MOUSE, IDRB_KEY, IDRB_MOUSE);

	SetDlgItemInt(IDET_REPEAT_NUM, m_repeatCount);
	SetDlgItemInt(IDET_REPEAT_TIME, m_repeatDelayMsec);

	m_mouseCombo.ResetContent();
	for (int i = 0; i < MOUSEOP_COUNT; i++)
	{
		m_mouseCombo.InsertString(i, MOUSE_OPERATION_NAMES[i]);
		m_mouseCombo.SetItemData(i, i);
	}
	m_mouseCombo.SetCurSel(0);

	m_keyCombo.ResetContent();
	for (int i = 0; i < KEYKIND_COUNT; i++)
	{
		m_keyCombo.InsertString(i, KEY_KIND_NAMES[i]);
		m_keyCombo.SetItemData(i, i);
	}
	m_keyCombo.SetCurSel(0);

	if (!m_hHookDll)
	{
		GetDlgItem(IDBT_RECORD)->EnableWindow(FALSE);
	}

	UpdateListControl(TRUE);
	UpdateControl();
}

void CMacroToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMacroToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// アイコンをクライアント領域の中央に描く
		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		dc.DrawIcon((rect.Width() - cxIcon + 1) / 2, (rect.Height() - cyIcon + 1) / 2, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMacroToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ダイアログ外にドラッグしてもカーソル位置を表示し続けるためにキャプチャする
void CMacroToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}

void CMacroToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CMacroToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT pt;
	GetCursorPos(&pt);

	CString text;
	text.Format(_T("(%4d:%4d)"), pt.x, pt.y);
	SetDlgItemText(IDLB_STATES, text);

	CDialog::OnMouseMove(nFlags, point);
}

// 隠し機能：ダブルクリックでフック DLL のログ出力を切り替える
void CMacroToolDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bDebug = !m_bDebug;

	CString text;
	text.Format(_T("DebugMode=%d"), m_bDebug);
	MessageBox(text);

	if (m_pfnDebugMode)
	{
		m_pfnDebugMode(m_bDebug);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CMacroToolDlg::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV && pNMLV->iItem != m_index)
	{
		m_index = pNMLV->iItem;
		UpdateControl();
	}
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// 一覧の編集

void CMacroToolDlg::OnListInsert()
{
	const int lastIndex = MAX_EVENT_COUNT - 1;
	if (m_index == lastIndex)
	{
		MessageBox(MSG_SELECT_LAST_ROW);
		return;
	}

	for (int i = lastIndex; i > m_index; i--)
	{
		m_events[i] = m_events[i - 1];
	}
	CurrentEvent() = MACROEVENT{};
	UpdateListControl(TRUE);
}

void CMacroToolDlg::OnListDelete()
{
	const int lastIndex = MAX_EVENT_COUNT - 1;
	for (int i = m_index; i < lastIndex; i++)
	{
		m_events[i] = m_events[i + 1];
	}
	m_events[lastIndex] = MACROEVENT{};
	UpdateListControl(TRUE);
}

void CMacroToolDlg::OnListCopy()
{
	m_clipboard = CurrentEvent();
}

void CMacroToolDlg::OnListPaste()
{
	CurrentEvent() = m_clipboard;
	UpdateListControl();
	UpdateControl();
}

void CMacroToolDlg::OnAllClear()
{
	std::fill(m_events.begin(), m_events.end(), MACROEVENT{});
	UpdateListControl(TRUE);
	UpdateControl();
}

/////////////////////////////////////////////////////////////////////////////
// 選択行の設定

// 未設定の行を編集し始めたら、その種別のイベントにする
void CMacroToolDlg::EnsureEventKind(EventKind kind)
{
	if (CurrentEvent().kind == EventKind::None)
	{
		CurrentEvent().kind = kind;
		UpdateControl();
	}
}

// ラジオボタンで種別を選んだとき
void CMacroToolDlg::SelectEventKind(EventKind kind)
{
	if (CurrentEvent().kind == EventKind::None)
	{
		CurrentEvent().execCount = 1;
	}
	CurrentEvent().kind = kind;

	UpdateControl();
	UpdateListControl();
}

// 上限を超えた値は上限に書き換える
int CMacroToolDlg::GetClampedDlgItemInt(UINT id, int maxValue, BOOL bSigned)
{
	BOOL bValid;
	int value = static_cast<int>(GetDlgItemInt(id, &bValid, bSigned));
	if (value > maxValue)
	{
		value = maxValue;

		CString text;
		text.Format(_T("%d"), value);
		SetDlgItemText(id, text);
	}
	return value;
}

void CMacroToolDlg::OnEnChangeExecute()
{
	CurrentEvent().execCount = GetClampedDlgItemInt(IDET_EXECUTE, EXECUTE_MAX, FALSE);
	SetTitleBar();
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeSleep()
{
	CurrentEvent().sleepMsec = GetClampedDlgItemInt(IDET_SLEEP, MAX_SLEEP_VALUE, FALSE);
	SetTitleBar();
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeComment()
{
	CString text;
	GetDlgItemText(IDET_COMMENT, text);
	strcpy_s(CurrentEvent().comment, text);
	UpdateListControl();
}

void CMacroToolDlg::OnMouse()
{
	SelectEventKind(EventKind::Mouse);
}

void CMacroToolDlg::OnCbnSelchangeMouse()
{
	CurrentEvent().mouse.operation = m_mouseCombo.GetCurSel();
	EnsureEventKind(EventKind::Mouse);
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeMouseX()
{
	CurrentEvent().mouse.pt.x = GetClampedDlgItemInt(IDET_MOUSE_X, MAX_MOUSE_POINT_VALUE, TRUE);
	EnsureEventKind(EventKind::Mouse);
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeMouseY()
{
	CurrentEvent().mouse.pt.y = GetClampedDlgItemInt(IDET_MOUSE_Y, MAX_MOUSE_POINT_VALUE, TRUE);
	EnsureEventKind(EventKind::Mouse);
	UpdateListControl();
}

void CMacroToolDlg::OnKey()
{
	SelectEventKind(EventKind::Key);
}

void CMacroToolDlg::OnCbnSelchangeKey()
{
	CurrentEvent().key.keyKind = m_keyCombo.GetCurSel();
	EnsureEventKind(EventKind::Key);
	UpdateListControl();
}

void CMacroToolDlg::OnEnChangeKey()
{
	CString text;
	GetDlgItemText(IDET_KEY, text);
	strcpy_s(CurrentEvent().key.text, text);
	EnsureEventKind(EventKind::Key);
	UpdateListControl();
}

void CMacroToolDlg::SetModifier(UINT checkId, DWORD modifier)
{
	if (IsDlgButtonChecked(checkId) == BST_CHECKED)
	{
		CurrentEvent().key.modifiers |= modifier;
	}
	else
	{
		CurrentEvent().key.modifiers &= ~modifier;
	}
	EnsureEventKind(EventKind::Key);
	UpdateListControl();
}

void CMacroToolDlg::OnKeyShift()	{ SetModifier(IDCH_KEY_SHIFT, MODIFIER_SHIFT); }
void CMacroToolDlg::OnKeyCtrl()		{ SetModifier(IDCH_KEY_CTRL, MODIFIER_CTRL); }
void CMacroToolDlg::OnKeyAlt()		{ SetModifier(IDCH_KEY_ALT, MODIFIER_ALT); }

/////////////////////////////////////////////////////////////////////////////
// 共通設定

void CMacroToolDlg::OnEnChangeRepeatNum()
{
	BOOL bValid;
	m_repeatCount = GetDlgItemInt(IDET_REPEAT_NUM, &bValid, FALSE);
}

void CMacroToolDlg::OnEnChangeRepeatTime()
{
	BOOL bValid;
	m_repeatDelayMsec = GetDlgItemInt(IDET_REPEAT_TIME, &bValid, FALSE);
	SetTitleBar();
}

/////////////////////////////////////////////////////////////////////////////
// ファイル

void CMacroToolDlg::OnRead()
{
	if (SelectFile(TRUE))
	{
		LoadFile(m_fileName);
	}
}

void CMacroToolDlg::LoadFile(const CString& fileName)
{
	std::fill(m_events.begin(), m_events.end(), MACROEVENT{});

	m_repeatCount = _ttoi(GetIniFileParam(fileName, SECTION_COMMON, KEY_REPEAT_NUM));
	m_repeatDelayMsec = _ttoi(GetIniFileParam(fileName, SECTION_COMMON, KEY_REPEAT_DELAY_MSEC));

	int count = 0;
	for (int i = 0; i < MAX_EVENT_COUNT; i++)
	{
		CString keyName;
		keyName.Format(_T("%s%d"), KEY_TABLE_PARAM, i);
		const CString line = GetIniFileParam(fileName, SECTION_TABLE, keyName);
		if (line.IsEmpty())
		{
			break;
		}

		const MACROEVENT ev = ParseEventString(line);
		if (!IsValidEvent(ev))
		{
			continue;
		}

		// 同じ座標で「押す」の直後に「離す」が来たら、前の行を「クリック」にまとめる
		if (count > 0)
		{
			MACROMOUSE& prev = m_events[count - 1].mouse;
			if (prev.pt == ev.mouse.pt)
			{
				if (ev.mouse.operation == MOUSEOP_LUP && prev.operation == MOUSEOP_LDOWN)
				{
					prev.operation = MOUSEOP_LCLICK;
					continue;
				}
				if (ev.mouse.operation == MOUSEOP_RUP && prev.operation == MOUSEOP_RDOWN)
				{
					prev.operation = MOUSEOP_RCLICK;
					continue;
				}
			}
		}

		m_events[count++] = ev;
	}

	UpdateListControl(TRUE);
	UpdateControl();

	SetDlgItemInt(IDET_REPEAT_NUM, m_repeatCount);
	SetDlgItemInt(IDET_REPEAT_TIME, m_repeatDelayMsec);
}

void CMacroToolDlg::OnWrite()
{
	if (!SelectFile(FALSE))
	{
		return;
	}

	SetIniFileParam(m_fileName, SECTION_COMMON, KEY_REPEAT_NUM, static_cast<int>(m_repeatCount));
	SetIniFileParam(m_fileName, SECTION_COMMON, KEY_REPEAT_DELAY_MSEC, static_cast<int>(m_repeatDelayMsec));

	// 未設定の行は飛ばし、番号を詰めて書く
	int index = 0;
	for (const MACROEVENT& ev : m_events)
	{
		if (ev.kind == EventKind::None)
		{
			continue;
		}

		CString keyName;
		keyName.Format(_T("%s%d"), KEY_TABLE_PARAM, index);
		SetIniFileParam(m_fileName, SECTION_TABLE, keyName, FormatEventString(ev));
		index++;
	}
}

BOOL CMacroToolDlg::SelectFile(BOOL bOpen)
{
	CFileDialog dlg(bOpen, nullptr, m_fileName, OFN_HIDEREADONLY, FILE_FILTER);
	dlg.m_ofn.lpstrTitle = FILE_DIALOG_TITLE;
	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	m_fileName = dlg.GetPathName();
	if (!bOpen)
	{
		// 拡張子が無ければ .txt を付けて保存する
		CString ext;
		SplitPath(m_fileName, nullptr, nullptr, nullptr, &ext);
		if (ext.IsEmpty())
		{
			m_fileName += DEFAULT_EXT;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 記録

void CMacroToolDlg::OnRecord()
{
	TCHAR tempDir[MAX_PATH];
	GetTempPath(MAX_PATH, tempDir);
	CString logPath;
	logPath.Format(_T("%s\\%s"), tempDir, HOOK_LOG_FILE_NAME);

	if (!m_bRecording)
	{
		CString title;
		title.Format(_T("%s(Record中)"), SETTING_TITLE);
		SetWindowText(title);
		SetDlgItemText(IDBT_RECORD, _T("記録終了"));

		::remove(logPath);	// 前回のログを消しておく
		StartRecord();
	}
	else
	{
		SetTitleBar();
		SetDlgItemText(IDBT_RECORD, _T("記録"));
		StopRecord();

		LoadFile(logPath);
	}
}

void CMacroToolDlg::StartRecord()
{
	bool started;
	if constexpr (USE_KEY_HOOK)
	{
		started = m_pfnStartKeyHook && m_pfnStartKeyHook() && m_pfnStartMouseHook && m_pfnStartMouseHook();
	}
	else
	{
		started = m_pfnStartMouseHook && m_pfnStartMouseHook();
	}

	if (started)
	{
		m_bRecording = TRUE;
	}
	else
	{
		MessageBox(_T("StartMouseHook() fail"));
	}
}

void CMacroToolDlg::StopRecord()
{
	bool stopped;
	if constexpr (USE_KEY_HOOK)
	{
		stopped = m_pfnStopKeyHook && m_pfnStopKeyHook() && m_pfnStopMouseHook && m_pfnStopMouseHook();
	}
	else
	{
		stopped = m_pfnStopMouseHook && m_pfnStopMouseHook();
	}

	if (stopped)
	{
		m_bRecording = FALSE;
	}
	else
	{
		MessageBox(_T("StopMouseHook() fail"));
	}
}

// 「ヘルプ」ボタンは動作確認用：NumLock / CapsLock / ScrollLock を順に点滅させる
void CMacroToolDlg::OnHelp()
{
	const BYTE lockKeys[] = { VK_NUMLOCK, VK_CAPITAL, VK_SCROLL };
	for (int i = 0; i < 10; i++)
	{
		for (BYTE key : lockKeys)
		{
			CInputSimulator::FunctionKeyAction(key, TRUE);
			Sleep(100);
			CInputSimulator::FunctionKeyAction(key, FALSE);
			Sleep(100);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 設定欄の表示更新

void CMacroToolDlg::SetDlgItemNumberIfChanged(UINT id, int value)
{
	CString current;
	GetDlgItemText(id, current);
	if (_ttoi(current) != value)
	{
		CString text;
		text.Format(_T("%d"), value);
		SetDlgItemText(id, text);
	}
}

void CMacroToolDlg::SetDlgItemTextIfChanged(UINT id, LPCTSTR text)
{
	CString current;
	GetDlgItemText(id, current);
	if (current.Compare(text) != 0)
	{
		SetDlgItemText(id, text);
	}
}

void CMacroToolDlg::CheckDlgButtonIfChanged(UINT id, bool check)
{
	const UINT state = check ? BST_CHECKED : BST_UNCHECKED;
	if (IsDlgButtonChecked(id) != state)
	{
		CheckDlgButton(id, state);
	}
}

// 値を書き換えると EN_CHANGE で選択行が更新されるので、変わったものだけ書き換える
void CMacroToolDlg::UpdateControl()
{
	SetDlgItemNumberIfChanged(IDET_EXECUTE, CurrentEvent().execCount);
	UpdateControlEvent();
	UpdateControlDetail();
	SetDlgItemNumberIfChanged(IDET_SLEEP, static_cast<int>(CurrentEvent().sleepMsec));

	SetTitleBar();
}

void CMacroToolDlg::UpdateControlEvent()
{
	const int checkId = (CurrentEvent().kind == EventKind::Key) ? IDRB_KEY : IDRB_MOUSE;
	if (GetCheckedRadioButton(IDRB_MOUSE, IDRB_KEY) != checkId)
	{
		CheckRadioButton(IDRB_MOUSE, IDRB_KEY, checkId);
	}
}

void CMacroToolDlg::UpdateControlDetail()
{
	SetDlgItemTextIfChanged(IDET_COMMENT, CurrentEvent().comment);
	SetDlgItemNumberIfChanged(IDET_MOUSE_X, CurrentEvent().mouse.pt.x);
	SetDlgItemNumberIfChanged(IDET_MOUSE_Y, CurrentEvent().mouse.pt.y);
	SetDlgItemTextIfChanged(IDET_KEY, CurrentEvent().key.text);

	CheckDlgButtonIfChanged(IDCH_KEY_SHIFT, (CurrentEvent().key.modifiers & MODIFIER_SHIFT) != 0);
	CheckDlgButtonIfChanged(IDCH_KEY_CTRL, (CurrentEvent().key.modifiers & MODIFIER_CTRL) != 0);
	CheckDlgButtonIfChanged(IDCH_KEY_ALT, (CurrentEvent().key.modifiers & MODIFIER_ALT) != 0);

	m_mouseCombo.SetCurSel(CurrentEvent().mouse.operation);
	m_keyCombo.SetCurSel(CurrentEvent().key.keyKind);
}

/////////////////////////////////////////////////////////////////////////////
// 一覧の表示更新

// bAll = FALSE なら選択行だけ
// （セルを書き換えると LVN_ITEMCHANGED で選択行が変わりうるので、選択行はセルごとに取り直す）
void CMacroToolDlg::UpdateListControl(BOOL bAll)
{
	const int columns[] = { COLUMN_EXECUTE, COLUMN_SLEEP, COLUMN_EVENT, COLUMN_DETAIL, COLUMN_COMMENT };
	if (bAll)
	{
		for (int row = 0; row < MAX_EVENT_COUNT; row++)
		{
			for (int column : columns)
			{
				UpdateListCell(row, column);
			}
		}
	}
	else
	{
		for (int column : columns)
		{
			UpdateListCell(m_index, column);
		}
	}
}

void CMacroToolDlg::UpdateListCell(int row, int column)
{
	const MACROEVENT& ev = m_events[row];

	CString text;
	if (ev.kind != EventKind::None)
	{
		switch (column)
		{
		case COLUMN_EXECUTE:
			text.Format(_T("%d"), ev.execCount);
			break;
		case COLUMN_SLEEP:
			text.Format(_T("%d"), ev.sleepMsec);
			break;
		case COLUMN_EVENT:
			if (ev.kind == EventKind::Key)
			{
				text = _T("キー");
			}
			else if (ev.kind == EventKind::Mouse)
			{
				text = _T("マウス");
			}
			break;
		case COLUMN_DETAIL:
			text = FormatEventDetail(ev);
			break;
		case COLUMN_COMMENT:
			text = ev.comment;
			break;
		}
	}
	m_list.SetItemText(row, column, text);
}

/////////////////////////////////////////////////////////////////////////////
// タイトル・確認

// 1周にかかる時間(ms)
int CMacroToolDlg::GetTotalTime() const
{
	int msec = static_cast<int>(m_repeatDelayMsec);
	for (const MACROEVENT& ev : m_events)
	{
		if (ev.kind != EventKind::None)
		{
			msec += static_cast<int>(ev.sleepMsec * ev.execCount);
		}
	}
	return msec;
}

void CMacroToolDlg::SetTitleBar()
{
	int rest = GetTotalTime();
	const int msec = rest % 1000;	rest /= 1000;
	const int sec = rest % 60;		rest /= 60;
	const int min = rest % 60;		rest /= 60;
	const int hour = rest % 24;

	CString total;
	total.Format(TOTAL_TIME_FORMAT, hour, min, sec, msec);
	SetWindowText(CString(SETTING_TITLE) + total);
}

// 2回以上繰り返すのに1周が短すぎる設定は、見直すか確認する
BOOL CMacroToolDlg::ConfirmSettings()
{
	if (m_repeatCount <= 1 || GetTotalTime() > CHECK_CRISIS_MSEC)
	{
		return TRUE;
	}

	const int ret = MessageBox(
		_T("下記が設定されました。設定を見直しますか？\n　・2回以上の実行回数\n　・１回の実行時間が2秒未満"),
		_T("Warning"), MB_YESNO);
	return ret == IDNO;
}

void CMacroToolDlg::OnOK()
{
	if (!ConfirmSettings())
	{
		return;		// 設定し直す
	}
	CDialog::OnOK();
}
