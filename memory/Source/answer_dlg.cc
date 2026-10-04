// answer_dlg.cc : 解答ダイアログ

#include "stdafx.h"
#include "memory.h"
#include "memory_dlg.h"
#include "answer_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AnswerDlg::AnswerDlg(const MemoryDlg& game, CWnd* parent)
	: CDialog(IDD, parent)
	, game_(game)
{
}

BEGIN_MESSAGE_MAP(AnswerDlg, CDialog)
	ON_BN_CLICKED(ID_ANS_CHK, &AnswerDlg::OnAnswerCheck)
	ON_BN_CLICKED(ID_ANSOK, &AnswerDlg::OnAnsok)
	ON_BN_CLICKED(ID_ANS_SHOW, &AnswerDlg::OnAnsShow)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL AnswerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 計算は合計値の1欄だけ、暗記は出題数ぶんの欄だけ入力できるようにする
	const int input_count = (game_.GetPlayMode() == PlayMode::kKeisan) ? 1 : game_.GetProblemCount();
	for (int i = 0; i < kPrNumMax; i++)
	{
		const BOOL enable = (i < input_count) ? TRUE : FALSE;
		GetDlgItem(IDC_ANS_TEXT1 + i)->EnableWindow(enable);
		GetDlgItem(ANS_NO1 + i)->EnableWindow(enable);
	}
	return TRUE;
}

// 計算モードの正解（出題した数の合計）
long AnswerDlg::SumRecords() const
{
	long sum = 0;
	for (int i = 0; i < game_.GetProblemCount(); i++)
	{
		sum += _ttol(game_.GetRecord(i));
	}
	return sum;
}

void AnswerDlg::OnAnsok()
{
	CDialog::OnOK();
}

// 答えの表示・非表示を切り替える（一度でも見たら「答え閲覧済み」）
void AnswerDlg::OnAnsShow()
{
	const int count = game_.GetProblemCount();

	cheat_ = !cheat_;

	if (cheat_)
	{
		if (game_.GetPlayMode() == PlayMode::kKeisan)
		{
			CString wk;
			wk.Format(_T("%ld"), SumRecords());
			GetDlgItem(IDC_ANSER1)->SetWindowText(wk);
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				GetDlgItem(IDC_ANSER1 + i)->SetWindowText(game_.GetRecord(i));
			}
		}

		GetDlgItem(ID_ANS_SHOW)->SetWindowText(_T("答えを非表示"));
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			GetDlgItem(IDC_ANSER1 + i)->SetWindowText(_T(""));
		}
		GetDlgItem(ID_ANS_SHOW)->SetWindowText(_T("カンニング"));
	}

	GetDlgItem(IDC_CHEAT)->SetWindowText(_T("答え閲覧済み"));
}

void AnswerDlg::OnAnswerCheck()
{
	// 計算は入力した合計値を判定する
	if (game_.GetPlayMode() == PlayMode::kKeisan)
	{
		CString input;
		GetDlgItemText(IDC_ANS_TEXT1, input);
		const long input_val = _ttol(input);

		if (SumRecords() == input_val)
		{
			CString wk;
			wk.Format(_T("%ld"), input_val);
			GetDlgItem(IDC_JUDGE1)->SetWindowText(_T("○"));
			GetDlgItem(IDC_ANSER1)->SetWindowText(wk);
		}
		else
		{
			GetDlgItem(IDC_JUDGE1)->SetWindowText(_T("×"));
		}
		return;
	}

	// 暗記は1問ずつ判定する
	for (int i = 0; i < game_.GetProblemCount(); i++)
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

HBRUSH AnswerDlg::OnCtlColor(CDC* dc, CWnd* wnd, UINT ctl_color)
{
	HBRUSH hbr = CDialog::OnCtlColor(dc, wnd, ctl_color);

	if (wnd->GetDlgCtrlID() == IDC_CHEAT)
	{
		dc->SetTextColor(RGB(0xFF, 0, 0));	// 文字色は赤
	}

	return hbr;
}
