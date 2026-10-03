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
	class CAboutDlg : public CDialog
	{
	public:
		CAboutDlg() : CDialog(IDD_ABOUTBOX) {}
	};
}

CJointMovieDlg::CJointMovieDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CJointMovieDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CJointMovieDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OUT_BROWSE, IDC_IN8_BROWSE, &CJointMovieDlg::OnBrowse)
	ON_BN_CLICKED(IDC_EXECUTE, &CJointMovieDlg::OnExecute)
END_MESSAGE_MAP()

BOOL CJointMovieDlg::OnInitDialog()
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

void CJointMovieDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CJointMovieDlg::OnPaint()
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

HCURSOR CJointMovieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 「参照」ボタン：選んだファイル名を対応する入力欄へ設定する
void CJointMovieDlg::OnBrowse(UINT nID)
{
	const int editId = (nID == IDC_OUT_BROWSE)
		? IDC_OUTPUTFILE
		: IDC_INPUTFILE1 + static_cast<int>(nID - IDC_IN1_BROWSE);

	TCHAR fileNames[MAX_PATH] = {};
	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
		_T("動画（*.mpg; *.mpeg;）|*.mpg; *.mpeg;|すべてのﾌｧｲﾙ （*.*）|*.*||"), this);
	dlg.GetOFN().lpstrFile = fileNames;
	dlg.GetOFN().nMaxFile = _countof(fileNames);
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(editId, fileNames);
	}
}

// 「実行」ボタン：copy /B 入力1+入力2+... 出力 で連結する（空欄の入力は飛ばす）
void CJointMovieDlg::OnExecute()
{
	CString outputFile;
	GetDlgItemText(IDC_OUTPUTFILE, outputFile);

	CString inputFiles;
	for (int i = 0; i < kInputFileCount; i++)
	{
		CString file;
		GetDlgItemText(IDC_INPUTFILE1 + i, file);
		if (file.IsEmpty())
		{
			continue;
		}
		if (!inputFiles.IsEmpty())
		{
			inputFiles += _T("+");
		}
		// 空白を含むパスでも copy に1つの引数として渡るよう引用符で囲む
		inputFiles += _T("\"") + file + _T("\"");
	}

	if (outputFile.IsEmpty())
	{
		MessageBox(_T("結合先ファイル名が不正"));
	}
	else if (inputFiles.IsEmpty())
	{
		MessageBox(_T("元ファイル名が不正"));
	}
	else
	{
		CString command;
		command.Format(_T("copy /B /-Y %s \"%s\""), static_cast<LPCTSTR>(inputFiles), static_cast<LPCTSTR>(outputFile));
		_tsystem(command);
	}
}
