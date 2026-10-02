// DivisionCouplingDlg.h : メインダイアログ（ファイルの分割と結合）

#pragma once

#include <atomic>

class CDivisionCouplingDlg : public CDialogEx
{
public:
	explicit CDivisionCouplingDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_DIVISION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedSplit();
	afx_msg void OnBnClickedMerge();
	afx_msg void OnBnClickedSplitBrowse();
	afx_msg void OnBnClickedMergeBrowse();
	afx_msg void OnBnClickedStop();
	DECLARE_MESSAGE_MAP()

private:
	enum class Error
	{
		None,
		OpenSource,		// 元ファイルが開けない
		OpenDest,		// 出力ファイルが作れない
		Alloc,			// 作業メモリが確保できない
		DivSize,		// 分割サイズが不正
	};

	// 分割・結合はワーカースレッドで行う
	static UINT SplitThreadProc(LPVOID pParam);
	static UINT MergeThreadProc(LPVOID pParam);

	void StartProcess(AFX_THREADPROC pfnThreadProc);
	void FinishProcess();
	void Split();
	void Merge();
	void BrowseFile(UINT editId);
	void EnableControls(bool running);

	CProgressCtrl		m_progress;
	HICON				m_hIcon;
	int					m_divSize = 0;		// 分割サイズ(バイト)
	CString				m_srcPath;			// 分割元ファイル / 結合で選んだ分割ファイル
	CString				m_destPath;			// 出力中のファイル（エラー表示にも使う）
	Error				m_error = Error::None;
	std::atomic<bool>	m_running{ false };	// 「停止」でワーカースレッドに中断を伝える
};
