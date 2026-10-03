// progress_bar_multi_thread_dlg.h : メインダイアログ

#pragma once

#include <atomic>

#include "worker_threads.h"

class ProgressBarDlg : public CDialogEx
{
public:
	explicit ProgressBarDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_PROGRESSBAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
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
	// 進捗を進めるワーカースレッド。UI スレッドを塞がないので、処理中も Stop が押せる
	static UINT ProgressThread(LPVOID param);
	void StopWorkers();		// 閉じる前にワーカーを止め、終了を待つ

	HICON icon_;
	CProgressCtrl progress_;
	CString status_;
	std::atomic<bool> stop_requested_{ false };	// UI スレッドとワーカースレッドで共有
	WorkerThreads workers_;
};
