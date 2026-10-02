// MultiThreadDlg.h : メインダイアログ（ワーカースレッドからタイトルを更新する）

#pragma once

class CMultiThreadDlg : public CDialogEx
{
public:
	explicit CMultiThreadDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_MULTITHREAD_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	DECLARE_MESSAGE_MAP()

private:
	static UINT CountThreadProc(LPVOID pParam);

	HICON m_hIcon;
	// UI スレッドが書き、ワーカースレッドが読むので atomic にする
	std::atomic<bool> m_bStop{ false };
};
