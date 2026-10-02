// AnserDlg.cpp : 解答ダイアログ

#include "stdafx.h"
#include "memory.h"
#include "memoryDlg.h"
#include "AnserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAnserDlg::CAnserDlg(const CMemoryDlg& game, CWnd* pParent)
	: CDialog(IDD, pParent)
	, m_game(game)
{
}

BEGIN_MESSAGE_MAP(CAnserDlg, CDialog)
	ON_BN_CLICKED(ID_ANS_CHK, &CAnserDlg::OnAnserCheck)
	ON_BN_CLICKED(ID_ANSOK, &CAnserDlg::OnAnsok)
	ON_BN_CLICKED(ID_ANS_SHOW, &CAnserDlg::OnAnsShow)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CAnserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bCheat = FALSE;

	// 計算は合計値の1欄だけ、暗記は出題数ぶんの欄だけ入力できるようにする
	for (int i = 0; i < PR_NUM_MAX; i++)
	{
		GetDlgItem(IDC_ANS_TEXT1 + i)->EnableWindow(FALSE);
		GetDlgItem(ANS_NO1 + i)->EnableWindow(FALSE);

		if (m_game.GetPlayMode() == PlayMode::Keisan)
		{
			GetDlgItem(IDC_ANS_TEXT1)->EnableWindow(TRUE);
			GetDlgItem(ANS_NO1)->EnableWindow(TRUE);
		}
		else if (i < m_game.GetProblemCount())
		{
			GetDlgItem(IDC_ANS_TEXT1 + i)->EnableWindow(TRUE);
			GetDlgItem(ANS_NO1 + i)->EnableWindow(TRUE);
		}
	}
	return TRUE;
}

void CAnserDlg::OnAnsok()
{
	CDialog::OnOK();
}

// 答えの表示・非表示を切り替える（一度でも見たら「答え閲覧済み」）
void CAnserDlg::OnAnsShow()
{
	const int count = m_game.GetProblemCount();
	const bool bKeisan = (m_game.GetPlayMode() == PlayMode::Keisan);

	m_bCheat = !m_bCheat;

	if (m_bCheat)
	{
		long sum = 0;
		for (int i = 0; i < count; i++)
		{
			if (bKeisan)
			{
				sum += _ttol(m_game.GetRecord(i));
			}
			else
			{
				GetDlgItem(IDC_ANSER1 + i)->SetWindowText(m_game.GetRecord(i));
			}
		}

		if (bKeisan)
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

void CAnserDlg::OnAnserCheck()
{
	const int count = m_game.GetProblemCount();
	const bool bKeisan = (m_game.GetPlayMode() == PlayMode::Keisan);
	long sum = 0;

	for (int i = 0; i < count; i++)
	{
		if (bKeisan)
		{
			sum += _ttol(m_game.GetRecord(i));
		}
		else
		{
			CString input;
			GetDlgItemText(IDC_ANS_TEXT1 + i, input);

			if (m_game.GetRecord(i) == input)
			{
				GetDlgItem(IDC_JUDGE1 + i)->SetWindowText(_T("○"));
				GetDlgItem(IDC_ANSER1 + i)->SetWindowText(m_game.GetRecord(i));
			}
			else
			{
				GetDlgItem(IDC_JUDGE1 + i)->SetWindowText(_T("×"));
			}
		}
	}

	// 計算は入力した合計値を判定する
	if (bKeisan)
	{
		CString input;
		GetDlgItemText(IDC_ANS_TEXT1, input);
		const long inputVal = _ttol(input);

		if (sum == inputVal)
		{
			CString wk;
			wk.Format(_T("%d"), inputVal);
			GetDlgItem(IDC_JUDGE1)->SetWindowText(_T("○"));
			GetDlgItem(IDC_ANSER1)->SetWindowText(wk);
		}
		else
		{
			GetDlgItem(IDC_JUDGE1)->SetWindowText(_T("×"));
		}
	}
}

HBRUSH CAnserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_CHEAT)
	{
		pDC->SetTextColor(RGB(0xFF0, 0, 0));	// 文字色は赤
	}

	return hbr;
}
