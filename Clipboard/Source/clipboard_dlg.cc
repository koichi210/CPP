// clipboard_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "clipboard.h"
#include "clipboard_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClipboardDlg::CClipboardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CClipboardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClipboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_TEXT, m_strText);
}

BEGIN_MESSAGE_MAP(CClipboardDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_COPY_CLIPBOARD, &CClipboardDlg::OnBnClickedCopyClipboard)
END_MESSAGE_MAP()

BOOL CClipboardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void CClipboardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CClipboardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClipboardDlg::OnBnClickedCopyClipboard()
{
	UpdateData(TRUE);
	if (!SetClipboardText(m_strText))
	{
		MessageBox(_T("エラーが発生しました"));
	}
}

// CF_TEXT（マルチバイト文字列）としてクリップボードに設定する
bool CClipboardDlg::SetClipboardText(const CStringA& text)
{
	// 終端の '\0' も含めて渡す
	const SIZE_T size = static_cast<SIZE_T>(text.GetLength()) + 1;

	// クリップボードに渡すメモリは移動可能な共有メモリでなければならない
	HGLOBAL hMem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, size);
	if (hMem == nullptr)
	{
		return false;
	}

	void* pBuf = GlobalLock(hMem);
	if (pBuf == nullptr)
	{
		GlobalFree(hMem);
		return false;
	}
	memcpy(pBuf, static_cast<LPCSTR>(text), size);
	GlobalUnlock(hMem);

	if (!OpenClipboard())
	{
		GlobalFree(hMem);
		return false;
	}
	EmptyClipboard();
	// 成功するとメモリの所有権はクリップボードへ移る
	const bool succeeded = (SetClipboardData(CF_TEXT, hMem) != nullptr);
	CloseClipboard();

	if (!succeeded)
	{
		GlobalFree(hMem);
	}
	return succeeded;
}
