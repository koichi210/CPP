// JointMovieDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "joint_movie.h"
#include "joint_movie_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// 入力ファイル欄の数（IDC_INPUTFILE1 から連番）
	constexpr int kInputFileCount = 8;

	// バージョン情報ダイアログ（システムメニューから開く）
	class AboutDlg : public CDialog
	{
	public:
		AboutDlg() : CDialog(IDD_ABOUTBOX) {}
	};
}

JointMovieDlg::JointMovieDlg(CWnd* parent /*=nullptr*/)
	: CDialog(JointMovieDlg::IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(JointMovieDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OUT_BROWSE, IDC_IN8_BROWSE, &JointMovieDlg::OnBrowse)
	ON_BN_CLICKED(IDC_EXECUTE, &JointMovieDlg::OnExecute)
END_MESSAGE_MAP()

BOOL JointMovieDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	// （IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* sys_menu = GetSystemMenu(FALSE);
	if (sys_menu != nullptr)
	{
		CString about_menu;
		about_menu.LoadString(IDS_ABOUTBOX);
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

void JointMovieDlg::OnSysCommand(UINT id, LPARAM param)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg about_dlg;
		about_dlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(id, param);
	}
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void JointMovieDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

HCURSOR JointMovieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// 「参照」ボタン：選んだファイル名を対応する入力欄へ設定する
void JointMovieDlg::OnBrowse(UINT id)
{
	const int edit_id = (id == IDC_OUT_BROWSE)
		? IDC_OUTPUTFILE
		: IDC_INPUTFILE1 + static_cast<int>(id - IDC_IN1_BROWSE);

	TCHAR file_names[MAX_PATH] = {};
	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY,
		_T("動画（*.mpg; *.mpeg;）|*.mpg; *.mpeg;|すべてのﾌｧｲﾙ （*.*）|*.*||"), this);
	dlg.GetOFN().lpstrFile = file_names;
	dlg.GetOFN().nMaxFile = _countof(file_names);
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(edit_id, file_names);
	}
}

// 「実行」ボタン：copy /B 入力1+入力2+... 出力 で連結する（空欄の入力は飛ばす）
void JointMovieDlg::OnExecute()
{
	CString output_file;
	GetDlgItemText(IDC_OUTPUTFILE, output_file);

	CString input_files;
	for (int i = 0; i < kInputFileCount; i++)
	{
		CString file;
		GetDlgItemText(IDC_INPUTFILE1 + i, file);
		if (file.IsEmpty())
		{
			continue;
		}
		if (!input_files.IsEmpty())
		{
			input_files += _T("+");
		}
		// 空白を含むパスでも copy に1つの引数として渡るよう引用符で囲む
		input_files += _T("\"") + file + _T("\"");
	}

	if (output_file.IsEmpty())
	{
		MessageBox(_T("結合先ファイル名が不正"));
	}
	else if (input_files.IsEmpty())
	{
		MessageBox(_T("元ファイル名が不正"));
	}
	else
	{
		CString command;
		command.Format(_T("copy /B /-Y %s \"%s\""), static_cast<LPCTSTR>(input_files), static_cast<LPCTSTR>(output_file));
		_tsystem(command);
	}
}
