// memory_dlg.cc : メインダイアログ（出題）

#include "stdafx.h"
#include "memory.h"
#include "memory_dlg.h"
#include "answer_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class AboutDlg : public CDialog
{
public:
	enum { IDD = IDD_ABOUTBOX };

	AboutDlg() : CDialog(IDD) {}
};

/////////////////////////////////////////////////////////////////////////////
// MemoryDlg

MemoryDlg::MemoryDlg(CWnd* parent)
	: CDialog(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MemoryDlg::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
	DDX_Control(dx, IDC_VIEW_SPEED, cycle_bar_);
}

BEGIN_MESSAGE_MAP(MemoryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, &MemoryDlg::OnStart)
	ON_BN_CLICKED(IDC_ANS, &MemoryDlg::OnAns)
	ON_BN_CLICKED(IDC_KEISAN, &MemoryDlg::OnKeisan)
	ON_BN_CLICKED(IDC_ANKI, &MemoryDlg::OnAnki)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_ENG_SMALL, IDC_NUMBER, &MemoryDlg::OnTypeCheck)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(ID_HELP, &MemoryDlg::OnHelp)
END_MESSAGE_MAP()

BOOL MemoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない
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

	timer_cycle_ = kCycInitVal;
	selected_mode_ = PlayMode::kAnki;
	type_flags_ = 0;

	GetDlgItem(IDCANCEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_PR_NUM, kNumInitVal, FALSE);
	SetDlgItemInt(IDC_PR_KETA, kKetaInitVal, FALSE);

	if (kModeInitVal == PlayMode::kAnki)
	{
		CheckDlgButton(IDC_ANKI, TRUE);
		OnAnki();
	}
	else
	{
		CheckDlgButton(IDC_KEISAN, TRUE);
		OnKeisan();
	}

	state_ = PlayState::kInit;
	cycle_bar_.SetScrollRange(kMinCyc, kMaxCyc, TRUE);
	cycle_bar_.SetScrollPos(kCycInitVal);
	CString str;
	str.Format(_T("(%d ms)"), timer_cycle_);
	GetDlgItem(IDC_SPEED_TXT)->SetWindowText(str);
	InitProc();

	return TRUE;
}

void MemoryDlg::OnSysCommand(UINT id, LPARAM l_param)
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
void MemoryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int icon_width = GetSystemMetrics(SM_CXICON);
		int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - icon_width + 1) / 2;
		int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR MemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void MemoryDlg::OnTimer(UINT_PTR id_event)
{
	if (id_event == kGenerateId)
	{
		ViewText();
	}
	CDialog::OnTimer(id_event);
}

void MemoryDlg::ViewText()
{
	CString str;

	if (count_ < problem_count_)
	{
		str = KeyGen();
	}
	else
	{
		EndProc();
	}
	GetDlgItem(IDC_SHOW)->SetWindowText(str);
}

void MemoryDlg::OnStart()
{
	if (type_flags_ == 0)
	{
		AfxMessageBox(_T("どれかひとつはチェック入れて下さい。"));
		return;
	}

	const int rt = StartCheck();
	if (rt == kCheckOk)
	{
		InitProc();
		StartProc();
		return;
	}

	CString str = _T("入力値エラーです。\n\n");
	if (rt & kCheckNumErr)
	{
		str += _T("正常な出題数を入力して下さい。\n");
	}
	if (rt & kCheckKetaErr)
	{
		str += _T("正常な桁数を入力して下さい。\n");
	}
	AfxMessageBox(str);
}

// 出題数・桁数の入力値をチェックし、正常なら取り込む
int MemoryDlg::StartCheck()
{
	int rt = kCheckOk;

	int wk = GetDlgItemInt(IDC_PR_NUM, nullptr, FALSE);
	if (wk < kMinVal || wk > kPrNumMax)
	{
		rt |= kCheckNumErr;
	}
	else
	{
		problem_count_ = wk;
		SetDlgItemInt(IDC_PR_NUM, problem_count_, FALSE);
	}

	const int max_val = (selected_mode_ == PlayMode::kKeisan) ? kKetaKeisanMax : kKetaAnkiMax;

	wk = GetDlgItemInt(IDC_PR_KETA, nullptr, FALSE);
	if (wk < kMinVal || wk > max_val)
	{
		rt |= kCheckKetaErr;
	}
	else
	{
		digits_ = wk;
		SetDlgItemInt(IDC_PR_KETA, digits_, FALSE);
	}
	return rt;
}

void MemoryDlg::OnAns()
{
	AnserDlg dlg(*this, this);
	dlg.DoModal();
}

void MemoryDlg::OnAnki()
{
	SelectMode(PlayMode::kAnki);
}

void MemoryDlg::OnKeisan()
{
	SelectMode(PlayMode::kKeisan);
}

