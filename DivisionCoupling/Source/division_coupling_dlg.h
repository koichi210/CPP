// division_coupling_dlg.h : メインダイアログ（ファイルの分割と結合）

#ifndef DIVISIONCOUPLING_SOURCE_DIVISION_COUPLING_DLG_H_
#define DIVISIONCOUPLING_SOURCE_DIVISION_COUPLING_DLG_H_

#include <atomic>

#include "worker_threads.h"

// ワーカーが分割・結合を終えたときに UI スレッドへ送る通知
constexpr UINT kWmAppProcessFinished = WM_APP + 1;

class DivisionCouplingDlg : public CDialogEx
{
public:
	explicit DivisionCouplingDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_DIVISION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
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
	afx_msg void OnEndSession(BOOL ending);
	afx_msg LRESULT OnProcessFinished(WPARAM w_param, LPARAM l_param);	// ワーカーの終了通知
	DECLARE_MESSAGE_MAP()

private:
	enum class Error
	{
		kNone,
		kOpenSource,		// 元ファイルが開けない
		kOpenDest,		// 出力ファイルが作れない
		kAlloc,			// 作業メモリが確保できない
		kDivSize,		// 分割サイズが不正
		kAborted,		// 「停止」または終了で中断した
	};

	// 分割・結合はワーカースレッドで行う
	static UINT SplitThreadProc(LPVOID param);
	static UINT MergeThreadProc(LPVOID param);

	void StartProcess(AFX_THREADPROC thread_proc);
	void Split();
	void Merge();
	void BrowseFile(UINT edit_id);
	void EnableControls(bool running);
	bool StopWorkersForClose();		// 処理中なら確認のうえ中断し、終了を待つ（閉じてよければ true）
	void StopWorkers();				// 確認せずに中断し、終了を待つ

	CProgressCtrl		progress_;
	HICON				icon_;
	int					div_size_ = 0;		// 分割サイズ(バイト)
	CString				src_path_;			// 分割元ファイル / 結合で選んだ分割ファイル
	CString				dest_path_;			// 出力中のファイル（エラー表示にも使う）
	Error				error_ = Error::kNone;
	std::atomic<bool>	running_{ false };	// 「停止」でワーカースレッドに中断を伝える
	std::atomic<bool>	closing_{ false };	// 終了処理中（ワーカーはメッセージを出さない）
	WorkerThreads		workers_;
};

#endif  // DIVISIONCOUPLING_SOURCE_DIVISION_COUPLING_DLG_H_
