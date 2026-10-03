// memory_dlg.cc : メインダイアログ（出題）

#include "stdafx.h"
#include "memory.h"
#include "memory_dlg.h"
#include "anser_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログ

class CAboutDlg : public CDialog
{
public:
	enum { IDD = IDD_ABOUTBOX };

	CAboutDlg() : CDialog(IDD) {}
};

/////////////////////////////////////////////////////////////////////////////
// CMemoryDlg

CMemoryDlg::CMemoryDlg(CWnd* pParent)
	: CDialog(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_SPEED, m_cycleBar);
}

BEGIN_MESSAGE_MAP(CMemoryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, &CMemoryDlg::OnStart)
	ON_BN_CLICKED(IDC_ANS, &CMemoryDlg::OnAns)
	ON_BN_CLICKED(IDC_KEISAN, &CMemoryDlg::OnKeisan)
	ON_BN_CLICKED(IDC_ANKI, &CMemoryDlg::OnAnki)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_ENG_SMALL, IDC_NUMBER, &CMemoryDlg::OnTypeCheck)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(ID_HELP, &CMemoryDlg::OnHelp)
END_MESSAGE_MAP()

BOOL CMemoryDlg::OnInitDialog()
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

	m_timerCycle = CYC_INIT_VAL;
	m_selectedMode = PlayMode::Anki;
	m_typeFlags = 0;

	GetDlgItem(IDCANCEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_PR_NUM, NUM_INIT_VAL, FALSE);
	SetDlgItemInt(IDC_PR_KETA, KETA_INIT_VAL, FALSE);

	if (MODE_INIT_VAL == PlayMode::Anki)
	{
		CheckDlgButton(IDC_ANKI, TRUE);
		OnAnki();
	}
	else
	{
		CheckDlgButton(IDC_KEISAN, TRUE);
		OnKeisan();
	}

	m_state = PlayState::Init;
	m_cycleBar.SetScrollRange(MIN_CYC, MAX_CYC, TRUE);
	m_cycleBar.SetScrollPos(CYC_INIT_VAL);
	CString str;
	str.Format(_T("(%d ms)"), m_timerCycle);
	GetDlgItem(IDC_SPEED_TXT)->SetWindowText(str);
	InitProc();

	return TRUE;
}

void CMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CMemoryDlg::OnPaint()
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

