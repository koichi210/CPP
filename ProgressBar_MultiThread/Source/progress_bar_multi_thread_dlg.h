// progress_bar_multi_thread_dlg.h : メインダイアログ

#pragma once

#include <atomic>

#include "worker_threads.h"

class CProgressBarDlg : public CDialogEx
{
public:
	explicit CProgressBarDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_PROGRESSBAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
	virtual void OnCancel() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnEndSession(BOOL bEnding);
	DECLARE_MESSAGE_MAP()

private:
	// 進捗を進めるワーカースレッド。UI スレッドを塞がないので、処理中も Stop が押せる
	static UINT ProgressThread(LPVOID pParam);
	void StopWorkers();		// 閉じる前にワーカーを止め、終了を待つ

	HICON m_hIcon;
	CProgressCtrl m_progress;
	CString m_strStatus;
	std::atomic<bool> m_stopRequested{ false };	// UI スレッドとワーカースレッドで共有
	WorkerThreads m_workers;
};
