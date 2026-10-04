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

	void FillWithColor(CDC* dc, const CRect& rect, COLORREF color)
	{
		CBrush brush(color);
		dc->FillRect(&rect, &brush);
	}
}

/////////////////////////////////////////////////////////////////////////////
// BitmapStatic

BEGIN_MESSAGE_MAP(BitmapStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void BitmapStatic::OnPaint()
{
	CPaintDC dc(this);

	CBitmap bitmap;
	if (!bitmap.LoadBitmap(bitmap_id_))
	{
		return;
	}

	BITMAP bm;
	bitmap.GetBitmap(&bm);

	CRect client;
	GetClientRect(&client);

	CDC dc_image;
	CDC dc_mask;
	CDC dc_off_screen;
	dc_image.CreateCompatibleDC(&dc);
	dc_mask.CreateCompatibleDC(&dc);
	dc_off_screen.CreateCompatibleDC(&dc);

	// 透過色（右上隅の色）の部分が白になるモノクロのマスクを作る
	CBitmap bmp_mask;
	bmp_mask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, nullptr);
	CBitmap* old_mask = dc_mask.SelectObject(&bmp_mask);
	CBitmap* old_image = dc_image.SelectObject(&bitmap);
	dc_image.SetBkColor(dc_image.GetPixel(bm.bmWidth - 1, 0));
	dc_mask.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc_image, 0, 0, SRCCOPY);

	// 背景をボタン色で塗る
	FillWithColor(&dc, client, ::GetSysColor(COLOR_BTNFACE));

	// 背景 XOR 画像 → AND マスク → XOR 画像 で透過部分だけ背景を残す
	CBitmap bmp_off_screen;
	bmp_off_screen.CreateBitmap(bm.bmWidth, bm.bmHeight, static_cast<UINT>(dc.GetDeviceCaps(PLANES)), static_cast<UINT>(dc.GetDeviceCaps(BITSPIXEL)), nullptr);
	CBitmap* old_off_screen = dc_off_screen.SelectObject(&bmp_off_screen);
	dc_off_screen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc, 0, 0, SRCCOPY);
	dc_off_screen.SetBkColor(RGB(255, 255, 255));
	dc_off_screen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc_image, 0, 0, SRCINVERT);
	dc_off_screen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc_mask, 0, 0, SRCAND);
	dc_off_screen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc_image, 0, 0, SRCINVERT);

	dc.StretchBlt(0, 0, client.Width(), client.Height(), &dc_off_screen, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	dc_off_screen.SelectObject(old_off_screen);
	dc_image.SelectObject(old_image);
	dc_mask.SelectObject(old_mask);
}

/////////////////////////////////////////////////////////////////////////////
// RestrictedEdit

BEGIN_MESSAGE_MAP(RestrictedEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

bool RestrictedEdit::IsAllowedChar(UINT char_code) const
{
	if (IsControlChar(char_code))
	{
		const bool is_clipboard_key = (char_code == kCtrlC || char_code == kCtrlV || char_code == kCtrlX);
		return !(disable_copy_and_paste_ && is_clipboard_key);
	}

	const bool is_digit = IsDigitChar(char_code);
	bool allowed = true;
	switch (char_kind_)
	{
	case kCharDigit:		allowed = is_digit;													break;
	case kCharDigitSign:	allowed = is_digit || char_code == _T('-');							break;
	case kCharDecimal:		allowed = is_digit || char_code == _T('.');							break;
	case kCharDecimalSign:	allowed = is_digit || char_code == _T('.') || char_code == _T('-');	break;
	case kCharAscii:		allowed = char_code < 0x80;											break;
	default:																					break;
	}
	if (!allowed)
	{
		return false;
	}

	// 2バイト文字の1バイトずつ届くので、バイト単位で比べる
	const int forbidden_count = forbidden_chars_.GetLength();
	for (int i = 0; i < forbidden_count; i++)
	{
		if (char_code == static_cast<unsigned char>(forbidden_chars_[i]))
		{
			return false;
		}
	}
	return true;
}

void RestrictedEdit::OnChar(UINT char_code, UINT rep_count, UINT flags)
{
	if (IsAllowedChar(char_code))
	{
		CEdit::OnChar(char_code, rep_count, flags);
	}
}

// コピペ無効のときはコンテキストメニューも出さない
void RestrictedEdit::OnContextMenu(CWnd* wnd, CPoint point)
{
	if (!disable_copy_and_paste_)
	{
		CEdit::OnContextMenu(wnd, point);
	}
}

/////////////////////////////////////////////////////////////////////////////
// PopupEdit

IMPLEMENT_DYNAMIC(PopupEdit, CEdit)

BEGIN_MESSAGE_MAP(PopupEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

PopupEdit::PopupEdit(CWnd* owner, CPoint cell)
	: owner_(owner)
	, cell_(cell)
{
}

// rect はスクリーン座標
BOOL PopupEdit::Create(DWORD input_kind, UINT max_length, LPCTSTR text, const RECT& rect, CWnd* parent_wnd)
{
	input_kind_ = input_kind;
	max_length_ = max_length;

	if (!CreateEx(WS_EX_TOPMOST, _T("Edit"), text, WS_POPUP | WS_VISIBLE | ES_AUTOHSCROLL, rect, parent_wnd, 0))
	{
		return FALSE;
	}
	SetFocus();
	return TRUE;
}

int PopupEdit::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CEdit::OnCreate(create_struct) == -1)
	{
		return -1;
	}

	if (owner_ != nullptr)
	{
		SetFont(owner_->GetFont());
		SetSel(0, -1);
	}
	return 0;
}

