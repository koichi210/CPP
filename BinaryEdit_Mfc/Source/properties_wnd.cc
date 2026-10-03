// properties_wnd.cc : プロパティ ウィンドウ（ドッキングペイン）

#include "stdafx.h"
#include "properties_wnd.h"
#include "Resource.h"
#include "main_frm.h"
#include "binary_edit_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, &PropertiesWnd::OnExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, &PropertiesWnd::OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, &PropertiesWnd::OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, &PropertiesWnd::OnNotImplemented)
	ON_COMMAND(ID_PROPERTIES2, &PropertiesWnd::OnNotImplemented)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

void PropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rect_client, rect_combo;
	GetClientRect(rect_client);

	object_combo_.GetWindowRect(&rect_combo);

	int combo_height = rect_combo.Size().cy;
	int toolbar_height = tool_bar_.CalcFixedLayout(FALSE, TRUE).cy;

	object_combo_.SetWindowPos(nullptr, rect_client.left, rect_client.top, rect_client.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	tool_bar_.SetWindowPos(nullptr, rect_client.left, rect_client.top + combo_height, rect_client.Width(), toolbar_height, SWP_NOACTIVATE | SWP_NOZORDER);
	prop_list_.SetWindowPos(nullptr, rect_client.left, rect_client.top + combo_height + toolbar_height, rect_client.Width(), rect_client.Height() - (combo_height + toolbar_height), SWP_NOACTIVATE | SWP_NOZORDER);
}

