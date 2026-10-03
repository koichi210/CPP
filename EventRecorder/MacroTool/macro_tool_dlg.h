// macro_tool_dlg.h : 設定ダイアログ（イベントの一覧編集・ファイル読み書き・記録）

#ifndef EVENTRECORDER_MACROTOOL_MACRO_TOOL_DLG_H_
#define EVENTRECORDER_MACROTOOL_MACRO_TOOL_DLG_H_

#include "macro_defs.h"
#include "common_ctrl.h"

class MacroToolDlg : public CDialog
{
public:
	// 繰り返し回数と待ち時間は、キャンセルしても呼び出し元に反映される（従来どおり）
	MacroToolDlg(CWnd* parent, const CString& file_name, const std::vector<MacroEvent>& events,
		UINT& repeat_count, UINT& repeat_delay_msec);
	virtual ~MacroToolDlg();

	enum { IDD = IDD_MACROTOOL_DIALOG };

	std::vector<MacroEvent> GetEvents() const;			// 未設定の行を詰めたもの
	const CString& GetFileName() const	{ return file_name_; }

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;

	afx_msg void OnSysCommand(UINT id, LPARAM l_param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT flags, CPoint point);
	afx_msg void OnLvnItemchangedList(NMHDR* nmhdr, LRESULT* result);

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
	using DebugModeFunc = void (__cdecl*)(BOOL is_debug);

	MacroEvent& CurrentEvent()	{ return events_[index_]; }

	void InitControls();
	void SelectEventKind(EventKind kind);
	void EnsureEventKind(EventKind kind);
	void SetModifier(UINT check_id, DWORD modifier);
	int GetClampedDlgItemInt(UINT id, int max_value, BOOL is_signed);

	// ファイル
	void LoadFile(const CString& file_name);
	BOOL SelectFile(BOOL is_open);

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
	void UpdateListControl(BOOL update_all = FALSE);
	void UpdateListCell(int row, int column);

	int GetTotalTime() const;
	void SetTitleBar();
	BOOL ConfirmSettings();

	HICON					icon_;
	SimpleListCtrl			list_;
	RestrictedEdit			key_edit_;
	CComboBox				mouse_combo_;
	CComboBox				key_combo_;

	std::vector<MacroEvent>	events_;			// 一覧の全行
	MacroEvent				clipboard_;		// コピーした行
	int						index_ = 0;		// 選択中の行
	CString					file_name_;
	UINT&					repeat_count_;
	UINT&					repeat_delay_msec_;

	BOOL					recording_ = FALSE;
	BOOL					debug_ = FALSE;	// フック DLL にログを書かせる
	HINSTANCE				hook_dll_;
	HookFunc				start_key_hook_ = nullptr;
	HookFunc				stop_key_hook_ = nullptr;
	HookFunc				start_mouse_hook_ = nullptr;
	HookFunc				stop_mouse_hook_ = nullptr;
	DebugModeFunc			debug_mode_ = nullptr;
};

#endif  // EVENTRECORDER_MACROTOOL_MACRO_TOOL_DLG_H_