void PopupEdit::OnDestroy()
{
	GetWindowText(value_);

	CWnd* notify = (owner_ != nullptr) ? owner_ : GetParent();
	if (notify != nullptr)
	{
		notify->PostMessage(kWmPopupEditClosed, static_cast<WPARAM>(result_), 0);
	}

	CEdit::OnDestroy();
}

void PopupEdit::OnChar(UINT char_code, UINT rep_count, UINT flags)
{
	bool allowed = false;
	bool limit_length = true;

	switch (char_code)
	{
	case VK_RETURN:
		result_ = kResultOk;
		DestroyWindow();
		return;

	case VK_ESCAPE:
		result_ = kResultCancel;
		DestroyWindow();
		return;

	case VK_BACK:
		allowed = true;
		limit_length = false;	// 削除は文字数制限の対象外
		break;

	case VK_SPACE:
		allowed = true;
		break;

	default:
		allowed = (input_kind_ == kInputAny)
			|| (IsDigitChar(char_code) && (input_kind_ & kInputDigit))
			|| (IsAlphaChar(char_code) && (input_kind_ & kInputAlpha));
		break;
	}

	if (!allowed)
	{
		return;
	}
	if (limit_length && max_length_ != 0 && max_length_ <= static_cast<UINT>(GetWindowTextLength()))
	{
		return;
	}

	CEdit::OnChar(char_code, rep_count, flags);
}

// クリップボード経由で入力させないよう、コンテキストメニューを出さない
void PopupEdit::OnRButtonUp(UINT /*flags*/, CPoint /*point*/)
{
}

