// turn_memory_dlg.cc : メインダイアログ（出題）

#include "stdafx.h"
#include "turn_memory.h"
#include "turn_memory_dlg.h"
#include "ans_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	constexpr int kDefPrb = 4;			// 1辺のマス数の初期値

	constexpr UINT_PTR kEventShow = 1;	// 順番を表示するタイマ
	constexpr UINT_PTR kEventWait = 2;	// 記憶時間のタイマ

	constexpr UINT kShowInval = 1000;
	constexpr UINT kWaitInval = 1000;
	constexpr int kWaitMax = 5;

	constexpr LPCTSTR kKeepMindStr		= _T("順番を記憶して下さい。");
	constexpr LPCTSTR kCntRememberStr	= _T("秒間覚えて下さい。");
	constexpr LPCTSTR kStartStr			= _T("スタートを押して下さい。");
	constexpr LPCTSTR kEndStr			= _T("解答を入力して下さい。");
	constexpr LPCTSTR kStartButton		= _T("スタート");
	constexpr LPCTSTR kStopButton		= _T("ストップ");
}

void ShowCellGrid(CWnd& dlg, int size)
{
	for (int i = 0; i < kCellMax; i++)
	{
		for (int j = 0; j < kCellMax; j++)
		{
			const bool show = (i < size) && (j < size);
			dlg.GetDlgItem(CellCtrlId(i, j))->ShowWindow(show ? SW_SHOWNORMAL : SW_HIDE);
		}

		const int show_edge = (i < size) ? SW_SHOWNORMAL : SW_HIDE;
		dlg.GetDlgItem(IDC_LINE1 + i)->ShowWindow(show_edge);
		dlg.GetDlgItem(IDC_ROW1 + i)->ShowWindow(show_edge);
	}
}

/////////////////////////////////////////////////////////////////////////////
// TurnMemoryDlg

TurnMemoryDlg::TurnMemoryDlg(CWnd* parent)
	: CDialog(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(TurnMemoryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &TurnMemoryDlg::OnStart)
	ON_BN_CLICKED(IDC_ANS, &TurnMemoryDlg::OnAns)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL TurnMemoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	GetDlgItem(IDC_TITLE)->SetWindowText(kStartStr);
	srand(static_cast<unsigned>(time(nullptr)));

	// マス数の選択肢
	SendDlgItemMessage(IDC_PROBLEM, CB_RESETCONTENT, 0, 0L);
	for (int i = kCellMin; i <= kCellMax; i++)
	{
		CString str;
		str.Format(_T("%d × %d"), i, i);
		SendDlgItemMessage(IDC_PROBLEM, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.GetString()));
	}

	size_ = kDefPrb;
	state_ = GameState::kInit;
	cnt_ = 1;
	SendDlgItemMessage(IDC_PROBLEM, CB_SETCURSEL, size_ - kCellMin, 0L);
	Refresh();

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログがメインウィンドウのため自前で描く）
void TurnMemoryDlg::OnPaint()
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

HCURSOR TurnMemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void TurnMemoryDlg::OnStart()
{
	if (state_ == GameState::kStart)
	{
		state_ = GameState::kStop;
		InitProc();
		ExitProc();
		return;
	}

	memset(input_, 0, sizeof(input_));
	state_ = GameState::kStart;
	GetDlgItem(IDC_START)->SetWindowText(kStopButton);
	GetDlgItem(IDC_TITLE)->SetWindowText(kKeepMindStr);
	GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	GetDlgItem(IDC_PROBLEM)->EnableWindow(FALSE);

	Refresh();
	BuildNumber();
	SetTimer(kEventShow, kShowInval, nullptr);
}

void TurnMemoryDlg::OnAns()
{
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			input_[i * size_ + j] = GetDlgItemInt(CellCtrlId(i, j), nullptr, FALSE);
		}
	}

	AnsDlg dlg(*this, this);
	dlg.DoModal();

	GetDlgItem(IDC_TITLE)->SetWindowText(kStartStr);
}

