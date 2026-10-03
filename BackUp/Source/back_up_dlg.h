// back_up_dlg.h : メインダイアログ

#ifndef BACKUP_SOURCE_BACK_UP_DLG_H_
#define BACKUP_SOURCE_BACK_UP_DLG_H_

// バックアップ設定 1 件分
struct BackupSetting
{
	// オプションのビット（設定ファイルにこの値のまま保存される）
	static constexpr DWORD kOptSubdir		= 0x1;	// サブディレクトリも対象
	static constexpr DWORD kOptDiff			= 0x2;	// 差分ファイルのみ対象
	static constexpr DWORD kOptOverwrite	= 0x4;	// 上書きの確認を表示

	BOOL	bk_enable	= TRUE;
	CString	src_path;
	CString	dst_path;
	DWORD	opt			= kOptSubdir | kOptDiff;
};

class BackUpDlg : public CDialog
{
public:
	BackUpDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_BACKUP_DIALOG };

	static constexpr int kMaxEntry = 50;

private:
	// バックアップ後の動作（IDC_END_NONE からの並び順と一致させる）
	enum class EndAction
	{
		kNone,
		kApp,
		kReboot,
		kShutdown,
	};

	CListCtrl	list_ctrl_;
	int			cur_index_ = 0;		// 選択中の設定
	EndAction	end_action_ = EndAction::kNone;
	BackupSetting		entries_[kMaxEntry];
	HICON		icon_;

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;

	afx_msg void OnSysCommand(UINT id, LPARAM l_param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLvnItemchangedList(NMHDR* nmhdr, LRESULT* result);
	afx_msg void OnBrowseSrc();
	afx_msg void OnBrowseDest();
	afx_msg void OnDiff();
	afx_msg void OnBackupStart();
	afx_msg void OnBnClickedSaveSetting();
	afx_msg void OnBnClickedSubdir();
	afx_msg void OnBnClickedOverWrite();
	afx_msg void OnBnClickedEnableBK();
	afx_msg void OnEnChangeEditSrc();
	afx_msg void OnEnChangeEditDst();
	afx_msg void OnBnClickedAllClear();
	afx_msg void OnEndOption(UINT id);
	DECLARE_MESSAGE_MAP()

private:
	bool RunBackup(bool write_batch_only);
	void InitListCtrl();
	void InsertListColumn(LVCOLUMN lv_col, int sub_item, LPCTSTR name);
	BOOL ReadSetting();
	BOOL WriteSetting();
	void WriteBatchFile(const CString& cmd);
	void SetOption(DWORD mask, bool is_on);
	void UpdateEnableBK(BOOL checked);
	void UpdateSubDirectory(DWORD opt);
	void UpdateDiffFile(DWORD opt);
	void UpdateOverWrite(DWORD opt);
	void UpdateCheckItem(int ctrl_id, int sub_item, bool is_on, LPCTSTR text_on, LPCTSTR text_off);
	void UpdatePath(int edit_id, int sub_item, LPCTSTR path);
	void Refresh();
};

#endif  // BACKUP_SOURCE_BACK_UP_DLG_H_