void PopupEdit::OnKillFocus(CWnd* new_wnd)
{
	CEdit::OnKillFocus(new_wnd);
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// PopupList

BEGIN_MESSAGE_MAP(PopupList, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// rect はスクリーン座標
BOOL PopupList::Create(const CString* items, int count, const RECT& rect, CWnd* parent_wnd)
{
	if (!CreateEx(WS_EX_TOPMOST, _T("LISTBOX"), _T(""), WS_BORDER | WS_VISIBLE | WS_VSCROLL | WS_POPUP | LBS_NOINTEGRALHEIGHT, rect, parent_wnd, 0))
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

int PopupList::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CListBox::OnCreate(create_struct) == -1)
	{
		return -1;
	}

	if (owner_ != nullptr)
	{
		SetFont(owner_->GetFont());
	}
	return 0;
}

void PopupList::OnDestroy()
{
	CListBox::OnDestroy();

	CWnd* notify = (owner_ != nullptr) ? owner_ : GetParent();
	if (notify != nullptr)
	{
		notify->PostMessage(kWmPopupListClosed, 0, 0);
	}
}

void PopupList::OnKeyDown(UINT char_code, UINT rep_count, UINT flags)
{
	switch (char_code)
	{
	case VK_ESCAPE:
		DestroyWindow();
		break;

	case VK_RETURN:
	{
		const int index = GetCurSel();
		if (index != LB_ERR)
		{
			selected_index_ = index;
		}
		DestroyWindow();
		break;
	}

	default:
		CListBox::OnKeyDown(char_code, rep_count, flags);
		break;
	}
}

void PopupList::OnLButtonUp(UINT flags, CPoint point)
{
	CListBox::OnLButtonUp(flags, point);

	const int index = GetCurSel();
	if (index != LB_ERR)
	{
		selected_index_ = index;
		DestroyWindow();
	}
}

void PopupList::OnKillFocus(CWnd* new_wnd)
{
	CListBox::OnKillFocus(new_wnd);
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// EditableListCtrl

IMPLEMENT_DYNAMIC(EditableListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(EditableListCtrl, CListCtrl)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(kWmPopupEditClosed, &EditableListCtrl::OnPopupEditClosed)
	ON_MESSAGE(kWmPopupListClosed, &EditableListCtrl::OnPopupListClosed)
END_MESSAGE_MAP()

void EditableListCtrl::SetListItems(const CString* items, int count)
{
	list_items_.assign(items, items + count);
}

void EditableListCtrl::UseSideHeader(BOOL use)
{
	side_header_ = use;
	if (cursor_.x < FirstEditableColumn())
	{
		cursor_.x = FirstEditableColumn();
	}
}

void EditableListCtrl::CenterJustifyHeader()
{
	CHeaderCtrl* header = GetHeaderCtrl();

	HDITEM item = {};
	item.mask = HDI_FORMAT;
	item.fmt = HDF_CENTER | HDF_STRING;

	for (int i = 0; i < header->GetItemCount(); i++)
	{
		header->SetItem(i, &item);
	}
}

BOOL EditableListCtrl::UseInEditKey(UINT /*char_code*/)
{
	return FALSE;
}

void EditableListCtrl::CreatePopup(LONG /*col*/, CRect /*rect*/)
{
}

void EditableListCtrl::CreatePopupEditBox(const CRect& rect)
{
	CRect frame = rect;
	frame.right -= 1;
	frame.bottom -= 1;

	popup_edit_ = std::make_unique<PopupEdit>(this, cursor_);
	popup_edit_->Create(edit_kind_, max_length_, GetItemText(cursor_.y, cursor_.x), frame, this);
}

// リストの高さはセルの高さ × 選択肢の数
void EditableListCtrl::CreatePopupListBox(const CRect& rect)
{
	CRect frame = rect;
	frame.top -= 1;
	frame.left -= 1;
	frame.bottom = frame.top + frame.Height() * static_cast<int>(list_items_.size());

	popup_list_ = std::make_unique<PopupList>(this);
	popup_list_->Create(list_items_.data(), static_cast<int>(list_items_.size()), frame, this);
}

void EditableListCtrl::OpenPopupAtCursor()
{
	CRect rect;
	GetSubItemRect(cursor_.y, cursor_.x, LVIR_BOUNDS, rect);
	ClientToScreen(&rect);
	CreatePopup(cursor_.x, rect);
}

// 行見出しの列は編集できないので、隣の列にする
void EditableListCtrl::MoveCursorTo(const LVHITTESTINFO& hit_test)
{
	cursor_ = CPoint(max(hit_test.iSubItem, FirstEditableColumn()), hit_test.iItem);
}

void EditableListCtrl::EditCell(const LVHITTESTINFO& hit_test)
{
	MoveCursorTo(hit_test);
	OpenPopupAtCursor();
}

bool EditableListCtrl::HitTestCell(CPoint point, LVHITTESTINFO& hit_test)
{
	hit_test = {};
	hit_test.pt = point;
	return SubItemHitTest(&hit_test) != -1;
}

void EditableListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();
	popup_list_.reset();
	popup_edit_.reset();
}

// 矢印キーでセルを移動、スペースまたは UseInEditKey のキーで編集
void EditableListCtrl::OnKeyDown(UINT char_code, UINT rep_count, UINT flags)
{
	// 何も選択していないときはセルを動かさない
	if (GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) != -1)
	{
		const int column_count = GetHeaderCtrl()->GetItemCount();
		const int row_count = GetItemCount();

		switch (char_code)
		{
		case VK_RIGHT:
			cursor_.x = min(cursor_.x + 1, column_count - 1);
			break;

		case VK_LEFT:
			cursor_.x = max(cursor_.x - 1, FirstEditableColumn());
			break;

		case VK_DOWN:
			cursor_.y = min(cursor_.y + 1, row_count - 1);
			break;

		case VK_UP:
			cursor_.y = max(cursor_.y - 1, 0);
			break;

		case VK_SPACE:
			OpenPopupAtCursor();
			break;

		default:
			if (!UseInEditKey(char_code))
			{
				return;		// それ以外のキーは無視する
			}
			OpenPopupAtCursor();
			break;
		}
	}

	CRect rect;
	GetSubItemRect(cursor_.y, 0, LVIR_BOUNDS, rect);
	InvalidateRect(&rect);
	CListCtrl::OnKeyDown(char_code, rep_count, flags);
}

// 文字入力でのインクリメンタルサーチを無効にする
void EditableListCtrl::OnChar(UINT /*char_code*/, UINT /*rep_count*/, UINT /*flags*/)
{
}

void EditableListCtrl::OnLButtonDown(UINT flags, CPoint point)
{
	LVHITTESTINFO hit_test;
	if (!HitTestCell(point, hit_test))
	{
		CListCtrl::OnLButtonDown(flags, point);
		return;
	}

	pressed_item_ = hit_test.iItem;
	pressed_sub_item_ = hit_test.iSubItem;
	MoveCursorTo(hit_test);

	SetFocus();
	SetItemState(hit_test.iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	CRect rect;
	GetItemRect(hit_test.iItem, &rect, LVIR_BOUNDS);
	InvalidateRect(&rect);
}

// 1回目のクリックで選択、選択中のセルをもう一度クリックで編集
void EditableListCtrl::OnLButtonUp(UINT flags, CPoint point)
{
	LVHITTESTINFO hit_test;
	if (HitTestCell(point, hit_test))
	{
		const bool is_pressed_cell = (hit_test.iItem == pressed_item_ && hit_test.iSubItem == pressed_sub_item_);
		const bool is_selected_cell = (hit_test.iItem == selected_item_ && hit_test.iSubItem == selected_sub_item_);

		if (is_pressed_cell && is_selected_cell)
		{
			EditCell(hit_test);
		}
		else
		{
			selected_item_ = pressed_item_;
			selected_sub_item_ = pressed_sub_item_;
		}
	}

	CListCtrl::OnLButtonUp(flags, point);
}

void EditableListCtrl::OnLButtonDblClk(UINT flags, CPoint point)
{
	LVHITTESTINFO hit_test;
	if (HitTestCell(point, hit_test))
	{
		EditCell(hit_test);
	}

	CListCtrl::OnLButtonDblClk(flags, point);
}

// 行見出しを右クリックすると不正なセルが選択状態になるので無視する
void EditableListCtrl::OnRButtonDown(UINT flags, CPoint point)
{
	if (!side_header_)
	{
		CListCtrl::OnRButtonDown(flags, point);
	}
}

LRESULT EditableListCtrl::OnPopupEditClosed(WPARAM wparam, LPARAM /*lparam*/)
{
	if (popup_edit_)
	{
		if (wparam == PopupEdit::kResultOk)
		{
			const CPoint cell = popup_edit_->GetCell();
			SetItemText(cell.y, cell.x, popup_edit_->GetValue());
		}
		popup_edit_.reset();
	}
	return TRUE;
}

LRESULT EditableListCtrl::OnPopupListClosed(WPARAM /*wparam*/, LPARAM /*lparam*/)
{
	if (popup_list_)
	{
		const int index = popup_list_->GetSelectedIndex();
		if (0 <= index && index < static_cast<int>(list_items_.size()))
		{
			SetItemText(cursor_.y, cursor_.x, list_items_[index]);
		}
		popup_list_.reset();
	}
	return TRUE;
}

// カーソル列のセルを強調表示し、行見出しはボタン風に描く
void EditableListCtrl::DrawItem(LPDRAWITEMSTRUCT draw_item_struct)
{
	CDC* dc = CDC::FromHandle(draw_item_struct->hDC);
	const int item = static_cast<int>(draw_item_struct->itemID);
	const bool is_selected_row = (GetItemState(item, LVIS_SELECTED) == LVIS_SELECTED);
	const int column_count = GetHeaderCtrl()->GetItemCount();
	CBrush border(line_color_);

	for (int col = 0; col < column_count; col++)
	{
		const CString text = GetItemText(item, col);
		const bool is_side_header = (col == 0 && side_header_);

		CRect rect;
		if (col == 0)
		{
			// 列0は GetSubItemRect だと行全体になるので、ラベル部分の右端までにする
			CRect item_rect;
			GetItemRect(item, &rect, LVIR_LABEL);
			GetItemRect(item, &item_rect, LVIR_BOUNDS);
			rect.top = item_rect.top;
			rect.left = item_rect.left;
		}
		else
		{
			GetSubItemRect(item, col, LVIR_BOUNDS, rect);
		}

		COLORREF text_color = ::GetSysColor(COLOR_WINDOWTEXT);
		if (is_selected_row && col == cursor_.x)
		{
			if (is_side_header)
			{
				FillWithColor(dc, rect, ::GetSysColor(COLOR_3DFACE));
				dc->Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
			}
			else
			{
				FillWithColor(dc, rect, ::GetSysColor(COLOR_HIGHLIGHT));
			}
			text_color = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
		else if (is_side_header)
		{
			FillWithColor(dc, rect, ::GetSysColor(COLOR_3DFACE));
			dc->Draw3dRect(rect, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));
		}
		else
		{
			FillWithColor(dc, rect, ::GetSysColor(COLOR_WINDOW));
		}

		// 罫線
		if (!is_side_header)
		{
			CRect frame = rect;
			frame.top -= 1;
			frame.left -= 1;
			dc->FrameRect(&frame, &border);
		}

		dc->SetTextColor(text_color);
		CRect text_rect = rect;
		text_rect.left += 2;
		const UINT format = DT_WORD_ELLIPSIS | DT_SINGLELINE | (is_side_header ? DT_CENTER : DT_LEFT);
		dc->DrawText(text, &text_rect, format);
	}
}

/////////////////////////////////////////////////////////////////////////////
// IconComboBox

IMPLEMENT_DYNAMIC(IconComboBox, CComboBox)

BEGIN_MESSAGE_MAP(IconComboBox, CComboBox)
END_MESSAGE_MAP()

IconComboBox::IconComboBox()
	: icon_size_(::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON))
	, item_height_(::GetSystemMetrics(SM_CYICON) + 4)	// 上下に2pxずつ余白
{
}

// constraint のビットと重なる項目は表示しない
void IconComboBox::SetItemList(IconComboBoxItem* item_list, UINT item_count, UINT icon_index, DWORD constraint)
{
	item_list_ = item_list;
	item_count_ = item_count;
	icon_index_ = icon_index;

	ResetContent();
	SetItemHeight(-1, item_height_);

	int combo_index = 0;
	for (UINT i = 0; i < item_count_; i++)
	{
		item_list[i].combo_index = -1;
		if (item_list[i].constraint & constraint)
		{
			continue;
		}

		combo_index = InsertString(combo_index, _T(""));
		if (combo_index != CB_ERR)
		{
			SetItemData(combo_index, item_list[i].value);
			SetItemHeight(combo_index, item_height_);
			item_list[i].combo_index = combo_index;
			combo_index++;
		}
	}
}

const IconComboBoxItem* IconComboBox::FindItem(UINT combo_index) const
{
	for (UINT i = 0; i < item_count_; i++)
	{
		if (item_list_[i].combo_index == static_cast<int>(combo_index))
		{
			return &item_list_[i];
		}
	}
	return nullptr;
}

void IconComboBox::DrawItem(LPDRAWITEMSTRUCT draw_item_struct)
{
	if (draw_item_struct == nullptr || draw_item_struct->itemID == static_cast<UINT>(-1))
	{
		return;
	}

	const IconComboBoxItem* item = FindItem(draw_item_struct->itemID);
	if (item == nullptr)
	{
		return;
	}

	CDC* dc = CDC::FromHandle(draw_item_struct->hDC);
	CRect rect = draw_item_struct->rcItem;

	UINT state_flags = DSS_NORMAL;
	if (draw_item_struct->itemState & ODS_SELECTED)
	{
		dc->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc->FillSolidRect(&rect, ::GetSysColor(COLOR_HIGHLIGHT));
		dc->DrawFocusRect(&rect);
	}
	else if (draw_item_struct->itemState & ODS_DISABLED)
	{
		dc->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		dc->FillSolidRect(&rect, ::GetSysColor(COLOR_MENU));
		state_flags = DSS_DISABLED;
	}
	else
	{
		dc->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
		dc->FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	}

	HICON icon = AfxGetApp()->LoadIcon(item->icon_ids[icon_index_]);
	if (icon != nullptr)
	{
		dc->DrawState(CPoint(rect.left + 2, rect.top + 2), icon_size_, icon, state_flags, static_cast<HBRUSH>(nullptr));
	}

	CString text;
	text.LoadString(item->text_id);
	rect.left += item_height_;
	dc->DrawText(text, &rect, DT_SINGLELINE | DT_VCENTER);
}

/////////////////////////////////////////////////////////////////////////////
// SimpleListCtrl

void SimpleListCtrl::AddColumn(int column, int width, LPCTSTR name)
{
	InsertColumn(column, name, LVCFMT_LEFT, width, column);
}

void SimpleListCtrl::FillColumn(int column, LPCTSTR text)
{
	if (column != 0)
	{
		for (int row = 0; row < row_count_; row++)
		{
			SetItemText(row, column, text);
		}
		return;
	}

	CString number;
	for (int row = 0; row < row_count_; row++)
	{
		number.Format(_T("%d"), row + 1);
		InsertItem(row, number);
	}
}

void SimpleListCtrl::SelectRow(int row)
{
	SetItemState(row, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	SetSelectionMark(row);
}

void SimpleListCtrl::EnableFullRowSelect()
{
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}
