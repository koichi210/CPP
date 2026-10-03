// SplitPathOwnDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "split_path_own.h"
#include "split_path_own_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	char file_full_path[256] = "C:/Windows/System32/explorer.exe";
	// strncpy は終端の '\0' を付けないので、出力先は0で埋めておく
	char drive[256] = "";
	char dir[256] = "";
	char fname[256] = "";
	SplitPath(file_full_path, drive, dir, fname);

	CString result;
	result.Format("org=%s\n\n drv=%s\n dir=%s\n fname=%s",
			file_full_path,
			drive,
			dir,
			fname
			);
	MessageBox(result);
}

void SplitPathOwnDlg::SplitPath(const char* file_full_path, char* drive, char* dir, char* file)
{
	const char* pt;

	/* ドライブ名取得 */
	{
		pt = file_full_path;	/* ポインタを先頭に移動*/
		while(*pt != '/')
		{
			pt++;
		}
		strncpy(drive, file_full_path, strlen(file_full_path) - strlen(pt) + 1);	// 「+1」は終端の「/」を追加
	}

	/* ファイル名取得 */
	{
		pt = file_full_path + strlen(file_full_path);	/* ポインタを終端に移動 */
		while(*pt != '/')
		{
			pt--;
		}
		pt++;	// 先頭の「/」を削除
		strncpy(file, pt, strlen(pt));
	}

	/* ディレクトリ名取得 */
	{
		pt = &file_full_path[strlen(drive)];	/* ポインタをドライブレターの後ろに移動 */
		strncpy(dir, pt, strlen(pt) - strlen(file));
	}
}
