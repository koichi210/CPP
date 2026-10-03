// common_ctrl.cc : 共通の MFC コントロール

#include "common_ctrl.h"

namespace
{
	bool IsDigitChar(UINT c)	{ return _T('0') <= c && c <= _T('9'); }
	bool IsAlphaChar(UINT c)	{ return (_T('A') <= c && c <= _T('Z')) || (_T('a') <= c && c <= _T('z')); }
	bool IsControlChar(UINT c)	{ return c < 0x20 || c == 0x7F; }

	const UINT kCtrlC = 0x03;	// コピー
	const UINT kCtrlV = 0x16;	// 貼り付け
	const UINT kCtrlX = 0x18;	// 切り取り

	void FillWithColor(CDC* pDC, const CRect& rect, COLORREF color)
	{
		CBrush brush(color);
		pDC->FillRect(&rect, &brush);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapStatic

BEGIN_MESSAGE_MAP(CBitmapStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CBitmapStatic::OnPaint()
{
	CPaintDC dc(this);

	CBitmap bitmap;
	if (!bitmap.LoadBitmap(m_bitmapId))
	{
		return;
	}

	BITMAP bm;
	bitmap.GetBitmap(&bm);

	CRect client;
	GetClientRect(&client);

	CDC dcImage;
	CDC dcMask;
	CDC dcOffScreen;
	dcImage.CreateCompatibleDC(&dc);
	dcMask.CreateCompatibleDC(&dc);
	dcOffScreen.CreateCompatibleDC(&dc);

	// 透過色（右上隅の色）の部分が白になるモノクロのマスクを作る
	CBitmap bmpMask;
	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, nullptr);
	CBitmap* pOldMask = dcMask.SelectObject(&bmpMask);
	CBitmap* pOldImage = dcImage.SelectObject(&bitmap);
	dcImage.SetBkColor(dcImage.GetPixel(bm.bmWidth - 1, 0));
	dcMask.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);

	// 背景をボタン色で塗る
	FillWithColor(&dc, client, ::GetSysColor(COLOR_BTNFACE));

	// 背景 XOR 画像 → AND マスク → XOR 画像 で透過部分だけ背景を残す
	CBitmap bmpOffScreen;
	bmpOffScreen.CreateBitmap(bm.bmWidth, bm.bmHeight, static_cast<UINT>(dc.GetDeviceCaps(PLANES)), static_cast<UINT>(dc.GetDeviceCaps(BITSPIXEL)), nullptr);
	CBitmap* pOldOffScreen = dcOffScreen.SelectObject(&bmpOffScreen);
	dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc, 0, 0, SRCCOPY);
	dcOffScreen.SetBkColor(RGB(255, 255, 255));
	dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCINVERT);
	dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcMask, 0, 0, SRCAND);
	dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCINVERT);

	dc.StretchBlt(0, 0, client.Width(), client.Height(), &dcOffScreen, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	dcOffScreen.SelectObject(pOldOffScreen);
	dcImage.SelectObject(pOldImage);
	dcMask.SelectObject(pOldMask);
}

/////////////////////////////////////////////////////////////////////////////
// CRestrictedEdit

