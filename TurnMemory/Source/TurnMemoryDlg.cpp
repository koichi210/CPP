// TurnMemoryDlg.cpp : メインダイアログ（出題）

#include "stdafx.h"
#include "TurnMemory.h"
#include "TurnMemoryDlg.h"
#include "AnsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int DEF_PRB = 4;			// 1辺のマス数の初期値

	constexpr UINT_PTR EVENT_SHOW = 1;	// 順番を表示するタイマ
	constexpr UINT_PTR EVENT_WAIT = 2;	// 記憶時間のタイマ

	constexpr UINT SHOW_INVAL = 1000;
	constexpr UINT WAIT_INVAL = 1000;
	constexpr int WAIT_MAX = 5;

	constexpr LPCTSTR KEEP_MIND_STR		= _T("順番を記憶して下さい。");
	constexpr LPCTSTR CNT_REMEMBER_STR	= _T("秒間覚えて下さい。");
	constexpr LPCTSTR START_STR			= _T("スタートを押して下さい。");
	constexpr LPCTSTR END_STR			= _T("解答を入力して下さい。");
	constexpr LPCTSTR START_BUTTON		= _T("スタート");
	constexpr LPCTSTR STOP_BUTTON		= _T("ストップ");
}

void ShowCellGrid(CWnd& dlg, int size)
{
	for (int i = 0; i < CELL_MAX; i++)
	{
		for (int j = 0; j < CELL_MAX; j++)
		{
			const bool bShow = (i < size) && (j < size);
			dlg.GetDlgItem(CellCtrlId(i, j))->ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
		}

		const int nShowEdge = (i < size) ? SW_SHOWNORMAL : SW_HIDE;
		dlg.GetDlgItem(IDC_LINE1 + i)->ShowWindow(nShowEdge);
		dlg.GetDlgItem(IDC_ROW1 + i)->ShowWindow(nShowEdge);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryDlg

CTurnMemoryDlg::CTurnMemoryDlg(CWnd* pParent)
	: CDialog(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CTurnMemoryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CTurnMemoryDlg::OnStart)
	ON_BN_CLICKED(IDC_ANS, &CTurnMemoryDlg::OnAns)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CTurnMemoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	GetDlgItem(IDC_TITLE)->SetWindowText(START_STR);
	srand(static_cast<unsigned>(time(nullptr)));

	// マス数の選択肢
	SendDlgItemMessage(IDC_PROBLEM, CB_RESETCONTENT, 0, 0L);
	for (int i = CELL_MIN; i <= CELL_MAX; i++)
	{
		CString str;
		str.Format(_T("%d × %d"), i, i);
		SendDlgItemMessage(IDC_PROBLEM, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.GetString()));
	}

	m_size = DEF_PRB;
	m_state = GameState::Init;
	m_cnt = 1;
	SendDlgItemMessage(IDC_PROBLEM, CB_SETCURSEL, m_size - CELL_MIN, 0L);
	Refresh();

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログがメインウィンドウのため自前で描く）
void CTurnMemoryDlg::OnPaint()
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

HCURSOR CTurnMemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTurnMemoryDlg::OnStart()
{
	if (m_state == GameState::Start)
	{
		m_state = GameState::Stop;
		InitProc();
		ExitProc();
		return;
	}

	memset(m_input, 0, sizeof(m_input));
	m_state = GameState::Start;
	GetDlgItem(IDC_START)->SetWindowText(STOP_BUTTON);
	GetDlgItem(IDC_TITLE)->SetWindowText(KEEP_MIND_STR);
	GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	GetDlgItem(IDC_PROBLEM)->EnableWindow(FALSE);

	Refresh();
	BuildNumber();
	SetTimer(EVENT_SHOW, SHOW_INVAL, nullptr);
}

void CTurnMemoryDlg::OnAns()
{
	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			m_input[i * m_size + j] = GetDlgItemInt(CellCtrlId(i, j), nullptr, FALSE);
		}
	}

	CAnsDlg dlg(*this, this);
	dlg.DoModal();

	GetDlgItem(IDC_TITLE)->SetWindowText(START_STR);
}

void CTurnMemoryDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == EVENT_SHOW)
	{
		ShowProc();
	}
	else if (nIDEvent == EVENT_WAIT)
	{
		WaitProc();
	}
	CDialog::OnTimer(nIDEvent);
}

// 全部の順番を表示し終えたら、記憶時間のカウントダウンに移る
void CTurnMemoryDlg::EndProc()
{
	m_cnt = 1;
	m_wait = WAIT_MAX;
	KillTimer(EVENT_SHOW);
	SetTimer(EVENT_WAIT, WAIT_INVAL, nullptr);
}

void CTurnMemoryDlg::ExitProc()
{
	m_state = GameState::End;
	m_cnt = 1;
	memset(m_answer, 0, sizeof(m_answer));
	GetDlgItem(IDC_TITLE)->SetWindowText(START_STR);
	GetDlgItem(IDC_START)->SetWindowText(START_BUTTON);
	KillTimer(EVENT_SHOW);
	KillTimer(EVENT_WAIT);
}

// 解答の入力を受け付ける状態にする
void CTurnMemoryDlg::InitProc()
{
	GetDlgItem(IDC_START)->SetWindowText(START_BUTTON);
	GetDlgItem(IDC_TITLE)->SetWindowText(END_STR);
	GetDlgItem(IDC_PROBLEM)->EnableWindow(TRUE);
	if (m_state == GameState::End && m_input[0] == 0)
	{
		GetDlgItem(IDC_ANS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	}

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			CWnd* pCell = GetDlgItem(CellCtrlId(i, j));
			pCell->EnableWindow(TRUE);
			pCell->SetWindowText(_T(""));
		}
	}
}

// 各マスに 1～マス数 の順番をランダムに割り当てる
void CTurnMemoryDlg::BuildNumber()
{
	const int cellCount = CellCount();
	int randtbl[CELL_MAX * CELL_MAX];

	memset(m_answer, 0, sizeof(m_answer));
	for (int i = 0; i < cellCount; i++)
	{
		randtbl[i] = rand();
	}

	// 乱数の大きいマスから大きい番号を振る。振ったマスは -1 にして次からは選ばない
	int number = cellCount;
	for (int i = 0; i < cellCount; i++)
	{
		int max = -1;
		int idx = 0;
		for (int j = 0; j < cellCount; j++)
		{
			if (randtbl[j] > max)
			{
				max = randtbl[j];
				idx = j;
			}
		}
		m_answer[idx] = number--;
		randtbl[idx] = -1;
	}
}

// 選択中のマス数に合わせてマスを表示し直す
void CTurnMemoryDlg::Refresh()
{
	m_size = static_cast<int>(SendDlgItemMessage(IDC_PROBLEM, CB_GETCURSEL, 0, 0)) + CELL_MIN;

	ShowCellGrid(*this, m_size);

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			CWnd* pCell = GetDlgItem(CellCtrlId(i, j));
			pCell->EnableWindow(FALSE);
			pCell->SetWindowText(_T(""));
		}
	}
}

// 次の順番のマスに番号を表示する
void CTurnMemoryDlg::ShowProc()
{
	for (int i = 0; i < CellCount(); i++)
	{
		if (m_answer[i] == m_cnt)
		{
			CString str;
			str.Format(_T("%d"), m_cnt);
			GetDlgItem(CellCtrlId(i / m_size, i % m_size))->SetWindowText(str);
			break;
		}
	}

	m_cnt++;
	if (m_cnt > CellCount())
	{
		EndProc();
	}
}

void CTurnMemoryDlg::WaitProc()
{
	CString str;
	str.Format(_T("%d%s"), m_wait, CNT_REMEMBER_STR);
	GetDlgItem(IDC_TITLE)->SetWindowText(str);

	m_wait--;
	if (m_wait < 0)
	{
		m_state = GameState::End;
		InitProc();
		m_wait = WAIT_MAX;
		KillTimer(EVENT_WAIT);
	}
}
