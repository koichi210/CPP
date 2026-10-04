// split_path_own_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "split_path_own.h"
#include "split_path_own_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// [begin, end) の文字列を dst へ写し、終端の '\0' を付ける
	void CopyRange(char* dst, const char* begin, const char* end)
	{
		const size_t len = end - begin;
		memcpy(dst, begin, len);
		dst[len] = '\0';
	}

	// _splitpath を使わずにパスを分解する自前実装（学習用）。区切りは '/' のみ対応
	//   "C:/Windows/System32/explorer.exe" -> drive="C:/", dir="Windows/System32/", file="explorer.exe"
	// 出力先には、それぞれ file_full_path と同じ長さ（終端を含む）以上のバッファを渡す
	void SplitPath(const char* file_full_path, char* drive, char* dir, char* file)
	{
		const char* end = file_full_path + strlen(file_full_path);
		const char* first_slash = strchr(file_full_path, '/');
		if (first_slash == nullptr)
		{
			// 区切りが無ければ全体をファイル名とみなす
			drive[0] = '\0';
			dir[0] = '\0';
			CopyRange(file, file_full_path, end);
			return;
		}
		const char* last_slash = strrchr(file_full_path, '/');

		CopyRange(drive, file_full_path, first_slash + 1);	// 先頭から最初の「/」まで
		CopyRange(dir, first_slash + 1, last_slash + 1);	// ドライブの後ろから最後の「/」まで
		CopyRange(file, last_slash + 1, end);				// 最後の「/」の後ろ
	}
}

// システムメニューの「バージョン情報」から開くダイアログ
class AboutDlg : public CDialogEx
{
public:
	AboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;

	DECLARE_MESSAGE_MAP()
};

AboutDlg::AboutDlg() : CDialogEx(IDD)
{
}

void AboutDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialogEx)
END_MESSAGE_MAP()


SplitPathOwnDlg::SplitPathOwnDlg(CWnd* parent /*=nullptr*/)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SplitPathOwnDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
}

BEGIN_MESSAGE_MAP(SplitPathOwnDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &SplitPathOwnDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL SplitPathOwnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	// （IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* sys_menu = GetSystemMenu(FALSE);
	if (sys_menu != nullptr)
	{
		CString about_menu;
		BOOL name_valid = about_menu.LoadString(IDS_ABOUTBOX);
		ASSERT(name_valid);
		UNREFERENCED_PARAMETER(name_valid);
		if (!about_menu.IsEmpty())
		{
			sys_menu->AppendMenu(MF_SEPARATOR);
			sys_menu->AppendMenu(MF_STRING, IDM_ABOUTBOX, about_menu);
		}
	}

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

void SplitPathOwnDlg::OnSysCommand(UINT id, LPARAM param)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg about_dlg;
		about_dlg.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(id, param);
	}
}

// 最小化時のアイコン描画（ダイアログベースのアプリでは自前で描く必要がある）
void SplitPathOwnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int icon_width = GetSystemMetrics(SM_CXICON);
		int icon_height = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - icon_width + 1) / 2;
		int y = (rect.Height() - icon_height + 1) / 2;

		dc.DrawIcon(x, y, icon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR SplitPathOwnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

void SplitPathOwnDlg::OnBnClickedButton1()
{
	const char file_full_path[] = "C:/Windows/System32/explorer.exe";
	char drive[_countof(file_full_path)];
	char dir[_countof(file_full_path)];
	char fname[_countof(file_full_path)];
	SplitPath(file_full_path, drive, dir, fname);

	CString result;
	result.Format("org=%s\n\n drv=%s\n dir=%s\n fname=%s", file_full_path, drive, dir, fname);
	MessageBox(result);
}
