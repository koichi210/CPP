// common_ctrl.h : 共通の MFC コントロール

#pragma once

#include <afxwin.h>
#include <afxcmn.h>

#include <memory>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CBitmapStatic : ビットマップの右上隅の色を透過色として、コントロールいっぱいに伸縮描画する

class CBitmapStatic : public CStatic
{
public:
	void SetBitmapId(UINT id)	{ m_bitmapId = id; }

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	UINT m_bitmapId = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CRestrictedEdit : 入力できる1バイト文字を制限するエディット

class CRestrictedEdit : public CEdit
{
public:
	enum CharKind
	{
		CHAR_ANY,			// 制限なし
		CHAR_DIGIT,			// 0〜9
		CHAR_DIGIT_SIGN,	// 0〜9 と '-'
		CHAR_DECIMAL,		// 0〜9 と '.'
		CHAR_DECIMAL_SIGN,	// 0〜9 と '.' と '-'
		CHAR_ASCII,			// 0x00〜0x7F
	};

	void SetCharacterKind(CharKind kind)			{ m_charKind = kind; }
	void SetForbiddenCharacters(LPCTSTR chars)		{ m_forbiddenChars = chars; }	// 個別に入力を禁止する文字
	void DisableCopyAndPaste(BOOL bDisable)			{ m_bDisableCopyAndPaste = bDisable; }

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	bool IsAllowedChar(UINT nChar) const;

	CharKind	m_charKind = CHAR_ANY;
	CString		m_forbiddenChars;
	BOOL		m_bDisableCopyAndPaste = FALSE;
};

/////////////////////////////////////////////////////////////////////////////
// CPopupEdit : リストのセル上に出すポップアップのエディット
// 閉じると owner に WM_POPUPEDIT_CLOSED（wParam = CPopupEdit::Result）を送る

constexpr UINT WM_POPUPEDIT_CLOSED = WM_USER + 121;

class CPopupEdit : public CEdit
{
	DECLARE_DYNAMIC(CPopupEdit)

public:
	enum InputKind
	{
		INPUT_DIGIT			= 0x01,
		INPUT_ALPHA			= 0x02,
		INPUT_DIGIT_ALPHA	= INPUT_DIGIT | INPUT_ALPHA,
		INPUT_ANY			= 0xFF,
	};
	enum Result
	{
		RESULT_CANCEL	= 0,	// Esc で取り消し
		RESULT_OK		= 1,	// Enter またはフォーカス喪失で確定
	};

	explicit CPopupEdit(CWnd* pOwner = nullptr, CPoint cell = CPoint(0, 0));

	BOOL Create(DWORD inputKind, UINT maxLength, LPCTSTR text, const RECT& rect, CWnd* pParentWnd);

	CPoint GetCell() const				{ return m_cell; }
	const CString& GetValue() const		{ return m_value; }

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_MESSAGE_MAP()

private:
	CWnd*		m_pOwner;
	CPoint		m_cell;				// 編集中のセル（x = 列, y = 行）
	Result		m_result = RESULT_OK;
	CString		m_value;			// 閉じたときの入力値
	DWORD		m_inputKind = INPUT_ANY;
	UINT		m_maxLength = 0;	// 0 なら制限なし
};

/////////////////////////////////////////////////////////////////////////////
// CPopupList : リストのセル上に出すポップアップのリストボックス
// 閉じると owner に WM_POPUPLIST_CLOSED を送る

constexpr UINT WM_POPUPLIST_CLOSED = WM_USER + 120;

class CPopupList : public CListBox
{
public:
	explicit CPopupList(CWnd* pOwner = nullptr) : m_pOwner(pOwner) {}

	BOOL Create(const CString* items, int count, const RECT& rect, CWnd* pParentWnd);

	int GetSelectedIndex() const	{ return m_selectedIndex; }	// 未選択なら -1

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_MESSAGE_MAP()

private:
	CWnd*	m_pOwner;
	int		m_selectedIndex = -1;
};

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl : セルを選択・編集できるオーナードローのリストコントロール
// 編集方法は派生クラスで CreatePopup / UseInEditKey をオーバーライドして決める

class CEditableListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditableListCtrl)

public:
	CEditableListCtrl();
	virtual ~CEditableListCtrl();

	void SetListItems(const CString* items, int count);		// ポップアップリストの選択肢
	void SetEditKind(DWORD inputKind)	{ m_editKind = inputKind; }	// CPopupEdit::InputKind
	void SetMaxLength(UINT maxLength)	{ m_maxLength = maxLength; }
	void SetLineColor(COLORREF color)	{ m_lineColor = color; }
	void UseSideHeader(BOOL bUse = TRUE);					// 先頭列を行見出しとして描く
	void CenterJustifyHeader();

protected:
	virtual BOOL UseInEditKey(UINT nChar);					// 編集を始めるキーか（既定は無し）
	virtual void CreatePopup(LONG col, CRect rect);			// 列に応じてポップアップを出す（既定は何もしない）
	void CreatePopupEditBox(const CRect& rect);
	void CreatePopupListBox(const CRect& rect);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;

	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnPopupEditClosed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPopupListClosed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	CPoint	m_cursor;		// カーソルのあるセル（x = 列, y = 行）

private:
	int FirstEditableColumn() const		{ return m_bSideHeader ? 1 : 0; }
	void OpenPopupAtCursor();
	void EditCell(const LVHITTESTINFO& hitTest);

	std::unique_ptr<CPopupList>	m_pPopupList;
	std::unique_ptr<CPopupEdit>	m_pPopupEdit;
	std::vector<CString>		m_listItems;
	DWORD		m_editKind;
	UINT		m_maxLength;
	int			m_pressedItem;		// 直前に押したセル
	int			m_pressedSubItem;
	int			m_selectedItem;		// 選択済みのセル（同じセルを再クリックで編集）
	int			m_selectedSubItem;
	BOOL		m_bSideHeader;
	COLORREF	m_lineColor;
};

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox : アイコン付きのオーナードローコンボボックス

constexpr int MAX_ICON_NUM = 10;

struct ICONCOMBOBOXITEM
{
	UINT	idText;					// 表示する文字列リソースID
	DWORD	value;					// アイテムの値（ItemData）
	DWORD	dwConstraint;			// このビットが SetItemList の dwConstraint と重なると表示しない
	UINT	idIcon[MAX_ICON_NUM];	// アイコンリソースID（SetItemList の iconIndex で選ぶ）
	int		idxItem;				// コンボボックス上のインデックス（非表示なら -1）
};

class CIconComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CIconComboBox)

public:
	CIconComboBox();

	void SetItemList(ICONCOMBOBOXITEM* pItemList, UINT itemCount, UINT iconIndex = 0, DWORD dwConstraint = 0);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
	DECLARE_MESSAGE_MAP()

private:
	const ICONCOMBOBOXITEM* FindItem(UINT comboIndex) const;

	CSize				m_iconSize;
	int					m_itemHeight;
	ICONCOMBOBOXITEM*	m_pItemList = nullptr;
	UINT				m_itemCount = 0;
	UINT				m_iconIndex = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CSimpleListCtrl : 先頭列に行番号を振る表

class CSimpleListCtrl : public CListCtrl
{
public:
	void AddColumn(int column, int width, LPCTSTR name);
	void SetRowCount(int count)		{ m_rowCount = count; }
	void FillColumn(int column, LPCTSTR text);		// 列0なら行番号付きで行を作り、それ以外は全行に text を入れる
	void SelectRow(int row);
	void EnableFullRowSelect();

private:
	int m_rowCount = 1;
};
