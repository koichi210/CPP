// multi_thread_dlg.h : メインダイアログ（ワーカースレッドからタイトルを更新する）

#pragma once

#include "worker_threads.h"

class MultiThreadDlg : public CDialogEx
{
public:
	explicit MultiThreadDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_MULTITHREAD_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
	virtual void OnCancel() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnEndSession(BOOL ending);
	DECLARE_MESSAGE_MAP()

private:
	static UINT CountThreadProc(LPVOID param);
	void StopWorkers();		// 閉じる前にワーカーを止め、終了を待つ

	HICON icon_;
	// UI スレッドが書き、ワーカースレッドが読むので atomic にする
	std::atomic<bool> stop_{ false };
	WorkerThreads workers_;
};
