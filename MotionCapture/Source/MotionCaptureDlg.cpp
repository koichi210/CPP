// MotionCaptureDlg.cpp : メインダイアログ

#include "stdafx.h"
#include "afxdialogex.h"
#include "MotionCapture.h"
#include "MotionCaptureDlg.h"
#include "SampleCaptAreaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMotionCaptureDlg::CMotionCaptureDlg(CWnd* pParent)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotionCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_SAVE_FILENAME, m_saveFilename);
	DDX_Text(pDX, IDET_CAPT_RECT_X, m_captRectX);
	DDX_Text(pDX, IDET_CAPT_RECT_Y, m_captRectY);
	DDX_Text(pDX, IDET_CAPT_RECT_WIDTH, m_captRectWidth);
	DDX_Text(pDX, IDET_CAPT_RECT_HEIGHT, m_captRectHeight);
	DDX_Text(pDX, IDET_CAPT_BPP, m_bitmapBpp);
	DDV_MinMaxInt(pDX, m_bitmapBpp, 0, 32);
	DDX_Text(pDX, IDET_CAPT_FPS, m_frameRate);
	DDV_MinMaxInt(pDX, m_frameRate, 1, 120);
	DDX_Text(pDX, IDET_TIMEOUT_SEC, m_timeoutSec);
	DDX_Text(pDX, IDET_SKIP_FRAME, m_skipFrame);
	DDX_Text(pDX, IDET_RESIZE_RECT_WIDTH, m_resizeRectWidth);
	DDX_Text(pDX, IDET_RESIZE_RECT_HEIGHT, m_resizeRectHeight);
	DDX_Check(pDX, IDCH_RESIZE, m_bResize);
	DDX_Check(pDX, IDCH_MOUSE_POINT_REC, m_bMousePointRec);
}

BEGIN_MESSAGE_MAP(CMotionCaptureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_RECORD, &CMotionCaptureDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDBT_RECORD_STOP, &CMotionCaptureDlg::OnBnClickedRecordStop)
	ON_BN_CLICKED(IDBT_CAPT_AREA_SAMPLE, &CMotionCaptureDlg::OnBnClickedSampleCaptArea)
	ON_BN_CLICKED(IDOK, &CMotionCaptureDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CMotionCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログがメインウィンドウのため自前で描く）
void CMotionCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMotionCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Enter キーでダイアログが閉じないよう、何もしない
void CMotionCaptureDlg::OnBnClickedOk()
{
}

void CMotionCaptureDlg::OnBnClickedRecord()
{
	if (m_avi.IsExecution())
	{
		if (MessageBox(_T("実行中です。中断しますか？"), _T("Warning"), MB_YESNO) == IDYES)
		{
			OnBnClickedRecordStop();
		}
		return;
	}

	UpdateData();

	// 別プロセスが保存先を使っていないか、実際に開いて確かめる
	{
		CFile file;
		if (!file.Open(m_saveFilename, CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox(_T("別プロセスが使用中です。"), _T("Warning"), MB_OK);
			return;
		}
		file.Close();
	}

	m_avi.SetSaveFileName(m_saveFilename);
	m_avi.SetFrameRate(m_frameRate);
	m_avi.SetRecordSec(m_timeoutSec);
	m_avi.SetSkipFrame(m_skipFrame);
	m_avi.SetCaptureRect(m_captRectX, m_captRectY, m_captRectWidth, m_captRectHeight);
	m_avi.SetCaptureQuality(m_bitmapBpp);
	m_avi.SetResize(m_bResize, m_resizeRectWidth, m_resizeRectHeight);
	m_avi.SetRecordMousePoint(m_bMousePointRec);

	m_avi.StartRecord();
}

void CMotionCaptureDlg::OnBnClickedRecordStop()
{
	m_avi.StopRecord();
}

void CMotionCaptureDlg::OnBnClickedSampleCaptArea()
{
	UpdateData();
	RECT rt = { m_captRectX, m_captRectY, m_captRectWidth, m_captRectHeight };

	CSampleCaptAreaDlg dlg(rt, m_bitmapBpp);
	dlg.DoModal();
}
