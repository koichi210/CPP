// clipboard_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "clipboard.h"
#include "clipboard_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ClipboardDlg::ClipboardDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(ClipboardDlg::IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ClipboardDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Text(dx, IDET_TEXT, text_);
}

BEGIN_MESSAGE_MAP(ClipboardDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_COPY_CLIPBOARD, &ClipboardDlg::OnBnClickedCopyClipboard)
END_MESSAGE_MAP()

BOOL ClipboardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void ClipboardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int icon_width = GetSystemMetrics(SM_CXICON);
		const int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - icon_width + 1) / 2;
		const int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR ClipboardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void ClipboardDlg::OnBnClickedCopyClipboard()
{
	UpdateData(TRUE);
	if (!SetClipboardText(text_))
	{
		MessageBox(_T("エラーが発生しました"));
	}
}

// CF_TEXT（マルチバイト文字列）としてクリップボードに設定する
bool ClipboardDlg::SetClipboardText(const CStringA& text)
{
	// 終端の '\0' も含めて渡す
	const SIZE_T size = static_cast<SIZE_T>(text.GetLength()) + 1;

	// クリップボードに渡すメモリは移動可能な共有メモリでなければならない
	HGLOBAL mem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, size);
	if (mem == nullptr)
	{
		return false;
	}

	void* buf = GlobalLock(mem);
	if (buf == nullptr)
	{
		GlobalFree(mem);
		return false;
	}
	memcpy(buf, static_cast<LPCSTR>(text), size);
	GlobalUnlock(mem);

	if (!OpenClipboard())
	{
		GlobalFree(mem);
		return false;
	}
	EmptyClipboard();
	// 成功するとメモリの所有権はクリップボードへ移る
	const bool succeeded = (SetClipboardData(CF_TEXT, mem) != nullptr);
	CloseClipboard();

	if (!succeeded)
	{
		GlobalFree(mem);
	}
	return succeeded;
}