// 計算モードは数字だけで出題するため、文字の種類を選べなくする
void MemoryDlg::SelectMode(PlayMode mode)
{
	selected_mode_ = mode;
	const BOOL anki = (mode == PlayMode::kAnki) ? TRUE : FALSE;

	if (!anki)
	{
		SetType(kTypeNumber, TRUE);
		SetType(kTypeEngSmall, FALSE);
		SetType(kTypeEngLarge, FALSE);
	}

	GetDlgItem(IDC_ENG_SMALL)->EnableWindow(anki);
	GetDlgItem(IDC_ENG_LARGE)->EnableWindow(anki);
	GetDlgItem(IDC_NUMBER)->EnableWindow(anki);
	CheckDlgButton(IDC_ENG_SMALL, (type_flags_ & kTypeEngSmall) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_ENG_LARGE, (type_flags_ & kTypeEngLarge) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_NUMBER, (type_flags_ & kTypeNumber) ? BST_CHECKED : BST_UNCHECKED);
	GetDlgItem(IDC_KETA_STR)->SetWindowText(anki ? _T("(1～15)") : _T("(1～3)"));

	const int max_digits = anki ? kKetaAnkiMax : kKetaKeisanMax;
	digits_ = GetDlgItemInt(IDC_PR_KETA, nullptr, FALSE);
	if (digits_ < kMinVal || digits_ > max_digits)
	{
		digits_ = kKetaInitVal;
		SetDlgItemInt(IDC_PR_KETA, digits_, FALSE);
	}
}

void MemoryDlg::StartProc()
{
	if (state_ == PlayState::kPlaying)
	{
		EndProc();
		return;
	}

	state_ = PlayState::kPlaying;
	play_mode_ = selected_mode_;
	if (play_mode_ == PlayMode::kKeisan)
	{
		SetType(kTypeNumber, TRUE);
	}
	ItemSts(FALSE);
	GetDlgItem(IDC_START)->SetWindowText(_T("ストップ"));

	// 先に表示を消しておかないとチラつく
	GetDlgItem(IDC_SHOW)->SetWindowText(_T(""));

	// 1問目はすぐに出す
	ViewText();
	SetTimer(kGenerateId, timer_cycle_, nullptr);
}

void MemoryDlg::EndProc()
{
	count_ = 0;
	state_ = PlayState::kEnd;
	ItemSts(TRUE);
	GetDlgItem(IDC_START)->SetWindowText(_T("開始"));
	GetDlgItem(IDC_SHOW)->SetWindowText(_T(""));
	KillTimer(kGenerateId);
}

void MemoryDlg::InitProc()
{
	for (CString& record : record_)
	{
		record.Empty();
	}
	count_ = 0;
}

// 出題中は設定を変えられないようにする
void MemoryDlg::ItemSts(BOOL flg)
{
	GetDlgItem(IDC_PR_NUM)->EnableWindow(flg);
	GetDlgItem(IDC_PR_KETA)->EnableWindow(flg);
	GetDlgItem(IDC_ANS)->EnableWindow(flg);
	GetDlgItem(IDC_ANKI)->EnableWindow(flg);
	GetDlgItem(IDC_KEISAN)->EnableWindow(flg);
	GetDlgItem(IDC_VIEW_SPEED)->EnableWindow(flg);

	// 計算モードでは文字の種類は常に選べない
	if (play_mode_ != PlayMode::kKeisan)
	{
		GetDlgItem(IDC_NUMBER)->EnableWindow(flg);
		GetDlgItem(IDC_ENG_SMALL)->EnableWindow(flg);
		GetDlgItem(IDC_ENG_LARGE)->EnableWindow(flg);
	}
}

// 1問分の文字列を作って記録する
CString MemoryDlg::KeyGen()
{
	// フォントは最初の1回だけ作り、アプリ終了まで使い回す
	if (show_font_.GetSafeHandle() == nullptr)
	{
		LOGFONT view_font = {};
		view_font.lfCharSet = DEFAULT_CHARSET;
		view_font.lfWeight = kShowWeight;
		view_font.lfHeight = kShowHeight;
		show_font_.CreateFontIndirect(&view_font);
	}
	SendDlgItemMessage(IDC_SHOW, WM_SETFONT, reinterpret_cast<WPARAM>(show_font_.GetSafeHandle()), MAKELPARAM(TRUE, 0));

	// time() は1秒単位でしか変わらず、1秒以内に呼ぶと前回と同じ乱数列になるため、
	// 出題番号を掛けて毎回違う種にする
	srand(static_cast<unsigned>(time(nullptr)) * (count_ + 1) * 2);

	CString str;
	for (int i = 0; i < digits_; i++)
	{
		const int val = rand();
		str += GetKeyGenChar(GetKeyGenType(val), val);
	}
	record_[count_] = str;
	count_++;

	return str;
}

// 乱数を3で割った余りで、選択中の種類から優先順位を付けて文字の種類を決める
int MemoryDlg::GetKeyGenType(int val) const
{
	switch (val % 3)
	{
	case 1:
		if (type_flags_ & kTypeEngSmall)	return kTypeEngSmall;
		if (type_flags_ & kTypeEngLarge)	return kTypeEngLarge;
		return kTypeNumber;

	case 2:
		if (type_flags_ & kTypeEngLarge)	return kTypeEngLarge;
		if (type_flags_ & kTypeNumber)		return kTypeNumber;
		return kTypeEngSmall;

	default:
		if (type_flags_ & kTypeNumber)		return kTypeNumber;
		if (type_flags_ & kTypeEngLarge)	return kTypeEngLarge;
		return kTypeEngSmall;
	}
}