int PropertiesWnd::OnCreate(LPCREATESTRUCT create_struct)
{
	if (CDockablePane::OnCreate(create_struct) == -1)
		return -1;

	CRect rect_dummy;
	rect_dummy.SetRectEmpty();

	const DWORD view_style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!object_combo_.Create(view_style, rect_dummy, this, 1))
	{
		TRACE0("プロパティ コンボ ボックスを作成できませんでした\n");
		return -1;
	}

	object_combo_.AddString(_T("アプリケーション"));
	object_combo_.AddString(_T("プロパティ ウィンドウ"));
	object_combo_.SetCurSel(0);

	if (!prop_list_.Create(WS_VISIBLE | WS_CHILD, rect_dummy, this, 2))
	{
		TRACE0("プロパティ グリッドを作成できませんでした\n");
		return -1;
	}

	InitPropList();

	tool_bar_.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	tool_bar_.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ロック */);
	tool_bar_.CleanUpLockedImages();
	tool_bar_.LoadBitmap(the_app.hi_color_icons_ ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ロック */);

	tool_bar_.SetPaneStyle(tool_bar_.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	tool_bar_.SetPaneStyle(tool_bar_.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	tool_bar_.SetOwner(this);

	// コマンドを親フレーム経由ではなくこのペインで受ける
	tool_bar_.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void PropertiesWnd::OnSize(UINT type, int cx, int cy)
{
	CDockablePane::OnSize(type, cx, cy);
	AdjustLayout();
}

void PropertiesWnd::OnExpandAllProperties()
{
	prop_list_.ExpandAll();
}

void PropertiesWnd::OnSortProperties()
{
	prop_list_.SetAlphabeticMode(!prop_list_.IsAlphabeticMode());
}

void PropertiesWnd::OnUpdateSortProperties(CCmdUI* cmd_ui)
{
	cmd_ui->SetCheck(prop_list_.IsAlphabeticMode());
}

// 未実装のボタン。ハンドラーが無いと灰色になるため空で受ける
void PropertiesWnd::OnNotImplemented()
{
}

void PropertiesWnd::InitPropList()
{
	SetPropListFont();

	// 表示確認用のダミーデータ
	prop_list_.EnableHeaderCtrl(FALSE);
	prop_list_.EnableDescriptionArea();
	prop_list_.SetVSDotNetLook();
	prop_list_.MarkModifiedProperties();

	CMFCPropertyGridProperty* group1 = new CMFCPropertyGridProperty(_T("表示"));

	group1->AddSubItem(new CMFCPropertyGridProperty(_T("3D 表示"), (_variant_t) false, _T("ウィンドウのフォントが太字以外になり、また、コントロールが 3D ボーダーで描画されます")));

	CMFCPropertyGridProperty* prop = new CMFCPropertyGridProperty(_T("罫線"), _T("ダイアログ枠"), _T("次のうちのどれかです : なし、細枠、サイズ変更可能枠、ダイアログ枠"));
	prop->AddOption(_T("なし"));
	prop->AddOption(_T("細枠"));
	prop->AddOption(_T("サイズ変更可能枠"));
	prop->AddOption(_T("ダイアログ枠"));
	prop->AllowEdit(FALSE);

	group1->AddSubItem(prop);
	group1->AddSubItem(new CMFCPropertyGridProperty(_T("キャプション"), (_variant_t) _T("バージョン情報"), _T("ウィンドウのタイトル バーに表示されるテキストを指定します")));

	prop_list_.AddProperty(group1);

	CMFCPropertyGridProperty* size_group = new CMFCPropertyGridProperty(_T("ウィンドウ サイズ"), 0, TRUE);

	prop = new CMFCPropertyGridProperty(_T("高さ"), (_variant_t) 250l, _T("ウィンドウの高さを指定します"));
	prop->EnableSpinControl(TRUE, 50, 300);
	size_group->AddSubItem(prop);

	prop = new CMFCPropertyGridProperty( _T("幅"), (_variant_t) 150l, _T("ウィンドウの幅を指定します"));
	prop->EnableSpinControl(TRUE, 50, 200);
	size_group->AddSubItem(prop);

	prop_list_.AddProperty(size_group);

	CMFCPropertyGridProperty* group2 = new CMFCPropertyGridProperty(_T("フォント"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("ＭＳ Ｐゴシック"));

	group2->AddSubItem(new CMFCPropertyGridFontProperty(_T("フォント"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ウィンドウの既定フォントを指定します")));
	group2->AddSubItem(new CMFCPropertyGridProperty(_T("システム フォントを使用する"), (_variant_t) true, _T("ウィンドウで MS Shell Dlg フォントを使用するように指定します")));

	prop_list_.AddProperty(group2);

	CMFCPropertyGridProperty* group3 = new CMFCPropertyGridProperty(_T("その他"));
	prop = new CMFCPropertyGridProperty(_T("(名前)"), _T("アプリケーション"));
	prop->Enable(FALSE);
	group3->AddSubItem(prop);

	CMFCPropertyGridColorProperty* color_prop = new CMFCPropertyGridColorProperty(_T("ウィンドウの色"), RGB(210, 192, 254), nullptr, _T("ウィンドウの既定の色を指定します"));
	color_prop->EnableOtherButton(_T("その他..."));
	color_prop->EnableAutomaticButton(_T("既定値"), ::GetSysColor(COLOR_3DFACE));
	group3->AddSubItem(color_prop);

	static const TCHAR kFilter[] = _T("アイコン ファイル (*.ico)|*.ico|すべてのファイル (*.*)|*.*||");
	group3->AddSubItem(new CMFCPropertyGridFileProperty(_T("アイコン"), TRUE, _T(""), _T("ico"), 0, kFilter, _T("ウィンドウ アイコンを指定します")));

	group3->AddSubItem(new CMFCPropertyGridFileProperty(_T("フォルダー"), _T("c:\\")));

	prop_list_.AddProperty(group3);

	CMFCPropertyGridProperty* group4 = new CMFCPropertyGridProperty(_T("階層"));

	CMFCPropertyGridProperty* group41 = new CMFCPropertyGridProperty(_T("1 番目のサブレベル"));
	group4->AddSubItem(group41);

	CMFCPropertyGridProperty* group411 = new CMFCPropertyGridProperty(_T("2 番目のサブレベル"));
	group41->AddSubItem(group411);

	group411->AddSubItem(new CMFCPropertyGridProperty(_T("項目 1"), (_variant_t) _T("値 1"), _T("これは説明です")));
	group411->AddSubItem(new CMFCPropertyGridProperty(_T("項目 2"), (_variant_t) _T("値 2"), _T("これは説明です")));
	group411->AddSubItem(new CMFCPropertyGridProperty(_T("項目 3"), (_variant_t) _T("値 3"), _T("これは説明です")));

	group4->Expand(FALSE);
	prop_list_.AddProperty(group4);
}

void PropertiesWnd::OnSetFocus(CWnd* old_wnd)
{
	CDockablePane::OnSetFocus(old_wnd);
	prop_list_.SetFocus();
}

void PropertiesWnd::OnSettingChange(UINT flags, LPCTSTR section)
{
	CDockablePane::OnSettingChange(flags, section);
	SetPropListFont();
}

void PropertiesWnd::SetPropListFont()
{
	::DeleteObject(prop_list_font_.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	prop_list_font_.CreateFontIndirect(&lf);

	prop_list_.SetFont(&prop_list_font_);
	object_combo_.SetFont(&prop_list_font_);
}
