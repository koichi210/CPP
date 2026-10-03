// memory_dlg.h : メインダイアログ（出題）

#ifndef MEMORY_SOURCE_MEMORY_DLG_H_
#define MEMORY_SOURCE_MEMORY_DLG_H_

#include "memory_def.h"

class MemoryDlg : public CDialog
{
public:
	MemoryDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_MEMORY_DIALOG };

	// 解答ダイアログが参照する直前の出題内容
	int GetProblemCount() const { return problem_count_; }
	PlayMode GetPlayMode() const { return play_mode_; }
	const CString& GetRecord(int index) const { return record_[index]; }

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM l_param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR id_event);
	afx_msg void OnStart();
	afx_msg void OnAns();
	afx_msg void OnKeisan();
	afx_msg void OnAnki();
	afx_msg void OnTypeCheck(UINT id);
	afx_msg void OnHScroll(UINT sb_code, UINT pos, CScrollBar* scroll_bar);
	afx_msg void OnHelp();
	DECLARE_MESSAGE_MAP()

private:
	void ViewText();
	void InitProc();
	void StartProc();
	void EndProc();
	void ItemSts(BOOL flg);
	void SelectMode(PlayMode mode);
	CString KeyGen();
	int GetKeyGenType(int val) const;
	TCHAR GetKeyGenChar(int char_type, int val);
	static int MatchProc(int org_val, int current);
	int StartCheck();
	void SetType(int set_type, BOOL flg);

	HICON		icon_;
	CScrollBar	cycle_bar_;		// 表示速度を設定
	CFont		show_font_;		// 出題文字の表示フォント

	CString		record_[kPrNumMax];	// 表示した値を覚えておく
	int			timer_cycle_ = kCycInitVal;	// タイマ起動周期
	int			count_ = 0;			// 現在の個数
	PlayState	state_ = PlayState::kInit;
	int			problem_count_ = 0;		// 表示回数（出題数）
	int			digits_ = 0;			// 桁数
	int			type_flags_ = 0;		// 数字・アルファベットの組み合わせ（TYPE_*）
	PlayMode	play_mode_ = PlayMode::kAnki;	// 出題中（直前）のモード
	PlayMode	selected_mode_ = PlayMode::kAnki;	// 画面で選んでいるモード
	int			prev_char_ = -1;		// 直前に出題した文字（同じ文字が続かないようにする）
};

#endif  // MEMORY_SOURCE_MEMORY_DLG_H_
