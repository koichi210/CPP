// fname_exchangeDlg.h : メインダイアログ（選んだファイル/フォルダの名前を一括で変換する）

#pragma once

// 変換方法（ラジオボタンと対応）
enum class ConvertType
{
	Enum,			// 通し番号付加
	AllSbcs,		// すべて半角文字
	AllDbcs,		// すべて全角文字
	DeleteExt,		// 拡張子の削除
	DeleteCount,	// 指定文字数の削除
	Add,			// 文字の追加
	Delete,			// 文字の削除
	Replace,		// 文字の置換
};

// 一覧に出す対象
enum class Target
{
	File,
	Folder,
};

// 1件分の名前変更（復元用）
struct RenameRecord
{
	CString	oldPath;
	CString	newPath;
};

class CFnameExchangeDlg : public CDialog
{
public:
	explicit CFnameExchangeDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_FNAME_EXCHANGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnGetFile();
	afx_msg void OnTargetFile();
	afx_msg void OnTargetFolder();
	afx_msg void OnAllCheck();
	afx_msg void OnAllUncheck();
	afx_msg void OnIgnoreAlert();
	afx_msg void OnCaseSensitive();
	afx_msg void OnConvertType(UINT nID);
	afx_msg void OnExecute();
	afx_msg void OnUndo();
	afx_msg void OnEnd();
	DECLARE_MESSAGE_MAP()

private:
	void InitDigitsCombo();
	void UpdateControls();						// 変換方法に合わせて入力欄の有効/無効と見出しを切り替える
	void EnableItem(int id, bool enable);
	void ReadSettings();
	std::vector<CString> GetSelectedNames();

	// 変換後のフルパスを作る。入力エラーなどで作れなければ空文字列
	CString MakeNewPath(const CString& oldPath, int fileCount);
	CString MakeEnumName(const CString& file, int fileCount);
	CString MakeDeleteCountName(const CString& file);
	CString MakeAddName(const CString& file);
	CString BuildPath(const CString& name, const CString& ext) const;

	void RenameFile(const CString& oldPath, const CString& newPath);
	void Undo();
	void ShowError(UINT messageId);

	HICON		m_hIcon;
	CListBox	m_list;
	Target		m_target;
	ConvertType	m_type;
	bool		m_ignoreAlert;		// 一般的な警告表示を無効にする
	bool		m_caseSensitive;	// 大文字小文字を区別する

	// 実行時に画面から読む設定
	CString		m_dir;
	UINT		m_firstNumber;		//【通し番号付加】最初の値
	UINT		m_nextNumber;		//【通し番号付加】次に付ける番号
	int			m_digits;			//【通し番号付加】桁数（0 なら自動）
	bool		m_keepName;			//【通し番号付加】もとのファイル名を残す
	UINT		m_deleteHead;		//【指定文字数削除】先頭からの文字数
	UINT		m_deleteTail;		//【指定文字数削除】後部からの文字数
	bool		m_addBefore;		//【文字の追加】先頭に追加
	bool		m_addAfter;			//【文字の追加】後部に追加
	CString		m_text1;			// 削除する文字 / 置換前文字
	CString		m_text2;			// 追加する文字 / 置換後文字

	std::vector<std::vector<RenameRecord>>	m_undoSteps;	// 実行1回分ずつの復元情報
};
