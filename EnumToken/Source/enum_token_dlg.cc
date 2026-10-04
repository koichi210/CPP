// enum_token_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "enum_token.h"
#include "enum_token_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// バージョン情報ダイアログ（システムメニューから開く）
	class AboutDlg : public CDialog
	{
	public:
		AboutDlg() : CDialog(IDD_ABOUTBOX) {}
	};

	// トークンが属するグループ（TOKEN_GROUPS）を buffer に読み出す
	bool QueryTokenGroups(HANDLE token, std::vector<BYTE>* buffer)
	{
		// 1回目はサイズ 0 で呼んで必要なバッファサイズを得る
		DWORD size = 0;
		if (GetTokenInformation(token, TokenGroups, nullptr, 0, &size)
			|| GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			return false;
		}
		buffer->resize(size);
		return GetTokenInformation(token, TokenGroups, buffer->data(), size, &size) != FALSE;
	}
}

EnumTokenDlg::EnumTokenDlg(CWnd* parent /*=nullptr*/)
	: CDialog(EnumTokenDlg::IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(EnumTokenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GETPROC, &EnumTokenDlg::OnGetproc)
END_MESSAGE_MAP()

BOOL EnumTokenDlg::OnInitDialog()
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

void EnumTokenDlg::OnSysCommand(UINT id, LPARAM l_param)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg about_dlg;
		about_dlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(id, l_param);
	}
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void EnumTokenDlg::OnPaint()
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

HCURSOR EnumTokenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// 自プロセスのアクセストークンが属するグループの名前とドメインを表示する
void EnumTokenDlg::OnGetproc()
{
	HANDLE token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
	{
		return;
	}
	std::vector<BYTE> buffer;
	const bool queried = QueryTokenGroups(token, &buffer);
	CloseHandle(token);
	if (!queried)
	{
		return;
	}

	const auto* groups = reinterpret_cast<const TOKEN_GROUPS*>(buffer.data());
	CString names;
	CString domains;
	for (DWORD i = 0; i < groups->GroupCount; i++)
	{
		TCHAR name[256];
		DWORD name_len = _countof(name);
		TCHAR domain[256];
		DWORD domain_len = _countof(domain);
		SID_NAME_USE use;

		if (!LookupAccountSid(nullptr, groups->Groups[i].Sid, name, &name_len, domain, &domain_len, &use))
		{
			// 名前を引けない SID（ログオン SID など）は空欄扱い
			name[0] = _T('\0');
			domain[0] = _T('\0');
		}

		names += name;
		names += _T("\n");
		domains += (domain[0] == _T('\0')) ? _T("(not available name)") : domain;
		domains += _T("\n");
	}
	SetDlgItemText(IDC_PROCTOKEN_NAME, names);
	SetDlgItemText(IDC_PROCTOKEN_DOMAIN, domains);
}
