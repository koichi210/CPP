
// MotionCaptureDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "MotionCapture.h"
#include "MotionCaptureDlg.h"
#include "SampleCaptAreaDlg.h"
#include "ManageAvi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMotionCaptureDlg ダイアログ
CMotionCaptureDlg::CMotionCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotionCaptureDlg::IDD, pParent)
	, m_ResizeRectWidth(0)
	, m_ResizeRectHeight(0)
	, m_ResizeEnable(FALSE)
	, m_MousePointRec(FALSE)
	, m_SkipFrame(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 外部から指定するパラメータ
	m_SaveFilename = "c:\\ScreenCapture.avi";
	m_MousePointRec = TRUE;

	m_FrameRate = 20;
	m_TimeoutSec = 10;
	m_SkipFrame = 0;

	m_CaptRectX = 0;
	m_CaptRectY = 0;
	m_CaptRectWidth = 1920;
	m_CaptRectHeight = 1080;
	m_BitmapBpp = 24;

	m_ResizeEnable = FALSE;
	m_ResizeRectWidth = 1024;
	m_ResizeRectHeight = 768;

}

void CMotionCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_SAVE_FILENAME, m_SaveFilename);
	DDX_Text(pDX, IDET_CAPT_RECT_X, m_CaptRectX);
	DDX_Text(pDX, IDET_CAPT_RECT_Y, m_CaptRectY);
	DDX_Text(pDX, IDET_CAPT_RECT_WIDTH, m_CaptRectWidth);
	DDX_Text(pDX, IDET_CAPT_RECT_HEIGHT, m_CaptRectHeight);
	DDX_Text(pDX, IDET_CAPT_BPP, m_BitmapBpp);
	DDV_MinMaxInt(pDX, m_BitmapBpp, 0, 32);
	DDX_Text(pDX, IDET_CAPT_FPS, m_FrameRate);
	DDV_MinMaxInt(pDX, m_FrameRate, 1, 120);
	DDX_Text(pDX, IDET_TIMEOUT_SEC, m_TimeoutSec);
	DDX_Text(pDX, IDET_SKIP_FRAME, m_SkipFrame);
	DDX_Text(pDX, IDET_RESIZE_RECT_WIDTH, m_ResizeRectWidth);
	DDX_Text(pDX, IDET_RESIZE_RECT_HEIGHT, m_ResizeRectHeight);
	DDX_Check(pDX, IDCH_RESIZE, m_ResizeEnable);
	DDX_Check(pDX, IDCH_MOUSE_POINT_REC, m_MousePointRec);
}

BEGIN_MESSAGE_MAP(CMotionCaptureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_RECORD, OnBnClickedRecord)
	ON_BN_CLICKED(IDBT_RECORD_STOP, OnBnClickedRecordStop)
	ON_BN_CLICKED(IDBT_CAPT_AREA_SAMPLE, OnBnClickedSampleCaptArea)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMotionCaptureDlg メッセージ ハンドラー

BOOL CMotionCaptureDlg::OnInitDialog()
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

void CMotionCaptureDlg::OnPaint()
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
HCURSOR CMotionCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMotionCaptureDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//CDialogEx::OnOK();
}

void CMotionCaptureDlg::OnBnClickedRecord()
{
	if (m_Avi.IsExecution() )
	{
		int rt = MessageBox("実行中です。中断しますか？", "Warning", MB_YESNO);
		if ( rt == IDYES )
		{
			OnBnClickedRecordStop();
		}
		return;
	}

	// GUIの値を取得
	UpdateData();

	// 別プロセスがアクセス中かチェック
	{
		CFile cfile;
		if ( cfile.Open(_T(m_SaveFilename), CFile::modeCreate | CFile::modeWrite) )
		{
			cfile.Close();
		}
		else
		{
			MessageBox("別プロセスが使用中です。", "Warning", MB_OK);
			return;
		}
	}

	// パラメータを設定
	m_Avi.SetSaveFileName(m_SaveFilename);
	m_Avi.SetFrameRate(m_FrameRate);
	m_Avi.SetRecordSec(m_TimeoutSec);
	m_Avi.SetSkipFrame(m_SkipFrame);
	m_Avi.SetCaptureRect(m_CaptRectX, m_CaptRectY, m_CaptRectWidth, m_CaptRectHeight);
	m_Avi.SetCaptureQuality(m_BitmapBpp);
	m_Avi.SetResize(m_ResizeEnable, m_ResizeRectWidth, m_ResizeRectHeight);
	m_Avi.SetRecordMousePoint(m_MousePointRec);

	// 記録開始
	m_Avi.StartRecord();
}

void CMotionCaptureDlg::OnBnClickedRecordStop()
{
	// 記録タスクを止める
	m_Avi.StopRecord();
}

void CMotionCaptureDlg::OnBnClickedSampleCaptArea()
{
	UpdateData();
	RECT rt = {m_CaptRectX, m_CaptRectY, m_CaptRectWidth, m_CaptRectHeight};

	SampleCaptAreaDlg dlg(rt, m_BitmapBpp);
	dlg.DoModal();
	return;
}
