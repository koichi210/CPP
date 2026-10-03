// common_ctrl.h : 共通の MFC コントロール

#ifndef COMMON_COMMON_CTRL_H_
#define COMMON_COMMON_CTRL_H_

#include <afxwin.h>
#include <afxcmn.h>

#include <memory>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// BitmapStatic : ビットマップの右上隅の色を透過色として、コントロールいっぱいに伸縮描画する

class BitmapStatic : public CStatic
{
public:
	void SetBitmapId(UINT id)	{ bitmap_id_ = id; }

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	UINT bitmap_id_ = 0;
};

/////////////////////////////////////////////////////////////////////////////
// RestrictedEdit : 入力できる1バイト文字を制限するエディット

class RestrictedEdit : public CEdit
{
public:
	enum CharKind
	{
		kCharAny,			// 制限なし
		kCharDigit,			// 0〜9
		kCharDigitSign,	// 0〜9 と '-'
		kCharDecimal,		// 0〜9 と '.'
		kCharDecimalSign,	// 0〜9 と '.' と '-'
		kCharAscii,			// 0x00〜0x7F
	};

	void SetCharacterKind(CharKind kind)			{ char_kind_ = kind; }
	void SetForbiddenCharacters(LPCTSTR chars)		{ forbidden_chars_ = chars; }	// 個別に入力を禁止する文字
	void DisableCopyAndPaste(BOOL disable)			{ disable_copy_and_paste_ = disable; }

protected:
	afx_msg void OnChar(UINT char_code, UINT rep_count, UINT flags);
	afx_msg void OnContextMenu(CWnd* wnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	bool IsAllowedChar(UINT char_code) const;

	CharKind	char_kind_ = kCharAny;
	CString		forbidden_chars_;
	BOOL		disable_copy_and_paste_ = FALSE;
};

/////////////////////////////////////////////////////////////////////////////
// PopupEdit : リストのセル上に出すポップアップのエディット
// 閉じると owner に kWmPopupEditClosed（wparam = PopupEdit::Result）を送る

constexpr UINT kWmPopupEditClosed = WM_USER + 121;

class PopupEdit : public CEdit
{
	DECLARE_DYNAMIC(PopupEdit)

public:
	enum InputKind
	{
		kInputDigit			= 0x01,
		kInputAlpha			= 0x02,
		kInputDigitAlpha	= kInputDigit | kInputAlpha,
		kInputAny			= 0xFF,
	};
	enum Result
	{
		kResultCancel	= 0,	// Esc で取り消し
		kResultOk		= 1,	// Enter またはフォーカス喪失で確定
	};

	explicit PopupEdit(CWnd* owner = nullptr, CPoint cell = CPoint(0, 0));

	BOOL Create(DWORD input_kind, UINT max_length, LPCTSTR text, const RECT& rect, CWnd* parent_wnd);

	CPoint GetCell() const				{ return cell_; }
	const CString& GetValue() const		{ return value_; }

protected:
	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT char_code, UINT rep_count, UINT flags);
	afx_msg void OnRButtonUp(UINT flags, CPoint point);
	afx_msg void OnKillFocus(CWnd* new_wnd);
	DECLARE_MESSAGE_MAP()

private:
	CWnd*		owner_;
	CPoint		cell_;				// 編集中のセル（x = 列, y = 行）
	Result		result_ = kResultOk;
	CString		value_;			// 閉じたときの入力値
	DWORD		input_kind_ = kInputAny;
	UINT		max_length_ = 0;	// 0 なら制限なし
};

/////////////////////////////////////////////////////////////////////////////
// PopupList : リストのセル上に出すポップアップのリストボックス
// 閉じると owner に kWmPopupListClosed を送る

constexpr UINT kWmPopupListClosed = WM_USER + 120;

class PopupList : public CListBox
{
public:
	explicit PopupList(CWnd* owner = nullptr) : owner_(owner) {}

	BOOL Create(const CString* items, int count, const RECT& rect, CWnd* parent_wnd);

	int GetSelectedIndex() const	{ return selected_index_; }	// 未選択なら -1

protected:
	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT char_code, UINT rep_count, UINT flags);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnKillFocus(CWnd* new_wnd);
	DECLARE_MESSAGE_MAP()