// 種類に応じた1文字を作る。直前と同じ文字にはしない
TCHAR MemoryDlg::GetKeyGenChar(int char_type, int val)
{
	int number = 0;

	switch (char_type)
	{
	case kTypeNumber:
		number = val % 10;
		if (number == prev_char_)
		{
			number = MatchProc(val, number);
		}
		prev_char_ = number;
		return static_cast<TCHAR>(_T('0') + number);

	case kTypeEngSmall:
		number = _T('a') + (val % 26);
		if (number == prev_char_)
		{
			number = MatchProc(val, number - _T('a')) + _T('a');
		}
		break;

	case kTypeEngLarge:
		number = _T('A') + (val % 26);
		if (number == prev_char_)
		{
			number = MatchProc(val, number - _T('A')) + _T('A');
		}
		break;
	}

	prev_char_ = number;
	return static_cast<TCHAR>(number);
}

// orgVal の各桁を下から見て、current と違う最初の数字を返す（無ければ current のまま）
int MemoryDlg::MatchProc(int org_val, int current)
{
	while (org_val != 0)
	{
		if (current != org_val % 10)
		{
			return org_val % 10;
		}
		org_val /= 10;
	}
	return current;
}

void MemoryDlg::OnTypeCheck(UINT id)
{
	int set_type = 0;
	switch (id)
	{
	case IDC_ENG_SMALL:	set_type = kTypeEngSmall;	break;
	case IDC_ENG_LARGE:	set_type = kTypeEngLarge;	break;
	case IDC_NUMBER:	set_type = kTypeNumber;		break;
	default:			return;
	}

	SetType(set_type, IsDlgButtonChecked(id));
}

// スクロールバーで表示速度を 10ms 単位で変える
void MemoryDlg::OnHScroll(UINT sb_code, UINT pos, CScrollBar* scroll_bar)
{
	timer_cycle_ = cycle_bar_.GetScrollPos();

	int min_pos, max_pos;
	cycle_bar_.GetScrollRange(&min_pos, &max_pos);
	switch (sb_code)
	{
	case SB_LINELEFT:
		timer_cycle_ -= 10;
		break;
	case SB_LINERIGHT:
		timer_cycle_ += 10;
		break;
	case SB_PAGELEFT:
		timer_cycle_ -= 100;
		break;
	case SB_PAGERIGHT:
		timer_cycle_ += 100;
		break;
	case SB_LEFT:
		timer_cycle_ = min_pos;
		break;
	case SB_RIGHT:
		timer_cycle_ = max_pos;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		timer_cycle_ = pos;
		break;
	default:
		break;
	}

	if (timer_cycle_ < kMinCyc)
	{
		timer_cycle_ = kMinCyc;
	}
	else if (timer_cycle_ > kMaxCyc)
	{
		timer_cycle_ = kMaxCyc;
	}

	timer_cycle_ = timer_cycle_ / 10 * 10;
	cycle_bar_.SetScrollPos(timer_cycle_);

	CString str;
	str.Format(_T("(%d ms)"), timer_cycle_);
	GetDlgItem(IDC_SPEED_TXT)->SetWindowText(str);

	CDialog::OnHScroll(sb_code, pos, scroll_bar);
}

void MemoryDlg::OnHelp()
{
	MessageBox(
		_T("　　右脳左脳活性化ソフト　ヘルプ\n")
		_T("\n")
		_T("○出題数\t出題する数\n")
		_T("○桁数\t１問につき出題される桁数\n")
		_T("\n")
		_T("○プレイモード選択（計算 / 暗記）\n")
		_T("　・計算\t出題された数を全て加算する\n")
		_T("　・暗記\t出題された文字を全て暗記する\n")
		_T("\n")
		_T("○プレイタイプ選択（数字 / 英小字 / 英大字）\n")
		_T("　・数字\t出題する文字に数字を追加\n")
		_T("　・英小字\t出題する文字に英小字を追加\n")
		_T("　・英大字\t出題する文字に英大字を追加\n")
		_T("\n")
		_T("○表示速度　問題の表示速度を10ms単位で調整\n")
		_T("\n")
		_T("○ボタン\n")
		_T("　・開始\tスタート\n")
		_T("　・解答\t答え合わせ\n")
		_T("　・終了\tアプリの終了\n")
		_T("　・ヘルプ\t今押したボタン\n"),
		_T("ヘルプ"), MB_OK);
}

void MemoryDlg::SetType(int set_type, BOOL flg)
{
	if (flg == TRUE)
	{
		type_flags_ |= set_type;
	}
	else
	{
		type_flags_ &= ~set_type;
	}
}
