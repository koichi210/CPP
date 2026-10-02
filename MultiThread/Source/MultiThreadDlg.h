// MultiThreadDlg.h : メインダイアログ（ワーカースレッドからタイトルを更新する）

#pragma once

#include "WorkerThreads.h"

class CMultiThreadDlg : public CDialogEx
{
public:
	explicit CMultiThreadDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_MULTITHREAD_DIALOG };

protected:
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
	static UINT CountThreadProc(LPVOID pParam);
	void StopWorkers();		// 閉じる前にワーカーを止め、終了を待つ

	HICON m_hIcon;
	// UI スレッドが書き、ワーカースレッドが読むので atomic にする
	std::atomic<bool> m_bStop{ false };
	CWorkerThreads m_workers;
};
