// fname_exchange_dlg.h : メインダイアログ（選んだファイル/フォルダの名前を一括で変換する）

#pragma once

// 変換方法（ラジオボタンと対応）
enum class ConvertType
{
	kEnum,			// 通し番号付加
	kAllSbcs,		// すべて半角文字
	kAllDbcs,		// すべて全角文字
	kDeleteExt,		// 拡張子の削除
	kDeleteCount,	// 指定文字数の削除
	kAdd,			// 文字の追加
	kDelete,		// 文字の削除
	kReplace,		// 文字の置換
};

// 一覧に出す対象
enum class Target
{
	kFile,
	kFolder,
};

// 1件分の名前変更（復元用）
struct RenameRecord
{
	CString	old_path;
	CString	new_path;
};

class FnameExchangeDlg : public CDialog
{
public:
	explicit FnameExchangeDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_FNAME_EXCHANGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM param);
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
	afx_msg void OnConvertType(UINT id);
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
	CString MakeNewPath(const CString& old_path, int file_count);
	CString MakeEnumName(const CString& file, int file_count);
	CString MakeDeleteCountName(const CString& file);
	CString MakeAddName(const CString& file);
	CString BuildPath(const CString& name, const CString& ext) const;

	void RenameFile(const CString& old_path, const CString& new_path);
	void Undo();
	void ShowError(UINT message_id);

	HICON		icon_;
	CListBox	list_;
	Target		target_;
	ConvertType	type_;
	bool		ignore_alert_;		// 一般的な警告表示を無効にする
	bool		case_sensitive_;	// 大文字小文字を区別する

	// 実行時に画面から読む設定
	CString		dir_;
	UINT		first_number_;		//【通し番号付加】最初の値
	UINT		next_number_;		//【通し番号付加】次に付ける番号
	int			digits_;			//【通し番号付加】桁数（0 なら自動）
	bool		keep_name_;			//【通し番号付加】もとのファイル名を残す
	UINT		delete_head_;		//【指定文字数削除】先頭からの文字数
	UINT		delete_tail_;		//【指定文字数削除】後部からの文字数
	bool		add_before_;		//【文字の追加】先頭に追加
	bool		add_after_;			//【文字の追加】後部に追加
	CString		text1_;			// 削除する文字 / 置換前文字
	CString		text2_;			// 追加する文字 / 置換後文字

	std::vector<std::vector<RenameRecord>>	undo_steps_;	// 実行1回分ずつの復元情報
};
