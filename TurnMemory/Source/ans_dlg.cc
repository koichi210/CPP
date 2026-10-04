// ans_dlg.cc : 解答ダイアログ（答え合わせ）

#include "stdafx.h"
#include "turn_memory.h"
#include "turn_memory_dlg.h"
#include "ans_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AnsDlg::AnsDlg(const TurnMemoryDlg& game, CWnd* parent)
	: CDialog(IDD, parent)
	, game_(game)
{
}

BEGIN_MESSAGE_MAP(AnsDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL AnsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ShowCellGrid(*this, game_.GetSize());
	CheckProc();

	return TRUE;
}

// 正解の順番を表示し、入力と一致しないマスを覚えておく（赤字で表示する）
void AnsDlg::CheckProc()
{
	const int size = game_.GetSize();
	bool all_ok = true;

	for (int i = 0; i < size * size; i++)
	{
		judge_[i] = (game_.GetAnswer(i) == game_.GetInput(i));
		if (!judge_[i])
		{
			all_ok = false;
		}

		CString str;
		str.Format(_T("%d"), game_.GetAnswer(i));
		GetDlgItem(CellCtrlId(i / size, i % size))->SetWindowText(str);
	}

	GetDlgItem(IDC_TITLE)->SetWindowText(all_ok ? _T("全問正解！！") : _T("残念。。"));
}

HBRUSH AnsDlg::OnCtlColor(CDC* dc, CWnd* wnd, UINT ctl_color)
{
	HBRUSH hbr = CDialog::OnCtlColor(dc, wnd, ctl_color);

	const int id = wnd->GetDlgCtrlID();
	if (IDC_EDIT1 <= id && id <= IDC_EDIT100)
	{
		const int offset = id - IDC_EDIT1;
		const int index = offset / kCellMax * game_.GetSize() + offset % kCellMax;
		if (!judge_[index])
		{
			dc->SetTextColor(RGB(0xFF, 0, 0));	// 文字色は赤
		}
	}

	return hbr;
}
