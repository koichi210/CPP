// division_coupling_dlg.h : メインダイアログ（ファイルの分割と結合）

#pragma once

#include <atomic>

#include "worker_threads.h"

// ワーカーが分割・結合を終えたときに UI スレッドへ送る通知
constexpr UINT WM_APP_PROCESS_FINISHED = WM_APP + 1;

class CDivisionCouplingDlg : public CDialogEx
{
public:
	explicit CDivisionCouplingDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_DIVISION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
	virtual void OnCancel() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedSplit();
	afx_msg void OnBnClickedMerge();
	afx_msg void OnBnClickedSplitBrowse();
	afx_msg void OnBnClickedMergeBrowse();
	afx_msg void OnBnClickedStop();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg LRESULT OnProcessFinished(WPARAM wParam, LPARAM lParam);	// ワーカーの終了通知
	DECLARE_MESSAGE_MAP()

private:
	enum class Error
	{
		None,
		OpenSource,		// 元ファイルが開けない
		OpenDest,		// 出力ファイルが作れない
		Alloc,			// 作業メモリが確保できない
		DivSize,		// 分割サイズが不正
		Aborted,		// 「停止」または終了で中断した
	};

	// 分割・結合はワーカースレッドで行う
	static UINT SplitThreadProc(LPVOID pParam);
	static UINT MergeThreadProc(LPVOID pParam);

	void StartProcess(AFX_THREADPROC pfnThreadProc);
	void Split();
	void Merge();
	void BrowseFile(UINT editId);
	void EnableControls(bool running);
	bool StopWorkersForClose();		// 処理中なら確認のうえ中断し、終了を待つ（閉じてよければ true）
	void StopWorkers();				// 確認せずに中断し、終了を待つ

	CProgressCtrl		m_progress;
	HICON				m_hIcon;
	int					m_divSize = 0;		// 分割サイズ(バイト)
	CString				m_srcPath;			// 分割元ファイル / 結合で選んだ分割ファイル
	CString				m_destPath;			// 出力中のファイル（エラー表示にも使う）
	Error				m_error = Error::None;
	std::atomic<bool>	m_running{ false };	// 「停止」でワーカースレッドに中断を伝える
	std::atomic<bool>	m_closing{ false };	// 終了処理中（ワーカーはメッセージを出さない）
	WorkerThreads		m_workers;
};