HCURSOR CMemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMemoryDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == GENERATE_ID)
	{
		ViewText();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMemoryDlg::ViewText()
{
	CString str;

	if (m_count < m_problemCount)
	{
		str = KeyGen();
	}
	else
	{
		EndProc();
	}
	GetDlgItem(IDC_SHOW)->SetWindowText(str);
}

void CMemoryDlg::OnStart()
{
	if (m_typeFlags == 0)
	{
		AfxMessageBox(_T("どれかひとつはチェック入れて下さい。"));
		return;
	}

	const int rt = StartCheck();
	if (rt == CHECK_OK)
	{
		InitProc();
		StartProc();
		return;
	}

	CString str = _T("入力値エラーです。\n\n");
	if (rt & CHECK_NUM_ERR)
	{
		str += _T("正常な出題数を入力して下さい。\n");
	}
	if (rt & CHECK_KETA_ERR)
	{
		str += _T("正常な桁数を入力して下さい。\n");
	}
	AfxMessageBox(str);
}

// 出題数・桁数の入力値をチェックし、正常なら取り込む
int CMemoryDlg::StartCheck()
{
	int rt = CHECK_OK;

	int wk = GetDlgItemInt(IDC_PR_NUM, nullptr, FALSE);
	if (wk < MIN_VAL || wk > PR_NUM_MAX)
	{
		rt |= CHECK_NUM_ERR;
	}
	else
	{
		m_problemCount = wk;
		SetDlgItemInt(IDC_PR_NUM, m_problemCount, FALSE);
	}

	const int maxVal = (m_selectedMode == PlayMode::Keisan) ? KETA_KEISAN_MAX : KETA_ANIKI_MAX;

	wk = GetDlgItemInt(IDC_PR_KETA, nullptr, FALSE);
	if (wk < MIN_VAL || wk > maxVal)
	{
		rt |= CHECK_KETA_ERR;
	}
	else
	{
		m_digits = wk;
		SetDlgItemInt(IDC_PR_KETA, m_digits, FALSE);
	}
	return rt;
}

void CMemoryDlg::OnAns()
{
	CAnserDlg dlg(*this, this);
	dlg.DoModal();
}

void CMemoryDlg::OnAnki()
{
	SelectMode(PlayMode::Anki);
}

void CMemoryDlg::OnKeisan()
{
	SelectMode(PlayMode::Keisan);
}

// 計算モードは数字だけで出題するため、文字の種類を選べなくする
void CMemoryDlg::SelectMode(PlayMode mode)
{
	m_selectedMode = mode;
	const BOOL bAnki = (mode == PlayMode::Anki) ? TRUE : FALSE;

	if (!bAnki)
	{
		SetType(TYPE_NUMBER, TRUE);
		SetType(TYPE_ENG_SMALL, FALSE);
		SetType(TYPE_ENG_LARGE, FALSE);
	}

	GetDlgItem(IDC_ENG_SMALL)->EnableWindow(bAnki);
	GetDlgItem(IDC_ENG_LARGE)->EnableWindow(bAnki);
	GetDlgItem(IDC_NUMBER)->EnableWindow(bAnki);
	CheckDlgButton(IDC_ENG_SMALL, (m_typeFlags & TYPE_ENG_SMALL) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_ENG_LARGE, (m_typeFlags & TYPE_ENG_LARGE) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_NUMBER, (m_typeFlags & TYPE_NUMBER) ? BST_CHECKED : BST_UNCHECKED);
	GetDlgItem(IDC_KETA_STR)->SetWindowText(bAnki ? _T("(1～15)") : _T("(1～3)"));

	const int maxDigits = bAnki ? KETA_ANIKI_MAX : KETA_KEISAN_MAX;
	m_digits = GetDlgItemInt(IDC_PR_KETA, nullptr, FALSE);
	if (m_digits < MIN_VAL || m_digits > maxDigits)
	{
		m_digits = KETA_INIT_VAL;
		SetDlgItemInt(IDC_PR_KETA, m_digits, FALSE);
	}
}

void CMemoryDlg::StartProc()
{
	if (m_state == PlayState::Playing)
	{
		EndProc();
		return;
	}

	m_state = PlayState::Playing;
	m_playMode = m_selectedMode;
	if (m_playMode == PlayMode::Keisan)
	{
		SetType(TYPE_NUMBER, TRUE);
	}
	ItemSts(FALSE);
	GetDlgItem(IDC_START)->SetWindowText(_T("ストップ"));

	// 先に表示を消しておかないとチラつく
	GetDlgItem(IDC_SHOW)->SetWindowText(_T(""));

	// 1問目はすぐに出す
	ViewText();
	SetTimer(GENERATE_ID, m_timerCycle, nullptr);
}

void CMemoryDlg::EndProc()
{
	m_count = 0;
	m_state = PlayState::End;
	ItemSts(TRUE);
	GetDlgItem(IDC_START)->SetWindowText(_T("開始"));
	GetDlgItem(IDC_SHOW)->SetWindowText(_T(""));
	KillTimer(GENERATE_ID);
}

void CMemoryDlg::InitProc()
{
	for (CString& record : m_record)
	{
		record.Empty();
	}
	m_count = 0;
}

// 出題中は設定を変えられないようにする
void CMemoryDlg::ItemSts(BOOL flg)
{
	GetDlgItem(IDC_PR_NUM)->EnableWindow(flg);
	GetDlgItem(IDC_PR_KETA)->EnableWindow(flg);
	GetDlgItem(IDC_ANS)->EnableWindow(flg);
	GetDlgItem(IDC_ANKI)->EnableWindow(flg);
	GetDlgItem(IDC_KEISAN)->EnableWindow(flg);
	GetDlgItem(IDC_VIEW_SPEED)->EnableWindow(flg);

	// 計算モードでは文字の種類は常に選べない
	if (m_playMode != PlayMode::Keisan)
	{
		GetDlgItem(IDC_NUMBER)->EnableWindow(flg);
		GetDlgItem(IDC_ENG_SMALL)->EnableWindow(flg);
		GetDlgItem(IDC_ENG_LARGE)->EnableWindow(flg);
	}
}

// 1問分の文字列を作って記録する
CString CMemoryDlg::KeyGen()
{
	// フォントは最初の1回だけ作り、アプリ終了まで使い回す
	if (m_showFont.GetSafeHandle() == nullptr)
	{
		LOGFONT viewFont = {};
		viewFont.lfCharSet = DEFAULT_CHARSET;
		viewFont.lfWeight = SHOW_WEIGHT;
		viewFont.lfHeight = SHOW_HEIGHT;
		m_showFont.CreateFontIndirect(&viewFont);
	}
	SendDlgItemMessage(IDC_SHOW, WM_SETFONT, reinterpret_cast<WPARAM>(m_showFont.GetSafeHandle()), MAKELPARAM(TRUE, 0));

	// time() は1秒単位でしか変わらず、1秒以内に呼ぶと前回と同じ乱数列になるため、
	// 出題番号を掛けて毎回違う種にする
	srand(static_cast<unsigned>(time(nullptr)) * (m_count + 1) * 2);

	CString str;
	for (int i = 0; i < m_digits; i++)
	{
		const int val = rand();
		str += GetKeyGenChar(GetKeyGenType(val), val);
	}
	m_record[m_count] = str;
	m_count++;

	return str;
}

// 乱数を3で割った余りで、選択中の種類から優先順位を付けて文字の種類を決める
int CMemoryDlg::GetKeyGenType(int val) const
{
	switch (val % 3)
	{
	case 1:
		if (m_typeFlags & TYPE_ENG_SMALL)	return TYPE_ENG_SMALL;
		if (m_typeFlags & TYPE_ENG_LARGE)	return TYPE_ENG_LARGE;
		return TYPE_NUMBER;

	case 2:
		if (m_typeFlags & TYPE_ENG_LARGE)	return TYPE_ENG_LARGE;
		if (m_typeFlags & TYPE_NUMBER)		return TYPE_NUMBER;
		return TYPE_ENG_SMALL;

	default:
		if (m_typeFlags & TYPE_NUMBER)		return TYPE_NUMBER;
		if (m_typeFlags & TYPE_ENG_LARGE)	return TYPE_ENG_LARGE;
		return TYPE_ENG_SMALL;
	}
}

// 種類に応じた1文字を作る。直前と同じ文字にはしない
TCHAR CMemoryDlg::GetKeyGenChar(int strType, int val)
{
	int number = 0;

	switch (strType)
	{
	case TYPE_NUMBER:
		number = val % 10;
		if (number == m_prevChar)
		{
			number = MatchProc(val, number);
		}
		m_prevChar = number;
		return static_cast<TCHAR>(_T('0') + number);

	case TYPE_ENG_SMALL:
		number = _T('a') + (val % 26);
		if (number == m_prevChar)
		{
			number = MatchProc(val, number) + _T('a');
		}
		break;

	case TYPE_ENG_LARGE:
		number = _T('A') + (val % 26);
		if (number == m_prevChar)
		{
			number = MatchProc(val, number) + _T('A');
		}
		break;
	}

	m_prevChar = number;
	return static_cast<TCHAR>(number);
}

// orgVal の各桁を下から見て、current と違う最初の数字を返す（無ければ current のまま）
int CMemoryDlg::MatchProc(int orgVal, int current)
{
	while (orgVal != 0)
	{
		if (current != orgVal % 10)
		{
			return orgVal % 10;
		}
		orgVal /= 10;
	}
	return current;
}

void CMemoryDlg::OnTypeCheck(UINT nID)
{
	int setType = 0;
	switch (nID)
	{
	case IDC_ENG_SMALL:	setType = TYPE_ENG_SMALL;	break;
	case IDC_ENG_LARGE:	setType = TYPE_ENG_LARGE;	break;
	case IDC_NUMBER:	setType = TYPE_NUMBER;		break;
	default:			return;
	}

	SetType(setType, IsDlgButtonChecked(nID));
}

// スクロールバーで表示速度を 10ms 単位で変える
void CMemoryDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_timerCycle = m_cycleBar.GetScrollPos();

	int minPos, maxPos;
	m_cycleBar.GetScrollRange(&minPos, &maxPos);
	switch (nSBCode)
	{
	case SB_LINELEFT:
		m_timerCycle -= 10;
		break;
	case SB_LINERIGHT:
		m_timerCycle += 10;
		break;
	case SB_PAGELEFT:
		m_timerCycle -= 100;
		break;
	case SB_PAGERIGHT:
		m_timerCycle += 100;
		break;
	case SB_LEFT:
		m_timerCycle = minPos;
		break;
	case SB_RIGHT:
		m_timerCycle = maxPos;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_timerCycle = nPos;
		break;
	default:
		break;
	}

	if (m_timerCycle < MIN_CYC)
	{
		m_timerCycle = MIN_CYC;
	}
	else if (m_timerCycle > MAX_CYC)
	{
		m_timerCycle = MAX_CYC;
	}

	m_timerCycle = m_timerCycle / 10 * 10;
	m_cycleBar.SetScrollPos(m_timerCycle);

	CString str;
	str.Format(_T("(%d ms)"), m_timerCycle);
	GetDlgItem(IDC_SPEED_TXT)->SetWindowText(str);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMemoryDlg::OnHelp()
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

void CMemoryDlg::SetType(int setType, BOOL flg)
{
	if (flg == TRUE)
	{
		m_typeFlags |= setType;
	}
	else
	{
		m_typeFlags &= ~setType;
	}
}
