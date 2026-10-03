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

class FileCompareDlg : public CDialogEx
{
public:
	explicit FileCompareDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_FILECOMPARE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedExecute();
	DECLARE_MESSAGE_MAP()

private:
	static UINT CompareThread(LPVOID param);

	void StartCompare();
	void StopCompare();
	std::vector<CString> GetFolders() const;				// パス欄を ; で区切ったフォルダ一覧
	void CollectFiles(const std::vector<CString>& folders);
	void CompareFiles();									// ワーカースレッドで実行する
	int GetProgressEnd() const;
	void UpdateProgress(int pos, int end);
	void ShowResult();

	HICON				icon_;
	CProgressCtrl		progress_;
	std::atomic<bool>	running_;		// false にすると比較スレッドが途中で止まる
	std::vector<FileEntry>	files_;	// 先頭 file_count_ 個が今回の比較対象
	int					file_count_;
	int					next_group_;	// 次に見つけた組に付ける番号
};