BEGIN_MESSAGE_MAP(CRestrictedEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

bool CRestrictedEdit::IsAllowedChar(UINT nChar) const
{
	if (IsControlChar(nChar))
	{
		const bool isClipboardKey = (nChar == kCtrlC || nChar == kCtrlV || nChar == kCtrlX);
		return !(m_bDisableCopyAndPaste && isClipboardKey);
	}

	const bool isDigit = IsDigitChar(nChar);
	switch (m_charKind)
	{
	case CHAR_DIGIT:		if (!isDigit) return false;										break;
	case CHAR_DIGIT_SIGN:	if (!isDigit && nChar != _T('-')) return false;					break;
	case CHAR_DECIMAL:		if (!isDigit && nChar != _T('.')) return false;					break;
	case CHAR_DECIMAL_SIGN:	if (!isDigit && nChar != _T('.') && nChar != _T('-')) return false;	break;
	case CHAR_ASCII:		if (nChar >= 0x80) return false;								break;
	default:																				break;
	}

	for (int i = 0; i < m_forbiddenChars.GetLength(); i++)
	{
		if (nChar == static_cast<unsigned char>(m_forbiddenChars[i]))
		{
			return false;
		}
	}
	return true;
}

void CRestrictedEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (IsAllowedChar(nChar))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

// コピペ無効のときはコンテキストメニューも出さない
void CRestrictedEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (!m_bDisableCopyAndPaste)
	{
		CEdit::OnContextMenu(pWnd, point);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPopupEdit

IMPLEMENT_DYNAMIC(CPopupEdit, CEdit)

BEGIN_MESSAGE_MAP(CPopupEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CPopupEdit::CPopupEdit(CWnd* pOwner, CPoint cell)
	: m_pOwner(pOwner)
	, m_cell(cell)
{
}

// rect はスクリーン座標
BOOL CPopupEdit::Create(DWORD inputKind, UINT maxLength, LPCTSTR text, const RECT& rect, CWnd* pParentWnd)
{
	m_inputKind = inputKind;
	m_maxLength = maxLength;

	if (!CreateEx(WS_EX_TOPMOST, _T("Edit"), text, WS_POPUP | WS_VISIBLE | ES_AUTOHSCROLL, rect, pParentWnd, 0))
	{
		return FALSE;
	}
	SetFocus();
	return TRUE;
}

int CPopupEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (m_pOwner != nullptr)
	{
		SetFont(m_pOwner->GetFont());
		SetSel(0, -1);
	}
	return 0;
}

void CPopupEdit::OnDestroy()
{
	GetWindowText(m_value);

	CWnd* pNotify = (m_pOwner != nullptr) ? m_pOwner : GetParent();
	if (pNotify != nullptr)
	{
		pNotify->PostMessage(WM_POPUPEDIT_CLOSED, static_cast<WPARAM>(m_result), 0);
	}

	CEdit::OnDestroy();
}

void CPopupEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool allowed = false;
	bool limitLength = true;

	switch (nChar)
	{
	case VK_RETURN:
		m_result = RESULT_OK;
		DestroyWindow();
		return;

	case VK_ESCAPE:
		m_result = RESULT_CANCEL;
		DestroyWindow();
		return;

	case VK_BACK:
		allowed = true;
		limitLength = false;	// 削除は文字数制限の対象外
		break;

	case VK_SPACE:
		allowed = true;
		break;

	default:
		allowed = (m_inputKind == INPUT_ANY)
			|| (IsDigitChar(nChar) && (m_inputKind & INPUT_DIGIT))
			|| (IsAlphaChar(nChar) && (m_inputKind & INPUT_ALPHA));
		break;
	}

	if (!allowed)
	{
		return;
	}
	if (limitLength && m_maxLength != 0 && m_maxLength <= static_cast<UINT>(GetWindowTextLength()))
	{
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

// クリップボード経由で入力させないよう、コンテキストメニューを出さない
void CPopupEdit::OnRButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
}

void CPopupEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CPopupList

BEGIN_MESSAGE_MAP(CPopupList, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// rect はスクリーン座標
BOOL CPopupList::Create(const CString* items, int count, const RECT& rect, CWnd* pParentWnd)
{
	if (!CreateEx(WS_EX_TOPMOST, _T("LISTBOX"), _T(""), WS_BORDER | WS_VISIBLE | WS_VSCROLL | WS_POPUP | LBS_NOINTEGRALHEIGHT, rect, pParentWnd, 0))
	{
		return FALSE;
	}

	for (int i = 0; i < count; i++)
	{
		AddString(items[i]);
	}
	SetFocus();
	return TRUE;
}

int CPopupList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (m_pOwner != nullptr)
	{
		SetFont(m_pOwner->GetFont());
	}
	return 0;
}

void CPopupList::OnDestroy()
{
	CListBox::OnDestroy();

	CWnd* pNotify = (m_pOwner != nullptr) ? m_pOwner : GetParent();
	if (pNotify != nullptr)
	{
		pNotify->PostMessage(WM_POPUPLIST_CLOSED, 0, 0);
	}
}

void CPopupList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		DestroyWindow();
		break;

	case VK_RETURN:
		if (GetCurSel() != LB_ERR)
		{
			m_selectedIndex = GetCurSel();
		}
		DestroyWindow();
		break;

	default:
		CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
}

void CPopupList::OnLButtonUp(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonUp(nFlags, point);

	if (GetCurSel() != LB_ERR)
	{
		m_selectedIndex = GetCurSel();
		DestroyWindow();
	}
}

void CPopupList::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl

IMPLEMENT_DYNAMIC(CEditableListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_POPUPEDIT_CLOSED, &CEditableListCtrl::OnPopupEditClosed)
	ON_MESSAGE(WM_POPUPLIST_CLOSED, &CEditableListCtrl::OnPopupListClosed)
END_MESSAGE_MAP()

CEditableListCtrl::CEditableListCtrl()
	: m_cursor(0, 0)
	, m_editKind(CPopupEdit::INPUT_ANY)
	, m_maxLength(0)
	, m_pressedItem(0)
	, m_pressedSubItem(0)
	, m_selectedItem(0)
	, m_selectedSubItem(0)
	, m_bSideHeader(FALSE)
	, m_lineColor(RGB(0, 0, 0))
{
}

CEditableListCtrl::~CEditableListCtrl()
{
}

void CEditableListCtrl::SetListItems(const CString* items, int count)
{
	m_listItems.assign(items, items + count);
}

void CEditableListCtrl::UseSideHeader(BOOL bUse)
{
	m_bSideHeader = bUse;
	if (m_cursor.x < FirstEditableColumn())
	{
		m_cursor.x = FirstEditableColumn();
	}
}

void CEditableListCtrl::CenterJustifyHeader()
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();

	HDITEM item = {};
	item.mask = HDI_FORMAT;
	item.fmt = HDF_CENTER | HDF_STRING;

	for (int i = 0; i < pHeader->GetItemCount(); i++)
	{
		pHeader->SetItem(i, &item);
	}
}

