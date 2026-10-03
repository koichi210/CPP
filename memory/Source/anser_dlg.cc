// anser_dlg.cc : 解答ダイアログ

#include "stdafx.h"
#include "memory.h"
#include "memory_dlg.h"
#include "anser_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AnserDlg::AnserDlg(const MemoryDlg& game, CWnd* parent)
	: CDialog(IDD, parent)
	, game_(game)
{
}

BEGIN_MESSAGE_MAP(AnserDlg, CDialog)
	ON_BN_CLICKED(ID_ANS_CHK, &AnserDlg::OnAnserCheck)
	ON_BN_CLICKED(ID_ANSOK, &AnserDlg::OnAnsok)
	ON_BN_CLICKED(ID_ANS_SHOW, &AnserDlg::OnAnsShow)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL AnserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	cheat_ = FALSE;

	// 計算は合計値の1欄だけ、暗記は出題数ぶんの欄だけ入力できるようにする
	for (int i = 0; i < kPrNumMax; i++)
	{
		GetDlgItem(IDC_ANS_TEXT1 + i)->EnableWindow(FALSE);
		GetDlgItem(ANS_NO1 + i)->EnableWindow(FALSE);

		if (game_.GetPlayMode() == PlayMode::kKeisan)
		{
			GetDlgItem(IDC_ANS_TEXT1)->EnableWindow(TRUE);
			GetDlgItem(ANS_NO1)->EnableWindow(TRUE);
		}
		else if (i < game_.GetProblemCount())
		{
			GetDlgItem(IDC_ANS_TEXT1 + i)->EnableWindow(TRUE);
			GetDlgItem(ANS_NO1 + i)->EnableWindow(TRUE);
		}
	}
	return TRUE;
}

void AnserDlg::OnAnsok()
{
	CDialog::OnOK();
}

// 答えの表示・非表示を切り替える（一度でも見たら「答え閲覧済み」）
void AnserDlg::OnAnsShow()
{
	const int count = game_.GetProblemCount();
	const bool keisan = (game_.GetPlayMode() == PlayMode::kKeisan);

	cheat_ = !cheat_;

	if (cheat_)
	{
		long sum = 0;
		for (int i = 0; i < count; i++)
		{
			if (keisan)
			{
				sum += _ttol(game_.GetRecord(i));
			}
			else
			{
				GetDlgItem(IDC_ANSER1 + i)->SetWindowText(game_.GetRecord(i));
			}
		}

		if (keisan)
		{
			CString wk;
			wk.Format(_T("%ld"), sum);
			GetDlgItem(IDC_ANSER1)->SetWindowText(wk);
		}

		GetDlgItem(ID_ANS_SHOW)->SetWindowText(_T("答えを非表示"));
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			GetDlgItem(IDC_ANSER1 + i)->SetWindowText(_T(""));
		}
		GetDlgItem(ID_ANS_SHOW)->SetWindowText(_T("カンニング示"));
	}

	GetDlgItem(IDC_CHEAT)->SetWindowText(_T("答え閲覧済み"));
}

void AnserDlg::OnAnserCheck()
{
	const int count = game_.GetProblemCount();
	const bool keisan = (game_.GetPlayMode() == PlayMode::kKeisan);
	long sum = 0;

	for (int i = 0; i < count; i++)
	{
		if (keisan)
		{
			sum += _ttol(game_.GetRecord(i));
		}
		else
		{
			CString input;
			GetDlgItemText(IDC_ANS_TEXT1 + i, input);

			if (game_.GetRecord(i) == input)
			{
				GetDlgItem(IDC_JUDGE1 + i)->SetWindowText(_T("○"));
				GetDlgItem(IDC_ANSER1 + i)->SetWindowText(game_.GetRecord(i));
			}
			else
			{
				GetDlgItem(IDC_JUDGE1 + i)->SetWindowText(_T("×"));
			}
		}
	}

	// 計算は入力した合計値を判定する
	if (keisan)
	{
		CString input;
		GetDlgItemText(IDC_ANS_TEXT1, input);
		const long input_val = _ttol(input);

		if (sum == input_val)
		{
			CString wk;
			wk.Format(_T("%d"), input_val);
			GetDlgItem(IDC_JUDGE1)->SetWindowText(_T("○"));
			GetDlgItem(IDC_ANSER1)->SetWindowText(wk);
		}
		else
		{
			GetDlgItem(IDC_JUDGE1)->SetWindowText(_T("×"));
		}
	}
}

HBRUSH AnserDlg::OnCtlColor(CDC* dc, CWnd* wnd, UINT ctl_color)
{
	HBRUSH hbr = CDialog::OnCtlColor(dc, wnd, ctl_color);

	if (wnd->GetDlgCtrlID() == IDC_CHEAT)
	{
		dc->SetTextColor(RGB(0xFF0, 0, 0));	// 文字色は赤
	}

	return hbr;
}