private:
	CWnd*	owner_;
	int		selected_index_ = -1;
};

/////////////////////////////////////////////////////////////////////////////
// EditableListCtrl : セルを選択・編集できるオーナードローのリストコントロール
// 編集方法は派生クラスで CreatePopup / UseInEditKey をオーバーライドして決める

class EditableListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(EditableListCtrl)

public:
	EditableListCtrl();
	virtual ~EditableListCtrl();

	void SetListItems(const CString* items, int count);		// ポップアップリストの選択肢
	void SetEditKind(DWORD input_kind)	{ edit_kind_ = input_kind; }	// PopupEdit::InputKind
	void SetMaxLength(UINT max_length)	{ max_length_ = max_length; }
	void SetLineColor(COLORREF color)	{ line_color_ = color; }
	void UseSideHeader(BOOL use = TRUE);					// 先頭列を行見出しとして描く
	void CenterJustifyHeader();

protected:
	virtual BOOL UseInEditKey(UINT char_code);					// 編集を始めるキーか（既定は無し）
	virtual void CreatePopup(LONG col, CRect rect);			// 列に応じてポップアップを出す（既定は何もしない）
	void CreatePopupEditBox(const CRect& rect);
	void CreatePopupListBox(const CRect& rect);

	virtual void DrawItem(LPDRAWITEMSTRUCT draw_item_struct) override;

	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT char_code, UINT rep_count, UINT flags);
	afx_msg void OnChar(UINT char_code, UINT rep_count, UINT flags);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT flags, CPoint point);
	afx_msg void OnRButtonDown(UINT flags, CPoint point);
	afx_msg LRESULT OnPopupEditClosed(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnPopupListClosed(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

	CPoint	cursor_;		// カーソルのあるセル（x = 列, y = 行）

private:
	int FirstEditableColumn() const		{ return side_header_ ? 1 : 0; }
	void OpenPopupAtCursor();
	void EditCell(const LVHITTESTINFO& hit_test);

	std::unique_ptr<PopupList>	popup_list_;
	std::unique_ptr<PopupEdit>	popup_edit_;
	std::vector<CString>		list_items_;
	DWORD		edit_kind_;
	UINT		max_length_;
	int			pressed_item_;		// 直前に押したセル
	int			pressed_sub_item_;
	int			selected_item_;		// 選択済みのセル（同じセルを再クリックで編集）
	int			selected_sub_item_;
	BOOL		side_header_;
	COLORREF	line_color_;
};

/////////////////////////////////////////////////////////////////////////////
// IconComboBox : アイコン付きのオーナードローコンボボックス

constexpr int kMaxIconNum = 10;

struct IconComboBoxItem
{
	UINT	text_id;					// 表示する文字列リソースID
	DWORD	value;					// アイテムの値（ItemData）
	DWORD	constraint;			// このビットが SetItemList の constraint と重なると表示しない
	UINT	icon_ids[kMaxIconNum];	// アイコンリソースID（SetItemList の icon_index で選ぶ）
	int		combo_index;				// コンボボックス上のインデックス（非表示なら -1）
};

class IconComboBox : public CComboBox
{
	DECLARE_DYNAMIC(IconComboBox)

public:
	IconComboBox();

	void SetItemList(IconComboBoxItem* item_list, UINT item_count, UINT icon_index = 0, DWORD constraint = 0);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT draw_item_struct) override;
	DECLARE_MESSAGE_MAP()

private:
	const IconComboBoxItem* FindItem(UINT combo_index) const;

	CSize				icon_size_;
	int					item_height_;
	IconComboBoxItem*	item_list_ = nullptr;
	UINT				item_count_ = 0;
	UINT				icon_index_ = 0;
};

/////////////////////////////////////////////////////////////////////////////
// SimpleListCtrl : 先頭列に行番号を振る表

class SimpleListCtrl : public CListCtrl
{
public:
	void AddColumn(int column, int width, LPCTSTR name);
	void SetRowCount(int count)		{ row_count_ = count; }
	void FillColumn(int column, LPCTSTR text);		// 列0なら行番号付きで行を作り、それ以外は全行に text を入れる
	void SelectRow(int row);
	void EnableFullRowSelect();

private:
	int row_count_ = 1;
};

#endif  // COMMON_COMMON_CTRL_H_
