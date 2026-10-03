// back_up_dlg.h : メインダイアログ

#pragma once

// バックアップ設定 1 件分
struct BACKUP
{
	// オプションのビット（設定ファイルにこの値のまま保存される）
	static constexpr DWORD OPT_SUBDIR		= 0x1;	// サブディレクトリも対象
	static constexpr DWORD OPT_DIFF			= 0x2;	// 差分ファイルのみ対象
	static constexpr DWORD OPT_OVERWRITE	= 0x4;	// 上書きの確認を表示

	BOOL	bBkEnable	= TRUE;
	CString	strSrcPath;
	CString	strDstPath;
	DWORD	opt			= OPT_SUBDIR | OPT_DIFF;
};

class CBackUpDlg : public CDialog
{
public:
	CBackUpDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_BACKUP_DIALOG };

	static constexpr int MAX_ENTRY = 50;

private:
	// バックアップ後の動作（IDC_END_NONE からの並び順と一致させる）
	enum class EndAction
	{
		None,
		App,
		Reboot,
		Shutdown,
	};

	CListCtrl	m_listCtrl;
	int			m_nCurIdx = 0;		// 選択中の設定
	EndAction	m_endAction = EndAction::None;
	BACKUP		m_entries[MAX_ENTRY];
	HICON		m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
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
	afx_msg void OnEndOption(UINT nID);
	DECLARE_MESSAGE_MAP()

private:
	bool RunBackup(bool bWriteBatchOnly);
	void InitListCtrl();
	void InsertListColumn(LVCOLUMN lvCol, int nSubItem, LPCTSTR name);
	BOOL ReadSetting();
	BOOL WriteSetting();
	void WriteBatchFile(const CString& cmd);
	void SetOption(DWORD mask, bool bOn);
	void UpdateEnableBK(BOOL bChk);
	void UpdateSubDirectory(DWORD opt);
	void UpdateDiffFile(DWORD opt);
	void UpdateOverWrite(DWORD opt);
	void UpdateCheckItem(int nCtrlId, int nSubItem, bool bOn, LPCTSTR pszOn, LPCTSTR pszOff);
	void UpdatePath(int nEditId, int nSubItem, LPCTSTR path);
	void Refresh();
};
