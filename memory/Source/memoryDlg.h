// memoryDlg.h : メインダイアログ（出題）

#pragma once

#include "memoryDef.h"

class CMemoryDlg : public CDialog
{
public:
	CMemoryDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_MEMORY_DIALOG };

	// 解答ダイアログが参照する直前の出題内容
	int GetProblemCount() const { return m_problemCount; }
	PlayMode GetPlayMode() const { return m_playMode; }
	const CString& GetRecord(int index) const { return m_record[index]; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnAns();
	afx_msg void OnKeisan();
	afx_msg void OnAnki();
	afx_msg void OnTypeCheck(UINT nID);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
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
	TCHAR GetKeyGenChar(int strType, int val);
	static int MatchProc(int orgVal, int current);
	int StartCheck();
	void SetType(int setType, BOOL flg);

	HICON		m_hIcon;
	CScrollBar	m_cycleBar;		// 表示速度を設定
	CFont		m_showFont;		// 出題文字の表示フォント

	CString		m_record[PR_NUM_MAX];	// 表示した値を覚えておく
	int			m_timerCycle = CYC_INIT_VAL;	// タイマ起動周期
	int			m_count = 0;			// 現在の個数
	PlayState	m_state = PlayState::Init;
	int			m_problemCount = 0;		// 表示回数（出題数）
	int			m_digits = 0;			// 桁数
	int			m_typeFlags = 0;		// 数字・アルファベットの組み合わせ（TYPE_*）
	PlayMode	m_playMode = PlayMode::Anki;	// 出題中（直前）のモード
	PlayMode	m_selectedMode = PlayMode::Anki;	// 画面で選んでいるモード
	int			m_prevChar = -1;		// 直前に出題した文字（同じ文字が続かないようにする）
};
