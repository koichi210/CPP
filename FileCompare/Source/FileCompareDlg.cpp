
// FileCompareDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "FileCompare.h"
#include "FileCompareDlg.h"
#include "afxdialogex.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileCompareDlg ダイアログ
CFileCompareDlg::CFileCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileCompareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bProc = FALSE;
}


void CFileCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDPR_COMPARE, mdx_progress);
}


BEGIN_MESSAGE_MAP(CFileCompareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_EXECUTE, OnBnClickedExecute)
END_MESSAGE_MAP()


// CFileCompareDlg メッセージ ハンドラー
BOOL CFileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	GetDlgItem(IDBT_EXECUTE)->SetWindowText(STR_START);
	GetDlgItem(IDET_PATH)->SetWindowText(STR_DEFALUT_PATH);
	GetDlgItem(IDET_PATH)->SetFocus();
	((CEdit*)GetDlgItem(IDET_PATH))->SetSel(0,-1);
	return FALSE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。
void CFileCompareDlg::OnPaint()
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
HCURSOR CFileCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileCompareDlg::OnBnClickedExecute()
{
	if ( m_bProc )
	{
		StopProc();
		return;
	}

	InitProc();
	GetFolder();
	GetFile();
	Compare();
}


void CFileCompareDlg::StopProc()
{
	GetDlgItem(IDBT_EXECUTE)->SetWindowText(STR_START);
	m_bProc = FALSE;
}

void CFileCompareDlg::InitProc()
{
	m_SameGroup = 1;
	m_FileNum = 0;
	m_bProc = TRUE;

	for ( int i=0; i<MAX_FILE_NUM; i++)
	{
		m_FileList[i].nFindSame = UNKNOWN_SAME;
		m_FileList[i].strFileName = "";
	}

	for ( int i=0; i<MAX_FOLDER_NUM; i++)
	{
		m_FolderList[i] = "";
	}

	GetDlgItem(IDBT_EXECUTE)->SetWindowText(STR_STOP);
	OutputResult(TRUE);
}


void CFileCompareDlg::GetFolder()
{
	CString temp;
	int		nCnt;

	GetDlgItem(IDET_PATH)->GetWindowText(temp);
	if ( temp.IsEmpty() )
	{
		return ;
	}
	
	for(int i=0; i<MAX_FOLDER_NUM; i++)
	{
		nCnt = temp.Find(";");
		if ( nCnt != -1 )
		{
			m_FolderList[i] = temp.Left(nCnt);
			temp.Delete(0, nCnt+1);	//目的の文字列数 + 区切り（;）文字数
		}
		else
		{
			m_FolderList[i] = temp;
			break;
		}
	}
}


void CFileCompareDlg::GetFile()
{
	CFileFind cFind;
	TCHAR pathName[MAX_PATH];

	for ( int i=0; i<MAX_FOLDER_NUM; i++)
	{
		if ( m_FolderList[i].IsEmpty() )
		{
			break;
		}

		// 検索対象フォルダのパス
		sprintf(pathName, "%s\\*", m_FolderList[i]);

		BOOL bContinue = cFind.FindFile(pathName);
		while( bContinue )
		{
			bContinue = cFind.FindNextFile();
			
			if ( cFind.IsDots() )	// "." or ".."
			{
				continue;
			}

			if( cFind.IsDirectory() )
			{
				// フォルダはネストする？
			}
			else // ファイルという認識で良い？
			{
				m_FileList[m_FileNum].strFileName = cFind.GetFilePath();
				if ( m_FileNum < MAX_FILE_NUM )
				{
					m_FileNum++;
				}
				else
				{
					break;
				}
			}
		}
	}
}

void CFileCompareDlg::Compare()
{
	//プログレスバーの幅をセット。総当たりで比較するため式は[x * (x-1) /2 = 1～x-1 までの和]
	mdx_progress.SetRange32(0, GetProgressBarEnd());

	// 位置表示のラベルをリセット
	UpdateProgressBar(POS_INIT);
	AfxBeginThread(ProcThread, this);
}

void CFileCompareDlg::OutputResult(BOOL bClear)
{
	CString str;

	if ( bClear )
	{
		str = "";
	}
	else
	{
		CString temp;
		for ( int i=1; i<m_SameGroup; i++)
		{
			temp.Format("[Group%d]%s", i, END_OF_LINE);
			str += temp;
			for ( int j=0; j<MAX_FILE_NUM; j++)
			{
				if ( m_FileList[j].nFindSame == i )
				{
					str += m_FileList[j].strFileName;
					str += END_OF_LINE;
				}
			}
			str += END_OF_LINE;
		}
	}
	GetDlgItem(IDET_RESULT)->SetWindowText(str);
}

void CFileCompareDlg::Result()
{
	m_bProc = FALSE;
	OutputResult();
}


int CFileCompareDlg::GetProgressBarEnd()
{
	//return m_FileNum*(m_FileNum-1)/2;
	return m_FileNum*(m_FileNum-1);
}


void CFileCompareDlg::UpdateProgressBar(int nPos)
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

	// ラベル表示
	CString str;
	str.Format("(%d / %d)", start, end);
	GetDlgItem(IDST_COMPARE)->SetWindowText(str);
}


UINT ProcThread(LPVOID pParam)
{
	CFileCompareDlg* pDlg=(CFileCompareDlg*)pParam;
	CMyCompareFile cCompFile;
	CString str;
	BOOL bIsSame;

	for( int i=0; i < pDlg->m_FileNum && pDlg->m_bProc; i++ )	// ファイルリストがいっぱいになるまでループ
	{
		pDlg->UpdateProgressBar(i * pDlg->m_FileNum);	// 前の行 * 1列MAX + j
		if ( pDlg->m_FileList[i].nFindSame != UNKNOWN_SAME )
		{
			// すでにダブりを見つけている
			continue;
		}

		for( int j=i+1; j < pDlg->m_FileNum && pDlg->m_bProc; j++ )
		{
			pDlg->UpdateProgressBar(i * pDlg->m_FileNum + j);	// 前の行 * 1列MAX + j
			if ( pDlg->m_FileList[j].nFindSame != UNKNOWN_SAME )
			{
				// すでにダブりを見つけている
				continue;
			}
			bIsSame = cCompFile.CompareBinary(pDlg->m_FileList[i].strFileName, pDlg->m_FileList[j].strFileName);
			if ( bIsSame )
			{
				pDlg->m_FileList[i].nFindSame = pDlg->m_SameGroup;
				pDlg->m_FileList[j].nFindSame = pDlg->m_SameGroup;
			}
		}

		if ( pDlg->m_FileList[i].nFindSame != UNKNOWN_SAME )
		{
			// 今回新しい塊を見つけた
			pDlg->m_SameGroup++;
		}
	}

	//結果表示
	pDlg->StopProc();
	pDlg->OutputResult();
	return TRUE;
}

