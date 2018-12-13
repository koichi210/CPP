
// DivisionCouplingDlg.cpp : 実装ファイル
//

#include <string> 

#include "stdafx.h"
#include "DivisionCoupling.h"
#include "DivisionCouplingDlg.h"
#include "afxdialogex.h"
#include "StandardTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDivisionCouplingDlg ダイアログ
CDivisionCouplingDlg::CDivisionCouplingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDivisionCouplingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDivisionCouplingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDPR_SPLITMERGE, mdx_progress);
}

BEGIN_MESSAGE_MAP(CDivisionCouplingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_SPLIT, OnBnClickedSplit)
	ON_BN_CLICKED(IDBT_MERGE, OnBnClickedMerge)
	ON_BN_CLICKED(IDBT_SPLIT_BROWSE, OnBnClickedSplitBrowse)
	ON_BN_CLICKED(IDBT_MERGE_BROWSE, OnBnClickedMergeBrowse)
	ON_BN_CLICKED(IDBT_STOP, OnBnClickedStop)
END_MESSAGE_MAP()


// CDivisionCouplingDlg メッセージ ハンドラー

BOOL CDivisionCouplingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CDivisionCouplingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CDivisionCouplingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDivisionCouplingDlg::OnBnClickedSplit()
{
	GetDlgItem(IDET_SPLIT_FNAME)->GetWindowText(m_fNameOrg);
	m_fDivSize = GetDlgItemInt(IDET_SIZE);
	m_fDivSize *= UNIT_VALUE;
	Initialize();
	AfxBeginThread(SplitProcThread, this);
}

void CDivisionCouplingDlg::OnBnClickedMerge()
{
	GetDlgItem(IDET_MERGE_FNAME)->GetWindowText(m_fNameNew);
	Initialize();
	AfxBeginThread(MergeProcThread, this);
}

void CDivisionCouplingDlg::Initialize()
{
	// GUIから設定値を取得
	m_nIdx = 1;
	m_Err = NO_ERROR;
	m_bProc = TRUE;
	EnableControl(m_bProc);
}

void CDivisionCouplingDlg::Result()
{
	m_bProc = FALSE;
	EnableControl(m_bProc);

	if ( m_Err != NO_ERROR )
	{
		// エラーメッセージ作成
		CString msg;
		switch(m_Err)
		{
		case ERR_OPEN_ORGFILE :
			msg.Format("元ファイルオープンエラー");
			break;

		case ERR_OPEN_NEWFILE :
			msg.Format("ファイルが作成できません\n\n%s" ,m_fNameNew);
			break;

		case ERR_CALLOC :
			msg.Format("Workメモリ確保エラー");
			break;

		case ERR_DIV_SIZE :
			msg.Format("分割サイズエラー");
			break;

		default :
			break;
		}

		MessageBox(msg , "error", MB_OK);
	}
}

