// ProgressBar_MultiThreadDlg.h : メインダイアログ

#pragma once

#include <atomic>

class CProgressBarDlg : public CDialogEx
{
public:
	explicit CProgressBarDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_PROGRESSBAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	DECLARE_MESSAGE_MAP()

private:
	// 進捗を進めるワーカースレッド。UI スレッドを塞がないので、処理中も Stop が押せる
	static UINT ProgressThread(LPVOID pParam);

	HICON m_hIcon;
	CProgressCtrl m_progress;
	CString m_strStatus;
	std::atomic<bool> m_stopRequested{ false };	// UI スレッドとワーカースレッドで共有
};
