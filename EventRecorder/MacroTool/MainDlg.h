// MainDlg.h : メインダイアログ（マクロの実行・停止と設定画面の呼び出し）

#pragma once

#include "MacroDefs.h"

class CMainDlg : public CDialog
{
public:
	explicit CMainDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_MAINDLG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSetting();
	afx_msg void OnExec();
	afx_msg void OnStop();
	afx_msg void OnBnClickedClose();
	afx_msg LRESULT OnPlayFinished(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	std::vector<MACROEVENT>	m_events;				// 実行するイベント（未設定の行は後ろに詰めてある）
	UINT					m_repeatCount;			// 全体の繰り返し回数
	UINT					m_repeatDelayMsec;		// 1周ごとの待ち時間(ms)
	std::atomic<bool>		m_running{ false };		// 実行中。false にすると実行スレッドが止まる
	CString					m_version;				// タイトルに出すバージョン
};