void CDivisionCouplingDlg::SplitProc()
{
	if ( m_fDivSize <= 0 )
	{
		m_Err = ERR_DIV_SIZE;
		return;
	}

	if ( ! m_cstOrgFile.Open(m_fNameOrg, CFile::typeBinary) )
	{
		m_Err = ERR_OPEN_ORGFILE;
		return;
	}

	char *buff;	// 読み込みバッファ（分割最大サイズで確保）
	buff = (char *)calloc(m_fDivSize, sizeof(char));
	if ( ! buff )
	{
		m_Err = ERR_CALLOC;
		return;
	}

	// 残りサイズの初期値は、元ファイルのサイズ
	m_fRestSize = (size_t)m_cstOrgFile.GetLength();

	// プログレスバーセット
	SetProgressSplitFileNum();
	mdx_progress.SetRange32(0, GetProgressBarEnd());

	int szDivSize;
	while( m_fRestSize > 0 && m_bProc )
	{
		m_fNameNew.Format("%s.div%03d", m_fNameOrg, m_nIdx);
		if ( ! m_cstNewFile.Open(m_fNameNew, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
		{
			m_Err = ERR_OPEN_NEWFILE;
			break;
		}

		// 今回の分割サイズを決定
		if ( m_fRestSize > m_fDivSize )
		{
			szDivSize = m_fDivSize;
		}
		else
		{
			szDivSize = m_fRestSize;
		}
		m_fRestSize -= szDivSize;

		// バイナリデータをRead&Write
		m_cstOrgFile.Read(buff, szDivSize);
		m_cstNewFile.Write(buff, szDivSize);
		m_cstNewFile.Close();

		// プログレスバーの更新
		UpdateProgressBar(m_nIdx);

		// ファイル名のインデックス
		m_nIdx++;
	}

	free(buff);
	m_cstOrgFile.Close();
}

void CDivisionCouplingDlg::MergeProc()
{
	std::string fullpath = m_fNameNew;

	// ex) fname.jpg.div001　→　fname.jpg
	int idx = fullpath.find_last_of(".");
	std::string pathname = fullpath.substr(0, idx);
	m_fNameNew = pathname.c_str();

	if ( PathFileExists(m_fNameNew))
	{
		int MsgResult = MessageBox("すでにファイルが存在します。上書きしますか？\n" + m_fNameNew,"Warning", MB_YESNO);
		if ( MsgResult == IDNO)
		{
			MessageBox("処理を中断しました。");
			return;
		}
	}

	if ( ! m_cstNewFile.Open(m_fNameNew, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
	{
		m_Err = ERR_OPEN_NEWFILE;
		return;
	}

	char *buff;	// 読み込みバッファ
	int szDivSize;
	while( m_bProc )
	{
		m_fNameOrg.Format("%s.div%03d", m_fNameNew, m_nIdx);
		if ( ! m_cstOrgFile.Open(m_fNameOrg, CFile::typeBinary) )
		{
			// 読み込むファイルが無くなったら終了
			break;
		}

		szDivSize = (size_t)m_cstOrgFile.GetLength();
		buff = (char *)calloc(szDivSize, sizeof(char));
		if ( ! buff )
		{
			m_Err = ERR_CALLOC;
			break;
		}

		// バイナリデータをRead&Write
		m_cstOrgFile.Read(buff, szDivSize);
		m_cstNewFile.Write(buff, szDivSize);
		m_cstOrgFile.Close();

		// TODO：結合するファイル全数を取得する必要がある
		// プログレスバーの更新
		//UpdateProgressBar(m_nIdx);

		m_nIdx++;
		free(buff);
	}

	m_cstNewFile.Close();
}

void CDivisionCouplingDlg::OnBnClickedSplitBrowse()
{
	OnBrowse(IDET_SPLIT_FNAME, TRUE);
}

void CDivisionCouplingDlg::OnBnClickedMergeBrowse()
{
	OnBrowse(IDET_MERGE_FNAME, TRUE);
}

void CDivisionCouplingDlg::OnBrowse(UINT nID, BOOL bIsFileOpen)
{
	// ファイルを開く
	CFileDialog *dlgFile;

	dlgFile = new CFileDialog(bIsFileOpen, "*.*", NULL, OFN_CREATEPROMPT, "*.*|*.*|全て(*.*)|*.*||");
	if ( dlgFile->DoModal() == IDOK )
	{
		CString strFileName;
		strFileName = dlgFile->GetPathName();
		GetDlgItem(nID)->SetWindowText(strFileName);
	}
}

void CDivisionCouplingDlg::EnableControl(BOOL bEnable)
{
	GetDlgItem(IDBT_SPLIT)->EnableWindow(!bEnable);
	GetDlgItem(IDBT_MERGE)->EnableWindow(!bEnable);
	GetDlgItem(IDBT_STOP)->EnableWindow(bEnable);
}

void CDivisionCouplingDlg::UpdateProgressBar(int nPos)
{
	// Start位置、End位置を設定
	int start,end;
	switch(nPos)
	{
	case POS_INIT :
		start = 0;
		end = 0;
		break;

	case POS_END :
		start = GetProgressBarEnd();
		end = GetProgressBarEnd();
		break;

	default :
		start = nPos;
		end = GetProgressBarEnd();
		break;
	}

	// プログレスバーの現在位置設定
	mdx_progress.SetPos(start);
}

int CDivisionCouplingDlg::GetProgressBarEnd()
{
	return m_MaxNum;
}

void CDivisionCouplingDlg::SetProgressSplitFileNum()
{
	m_MaxNum = m_fRestSize / m_fDivSize;
	if ( m_fRestSize % m_fDivSize)
	{
		m_MaxNum++;
	}
}

void CDivisionCouplingDlg::SetProgressMergeFileNum()
{
	m_MaxNum = 1;
	while( TRUE )
	{
		m_fNameOrg.Format("%s.div%03d" ,m_fNameNew, m_MaxNum);
		if ( ! m_cstOrgFile.Open(m_fNameOrg, CFile::typeBinary) )
		{
			break;
		}
		m_cstOrgFile.Close();
		m_MaxNum++;
	}
}

UINT SplitProcThread(LPVOID pParam)
{
	CDivisionCouplingDlg *pDlg = (CDivisionCouplingDlg*)pParam;
	pDlg->SplitProc();
	pDlg->Result();
	return TRUE;
}

UINT MergeProcThread(LPVOID pParam)
{
	CDivisionCouplingDlg *pDlg = (CDivisionCouplingDlg*)pParam;
	pDlg->MergeProc();
	pDlg->Result();
	return TRUE;
}

void CDivisionCouplingDlg::OnBnClickedStop()
{
	m_bProc = FALSE;
	EnableControl(m_bProc);
}
