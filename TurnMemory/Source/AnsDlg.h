// AnsDlg.h : 解答ダイアログ（答え合わせ）

#pragma once

class CTurnMemoryDlg;

class CAnsDlg : public CDialog
{
public:
	CAnsDlg(const CTurnMemoryDlg& game, CWnd* pParent = nullptr);

	enum { IDD = IDD_ANS };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	void CheckProc();

	const CTurnMemoryDlg&	m_game;
	bool					m_judge[CELL_MAX * CELL_MAX] = {};	// 各マスが正解か
};
