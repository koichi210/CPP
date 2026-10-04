// main_dlg.h : メインダイアログ（マクロの実行・停止と設定画面の呼び出し）

#ifndef EVENTRECORDER_MACROTOOL_MAIN_DLG_H_
#define EVENTRECORDER_MACROTOOL_MAIN_DLG_H_

#include "macro_defs.h"

class MainDlg : public CDialog
{
public:
	explicit MainDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_MAINDLG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSetting();
	afx_msg void OnExec();
	afx_msg void OnStop();
	afx_msg void OnBnClickedClose();
	afx_msg LRESULT OnPlayFinished(WPARAM w_param, LPARAM l_param);
	DECLARE_MESSAGE_MAP()

private:
	std::vector<MacroEvent>	events_;				// 実行するイベント（未設定の行は後ろに詰めてある）
	UINT					repeat_count_;			// 全体の繰り返し回数
	UINT					repeat_delay_msec_;		// 1周ごとの待ち時間(ms)
	// 実行中の停止フラグ。false にすると実行スレッドが止まる。
	// 実行ごとに作り直すので、止めた直後に再実行しても前のスレッドは止まったままになる
	std::shared_ptr<std::atomic<bool>>	running_;
	UINT					run_id_ = 0;			// 実行ごとに増やす（終了通知が今の実行のものか見分ける）
	CString					version_;				// タイトルに出すバージョン
};

#endif  // EVENTRECORDER_MACROTOOL_MAIN_DLG_H_