BOOL CEditableListCtrl::UseInEditKey(UINT /*nChar*/)
{
	return FALSE;
}

void CEditableListCtrl::CreatePopup(LONG /*col*/, CRect /*rect*/)
{
}

void CEditableListCtrl::CreatePopupEditBox(const CRect& rect)
{
	CRect frame = rect;
	frame.right -= 1;
	frame.bottom -= 1;

	m_pPopupEdit = std::make_unique<CPopupEdit>(this, m_cursor);
	m_pPopupEdit->Create(m_editKind, m_maxLength, GetItemText(m_cursor.y, m_cursor.x), frame, this);
}

// リストの高さはセルの高さ × 選択肢の数
void CEditableListCtrl::CreatePopupListBox(const CRect& rect)
{
	CRect frame = rect;
	frame.top -= 1;
	frame.left -= 1;
	frame.bottom = frame.top + frame.Height() * static_cast<int>(m_listItems.size());

	m_pPopupList = std::make_unique<CPopupList>(this);
	m_pPopupList->Create(m_listItems.data(), static_cast<int>(m_listItems.size()), frame, this);
}

void CEditableListCtrl::OpenPopupAtCursor()
{
	CRect rect;
	GetSubItemRect(m_cursor.y, m_cursor.x, LVIR_BOUNDS, rect);
	ClientToScreen(&rect);
	CreatePopup(m_cursor.x, rect);
}

void CEditableListCtrl::EditCell(const LVHITTESTINFO& hitTest)
{
	const int column = max(hitTest.iSubItem, FirstEditableColumn());
	m_cursor = CPoint(column, hitTest.iItem);
	OpenPopupAtCursor();
}

void CEditableListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();
	m_pPopupList.reset();
	m_pPopupEdit.reset();
}

