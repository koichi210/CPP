// macro_tool_dlg.h : 設定ダイアログ（イベントの一覧編集・ファイル読み書き・記録）

#pragma once

#include "macro_defs.h"
#include "common_ctrl.h"

class CMacroToolDlg : public CDialog
{
public:
	// 繰り返し回数と待ち時間は、キャンセルしても呼び出し元に反映される（従来どおり）
	CMacroToolDlg(CWnd* pParent, const CString& fileName, const std::vector<MACROEVENT>& events,
		UINT& repeatCount, UINT& repeatDelayMsec);
	virtual ~CMacroToolDlg();

	enum { IDD = IDD_MACROTOOL_DIALOG };

	std::vector<MACROEVENT> GetEvents() const;			// 未設定の行を詰めたもの
	const CString& GetFileName() const	{ return m_fileName; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	// 一覧の編集
	afx_msg void OnListInsert();
	afx_msg void OnListDelete();
	afx_msg void OnListCopy();
	afx_msg void OnListPaste();
	afx_msg void OnAllClear();

	// 選択行の設定
	afx_msg void OnEnChangeExecute();
	afx_msg void OnEnChangeSleep();
	afx_msg void OnEnChangeComment();
	afx_msg void OnMouse();
	afx_msg void OnCbnSelchangeMouse();
	afx_msg void OnEnChangeMouseX();
	afx_msg void OnEnChangeMouseY();
	afx_msg void OnKey();
	afx_msg void OnCbnSelchangeKey();
	afx_msg void OnEnChangeKey();
	afx_msg void OnKeyShift();
	afx_msg void OnKeyCtrl();
	afx_msg void OnKeyAlt();

	// 共通設定
	afx_msg void OnEnChangeRepeatNum();
	afx_msg void OnEnChangeRepeatTime();

	afx_msg void OnRead();
	afx_msg void OnWrite();
	afx_msg void OnRecord();
	afx_msg void OnHelp();
	DECLARE_MESSAGE_MAP()

private:
	// EventHookd.dll の公開関数（extern "C" の __cdecl）
	using HookFunc = BOOL (__cdecl*)();
	using DebugModeFunc = void (__cdecl*)(BOOL isDebug);

	MACROEVENT& CurrentEvent()	{ return m_events[m_index]; }

	void InitControls();
	void SelectEventKind(EventKind kind);
	void EnsureEventKind(EventKind kind);
	void SetModifier(UINT checkId, DWORD modifier);
	int GetClampedDlgItemInt(UINT id, int maxValue, BOOL bSigned);

	// ファイル
	void LoadFile(const CString& fileName);
	BOOL SelectFile(BOOL bOpen);

	// 記録
	void StartRecord();
	void StopRecord();

	// 設定欄の表示更新（値が変わったコントロールだけ書き換える）
	void UpdateControl();
	void UpdateControlEvent();
	void UpdateControlDetail();
	void SetDlgItemNumberIfChanged(UINT id, int value);
	void SetDlgItemTextIfChanged(UINT id, LPCTSTR text);
	void CheckDlgButtonIfChanged(UINT id, bool check);

	// 一覧の表示更新
	void UpdateListControl(BOOL bAll = FALSE);
	void UpdateListCell(int row, int column);

	int GetTotalTime() const;
	void SetTitleBar();
	BOOL ConfirmSettings();

	HICON					m_hIcon;
	SimpleListCtrl			m_list;
	RestrictedEdit			m_keyEdit;
	CComboBox				m_mouseCombo;
	CComboBox				m_keyCombo;

	std::vector<MACROEVENT>	m_events;			// 一覧の全行
	MACROEVENT				m_clipboard;		// コピーした行
	int						m_index = 0;		// 選択中の行
	CString					m_fileName;
	UINT&					m_repeatCount;
	UINT&					m_repeatDelayMsec;

	BOOL					m_bRecording = FALSE;
	BOOL					m_bDebug = FALSE;	// フック DLL にログを書かせる
	HINSTANCE				m_hHookDll;
	HookFunc				m_pfnStartKeyHook = nullptr;
	HookFunc				m_pfnStopKeyHook = nullptr;
	HookFunc				m_pfnStartMouseHook = nullptr;
	HookFunc				m_pfnStopMouseHook = nullptr;
	DebugModeFunc			m_pfnDebugMode = nullptr;
};
