// EnumTokenDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "EnumToken.h"
#include "EnumTokenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// バージョン情報ダイアログ（システムメニューから開く）
	class CAboutDlg : public CDialog
	{
	public:
		CAboutDlg() : CDialog(IDD_ABOUTBOX) {}
	};
}

CEnumTokenDlg::CEnumTokenDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CEnumTokenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CEnumTokenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GETPROC, &CEnumTokenDlg::OnGetproc)
END_MESSAGE_MAP()

BOOL CEnumTokenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// システムメニューに「バージョン情報」を追加する
	// （IDM_ABOUTBOX はシステムコマンドの範囲内でなければならない）
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

void CEnumTokenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 最小化時のアイコン描画（ダイアログはフレームワークが描いてくれないため）
void CEnumTokenDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

HCURSOR CEnumTokenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 自プロセスのアクセストークンが属するグループの名前とドメインを表示する
void CEnumTokenDlg::OnGetproc()
{
	HANDLE hToken = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		return;
	}

	// 1回目はサイズ 0 で呼んで必要なバッファサイズを得る
	DWORD size = 0;
	if (!GetTokenInformation(hToken, TokenGroups, nullptr, 0, &size)
		&& GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		std::vector<BYTE> buffer(size);
		auto* pGroups = reinterpret_cast<TOKEN_GROUPS*>(buffer.data());
		if (GetTokenInformation(hToken, TokenGroups, pGroups, size, &size))
		{
			CString names;
			CString domains;
			for (DWORD i = 0; i < pGroups->GroupCount; i++)
			{
				TCHAR name[256];
				DWORD nameLen = _countof(name);
				TCHAR domain[256];
				DWORD domainLen = _countof(domain);
				SID_NAME_USE use;

				if (!LookupAccountSid(nullptr, pGroups->Groups[i].Sid, name, &nameLen, domain, &domainLen, &use))
				{
					// 名前を引けない SID（ログオン SID など）は空欄扱い
					name[0] = _T('\0');
					domain[0] = _T('\0');
				}

				names += name;
				names += _T("\n");
				if (domain[0] == _T('\0'))
				{
					domains += _T("(not available name)");
				}
				else
				{
					domains += domain;
				}
				domains += _T("\n");
			}
			SetDlgItemText(IDC_PROCTOKEN_NAME, names);
			SetDlgItemText(IDC_PROCTOKEN_DOMAIN, domains);
		}
	}

	CloseHandle(hToken);
}