void TurnMemoryDlg::OnTimer(UINT_PTR id_event)
{
	if (id_event == kEventShow)
	{
		ShowProc();
	}
	else if (id_event == kEventWait)
	{
		WaitProc();
	}
	CDialog::OnTimer(id_event);
}

// 全部の順番を表示し終えたら、記憶時間のカウントダウンに移る
void TurnMemoryDlg::EndProc()
{
	cnt_ = 1;
	wait_ = kWaitMax;
	KillTimer(kEventShow);
	SetTimer(kEventWait, kWaitInval, nullptr);
}

void TurnMemoryDlg::ExitProc()
{
	state_ = GameState::kEnd;
	cnt_ = 1;
	memset(answer_, 0, sizeof(answer_));
	GetDlgItem(IDC_TITLE)->SetWindowText(kStartStr);
	GetDlgItem(IDC_START)->SetWindowText(kStartButton);
	KillTimer(kEventShow);
	KillTimer(kEventWait);
}

// 解答の入力を受け付ける状態にする
void TurnMemoryDlg::InitProc()
{
	GetDlgItem(IDC_START)->SetWindowText(kStartButton);
	GetDlgItem(IDC_TITLE)->SetWindowText(kEndStr);
	GetDlgItem(IDC_PROBLEM)->EnableWindow(TRUE);
	if (state_ == GameState::kEnd && input_[0] == 0)
	{
		GetDlgItem(IDC_ANS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ANS)->EnableWindow(FALSE);
	}

	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			CWnd* cell = GetDlgItem(CellCtrlId(i, j));
			cell->EnableWindow(TRUE);
			cell->SetWindowText(_T(""));
		}
	}
}

// 各マスに 1～マス数 の順番をランダムに割り当てる
void TurnMemoryDlg::BuildNumber()
{
	const int cell_count = CellCount();
	int randtbl[kCellMax * kCellMax];

	memset(answer_, 0, sizeof(answer_));
	for (int i = 0; i < cell_count; i++)
	{
		randtbl[i] = rand();
	}

	// 乱数の大きいマスから大きい番号を振る。振ったマスは -1 にして次からは選ばない
	int number = cell_count;
	for (int i = 0; i < cell_count; i++)
	{
		int max = -1;
		int idx = 0;
		for (int j = 0; j < cell_count; j++)
		{
			if (randtbl[j] > max)
			{
				max = randtbl[j];
				idx = j;
			}
		}
		answer_[idx] = number--;
		randtbl[idx] = -1;
	}
}

// 選択中のマス数に合わせてマスを表示し直す
void TurnMemoryDlg::Refresh()
{
	size_ = static_cast<int>(SendDlgItemMessage(IDC_PROBLEM, CB_GETCURSEL, 0, 0)) + kCellMin;

	ShowCellGrid(*this, size_);

	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			CWnd* cell = GetDlgItem(CellCtrlId(i, j));
			cell->EnableWindow(FALSE);
			cell->SetWindowText(_T(""));
		}
	}
}

// 次の順番のマスに番号を表示する
void TurnMemoryDlg::ShowProc()
{
	for (int i = 0; i < CellCount(); i++)
	{
		if (answer_[i] == cnt_)
		{
			CString str;
			str.Format(_T("%d"), cnt_);
			GetDlgItem(CellCtrlId(i / size_, i % size_))->SetWindowText(str);
			break;
		}
	}

	cnt_++;
	if (cnt_ > CellCount())
	{
		EndProc();
	}
}

void TurnMemoryDlg::WaitProc()
{
	CString str;
	str.Format(_T("%d%s"), wait_, kCntRememberStr);
	GetDlgItem(IDC_TITLE)->SetWindowText(str);

	wait_--;
	if (wait_ < 0)
	{
		state_ = GameState::kEnd;
		InitProc();
		wait_ = kWaitMax;
		KillTimer(kEventWait);
	}
}
