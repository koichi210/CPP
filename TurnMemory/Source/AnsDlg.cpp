// AnsDlg.cpp : 解答ダイアログ（答え合わせ）

#include "stdafx.h"
#include "TurnMemory.h"
#include "TurnMemoryDlg.h"
#include "AnsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAnsDlg::CAnsDlg(const CTurnMemoryDlg& game, CWnd* pParent)
	: CDialog(IDD, pParent)
	, m_game(game)
{
}

BEGIN_MESSAGE_MAP(CAnsDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CAnsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ShowCellGrid(*this, m_game.GetSize());
	CheckProc();

	return TRUE;
}

// 正解の順番を表示し、入力と一致しないマスを覚えておく（赤字で表示する）
void CAnsDlg::CheckProc()
{
	const int size = m_game.GetSize();
	bool bAllOk = true;

	for (int i = 0; i < size * size; i++)
	{
		m_judge[i] = (m_game.GetAnswer(i) == m_game.GetInput(i));
		if (!m_judge[i])
		{
			bAllOk = false;
		}

		CString str;
		str.Format(_T("%d"), m_game.GetAnswer(i));
		GetDlgItem(CellCtrlId(i / size, i % size))->SetWindowText(str);
	}

	GetDlgItem(IDC_TITLE)->SetWindowText(bAllOk ? _T("全問正解！！") : _T("残念。。"));
}

HBRUSH CAnsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	const int id = pWnd->GetDlgCtrlID();
	if (IDC_EDIT1 <= id && id <= IDC_EDIT100)
	{
		const int offset = id - IDC_EDIT1;
		const int index = offset / CELL_MAX * m_game.GetSize() + offset % CELL_MAX;
		if (!m_judge[index])
		{
			pDC->SetTextColor(RGB(0xFF0, 0, 0));	// 文字色は赤
		}
	}

	return hbr;
}