// 矢印キーでセルを移動、スペースまたは UseInEditKey のキーで編集
void CEditableListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool callDefault = true;

	// 何も選択していないときはキー操作しない
	if (GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) != -1)
	{
		const int columnCount = GetHeaderCtrl()->GetItemCount();
		const int rowCount = GetItemCount();

		switch (nChar)
		{
		case VK_RIGHT:
			m_cursor.x = min(m_cursor.x + 1, columnCount - 1);
			break;

		case VK_LEFT:
			m_cursor.x = max(m_cursor.x - 1, FirstEditableColumn());
			break;

		case VK_DOWN:
			m_cursor.y = min(m_cursor.y + 1, rowCount - 1);
			break;

		case VK_UP:
			m_cursor.y = max(m_cursor.y - 1, 0);
			break;

		case VK_SPACE:
			OpenPopupAtCursor();
			break;

		default:
			if (UseInEditKey(nChar))
			{
				OpenPopupAtCursor();
			}
			else
			{
				callDefault = false;
			}
			break;
		}
	}

	if (callDefault)
	{
		CRect rect;
		GetSubItemRect(m_cursor.y, 0, LVIR_BOUNDS, rect);
		InvalidateRect(&rect);
		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

// 文字入力でのインクリメンタルサーチを無効にする
void CEditableListCtrl::OnChar(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
}

void CEditableListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitTest = {};
	hitTest.pt = point;

	if (SubItemHitTest(&hitTest) == -1)
	{
		CListCtrl::OnLButtonDown(nFlags, point);
		return;
	}

	m_pressedItem = hitTest.iItem;
	m_pressedSubItem = hitTest.iSubItem;
	m_cursor = CPoint(max(hitTest.iSubItem, FirstEditableColumn()), hitTest.iItem);

	SetFocus();
	SetItemState(hitTest.iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	CRect rect;
	GetItemRect(hitTest.iItem, &rect, LVIR_BOUNDS);
	InvalidateRect(&rect);
}

// 1回目のクリックで選択、選択中のセルをもう一度クリックで編集
void CEditableListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitTest = {};
	hitTest.pt = point;

	if (SubItemHitTest(&hitTest) != -1)
	{
		const bool isPressedCell = (hitTest.iItem == m_pressedItem && hitTest.iSubItem == m_pressedSubItem);
		const bool isSelectedCell = (hitTest.iItem == m_selectedItem && hitTest.iSubItem == m_selectedSubItem);

		if (isPressedCell && isSelectedCell)
		{
			EditCell(hitTest);
		}
		else
		{
			m_selectedItem = m_pressedItem;
			m_selectedSubItem = m_pressedSubItem;
		}
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}

void CEditableListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitTest = {};
	hitTest.pt = point;

	if (SubItemHitTest(&hitTest) != -1)
	{
		EditCell(hitTest);
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

// 行見出しを右クリックすると不正なセルが選択状態になるので無視する
void CEditableListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bSideHeader)
	{
		CListCtrl::OnRButtonDown(nFlags, point);
	}
}

LRESULT CEditableListCtrl::OnPopupEditClosed(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_pPopupEdit)
	{
		if (wParam == CPopupEdit::RESULT_OK)
		{
			const CPoint cell = m_pPopupEdit->GetCell();
			SetItemText(cell.y, cell.x, m_pPopupEdit->GetValue());
		}
		m_pPopupEdit.reset();
	}
	return TRUE;
}

LRESULT CEditableListCtrl::OnPopupListClosed(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_pPopupList)
	{
		const int index = m_pPopupList->GetSelectedIndex();
		if (0 <= index && index < static_cast<int>(m_listItems.size()))
		{
			SetItemText(m_cursor.y, m_cursor.x, m_listItems[index]);
		}
		m_pPopupList.reset();
	}
	return TRUE;
}

// カーソル列のセルを強調表示し、行見出しはボタン風に描く
void CEditableListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	const int item = static_cast<int>(lpDrawItemStruct->itemID);
	const bool isSelectedRow = (GetItemState(item, LVIS_SELECTED) == LVIS_SELECTED);
	const int columnCount = GetHeaderCtrl()->GetItemCount();

	for (int col = 0; col < columnCount; col++)
	{
		const CString text = GetItemText(item, col);
		const bool isSideHeader = (col == 0 && m_bSideHeader);

		CRect rect;
		if (col == 0)
		{
			// 列0は GetSubItemRect だと行全体になるので、ラベル部分の右端までにする
			CRect itemRect;
			GetItemRect(item, &rect, LVIR_LABEL);
			GetItemRect(item, &itemRect, LVIR_BOUNDS);
			rect.top = itemRect.top;
			rect.left = itemRect.left;
		}
		else
		{
			GetSubItemRect(item, col, LVIR_BOUNDS, rect);
		}

		COLORREF textColor = ::GetSysColor(COLOR_WINDOWTEXT);
		if (isSelectedRow && col == m_cursor.x)
		{
			if (isSideHeader)
			{
				FillWithColor(pDC, rect, ::GetSysColor(COLOR_3DFACE));
				pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
			}
			else
			{
				FillWithColor(pDC, rect, ::GetSysColor(COLOR_HIGHLIGHT));
			}
			textColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
		else if (isSideHeader)
		{
			FillWithColor(pDC, rect, ::GetSysColor(COLOR_3DFACE));
			pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));
		}
		else
		{
			FillWithColor(pDC, rect, ::GetSysColor(COLOR_WINDOW));
		}

		// 罫線
		if (!isSideHeader)
		{
			CBrush border(m_lineColor);
			CRect frame = rect;
			frame.top -= 1;
			frame.left -= 1;
			pDC->FrameRect(&frame, &border);
		}

		pDC->SetTextColor(textColor);
		CRect textRect = rect;
		textRect.left += 2;
		const UINT format = DT_WORD_ELLIPSIS | DT_SINGLELINE | (isSideHeader ? DT_CENTER : DT_LEFT);
		pDC->DrawText(text, &textRect, format);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox

