// ans_dlg.h : 解答ダイアログ（答え合わせ）

#pragma once

class TurnMemoryDlg;

class AnsDlg : public CDialog
{
public:
	AnsDlg(const TurnMemoryDlg& game, CWnd* parent = nullptr);

	enum { IDD = IDD_ANS };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg HBRUSH OnCtlColor(CDC* dc, CWnd* wnd, UINT ctl_color);
	DECLARE_MESSAGE_MAP()

private:
	void CheckProc();

	const TurnMemoryDlg&	game_;
	bool					judge_[kCellMax * kCellMax] = {};	// 各マスが正解か
};
