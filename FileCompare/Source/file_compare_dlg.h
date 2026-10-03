// file_compare_dlg.h : メインダイアログ（指定フォルダ内の重複ファイルを探す）

#pragma once

#include <atomic>
#include <vector>

// 比較対象の1ファイル
struct FileEntry
{
	CString	path;
	int		group;		// 同じ内容のファイルの組番号（kNoGroup なら未発見）
};

class CFileCompareDlg : public CDialogEx
{
public:
	explicit CFileCompareDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_FILECOMPARE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedExecute();
	DECLARE_MESSAGE_MAP()

private:
	static UINT CompareThread(LPVOID pParam);

	void StartCompare();
	void StopCompare();
	std::vector<CString> GetFolders() const;				// パス欄を ; で区切ったフォルダ一覧
	void CollectFiles(const std::vector<CString>& folders);
	void CompareFiles();									// ワーカースレッドで実行する
	int GetProgressEnd() const;
	void UpdateProgress(int pos, int end);
	void ShowResult();

	HICON				m_hIcon;
	CProgressCtrl		m_progress;
	std::atomic<bool>	m_running;		// false にすると比較スレッドが途中で止まる
	std::vector<FileEntry>	m_files;	// 先頭 m_fileCount 個が今回の比較対象
	int					m_fileCount;
	int					m_nextGroup;	// 次に見つけた組に付ける番号
};