IMPLEMENT_DYNAMIC(CIconComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CIconComboBox, CComboBox)
END_MESSAGE_MAP()

CIconComboBox::CIconComboBox()
	: m_iconSize(::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON))
	, m_itemHeight(::GetSystemMetrics(SM_CYICON) + 4)	// 上下に2pxずつ余白
{
}

// dwConstraint のビットと重なる項目は表示しない
void CIconComboBox::SetItemList(ICONCOMBOBOXITEM* pItemList, UINT itemCount, UINT iconIndex, DWORD dwConstraint)
{
	m_pItemList = pItemList;
	m_itemCount = itemCount;
	m_iconIndex = iconIndex;

	ResetContent();
	SetItemHeight(-1, m_itemHeight);

	int comboIndex = 0;
	for (UINT i = 0; i < m_itemCount; i++)
	{
		pItemList[i].idxItem = -1;
		if (pItemList[i].dwConstraint & dwConstraint)
		{
			continue;
		}

		comboIndex = InsertString(comboIndex, _T(""));
		if (comboIndex != CB_ERR)
		{
			SetItemData(comboIndex, pItemList[i].value);
			SetItemHeight(comboIndex, m_itemHeight);
			pItemList[i].idxItem = comboIndex;
			comboIndex++;
		}
	}
}

const ICONCOMBOBOXITEM* CIconComboBox::FindItem(UINT comboIndex) const
{
	for (UINT i = 0; i < m_itemCount; i++)
	{
		if (m_pItemList[i].idxItem == static_cast<int>(comboIndex))
		{
			return &m_pItemList[i];
		}
	}
	return nullptr;
}

void CIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct == nullptr || lpDrawItemStruct->itemID == static_cast<UINT>(-1))
	{
		return;
	}

	const ICONCOMBOBOXITEM* pItem = FindItem(lpDrawItemStruct->itemID);
	if (pItem == nullptr)
	{
		return;
	}

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;

	UINT stateFlags = DSS_NORMAL;
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_HIGHLIGHT));
		pDC->DrawFocusRect(&rect);
	}
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_MENU));
		stateFlags = DSS_DISABLED;
	}
	else
	{
		pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
		pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	}

	HICON hIcon = AfxGetApp()->LoadIcon(pItem->idIcon[m_iconIndex]);
	if (hIcon != nullptr)
	{
		pDC->DrawState(CPoint(rect.left + 2, rect.top + 2), m_iconSize, hIcon, stateFlags, static_cast<HBRUSH>(nullptr));
	}

	CString text;
	text.LoadString(pItem->idText);
	rect.left += m_itemHeight;
	pDC->DrawText(text, &rect, DT_SINGLELINE | DT_VCENTER);
}

/////////////////////////////////////////////////////////////////////////////
// CSimpleListCtrl

void CSimpleListCtrl::AddColumn(int column, int width, LPCTSTR name)
{
	InsertColumn(column, name, LVCFMT_LEFT, width, column);
}

void CSimpleListCtrl::FillColumn(int column, LPCTSTR text)
{
	for (int row = 0; row < m_rowCount; row++)
	{
		if (column == 0)
		{
			CString number;
			number.Format(_T("%d"), row + 1);
			InsertItem(row, number);
		}
		else
		{
			SetItemText(row, column, text);
		}
	}
}

void CSimpleListCtrl::SelectRow(int row)
{
	SetItemState(row, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	SetSelectionMark(row);
}

void CSimpleListCtrl::EnableFullRowSelect()
{
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}
