// motion_capture_dlg.cc : メインダイアログ

#include "stdafx.h"
#include "afxdialogex.h"
#include "motion_capture.h"
#include "motion_capture_dlg.h"
#include "sample_capt_area_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// 記録領域・リサイズ後の幅と高さの上限
	constexpr int kMaxFrameLength = 16384;
}

MotionCaptureDlg::MotionCaptureDlg(CWnd* parent)
	: CDialogEx(IDD, parent)
{
	icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MotionCaptureDlg::DoDataExchange(CDataExchange* dx)
{
	CDialogEx::DoDataExchange(dx);
	DDX_Text(dx, IDET_SAVE_FILENAME, save_filename_);
	DDX_Text(dx, IDET_CAPT_RECT_X, capt_rect_x_);
	DDX_Text(dx, IDET_CAPT_RECT_Y, capt_rect_y_);
	DDX_Text(dx, IDET_CAPT_RECT_WIDTH, capt_rect_width_);
	DDV_MinMaxInt(dx, capt_rect_width_, 1, kMaxFrameLength);
	DDX_Text(dx, IDET_CAPT_RECT_HEIGHT, capt_rect_height_);
	DDV_MinMaxInt(dx, capt_rect_height_, 1, kMaxFrameLength);
	DDX_Text(dx, IDET_CAPT_BPP, bitmap_bpp_);
	DDV_MinMaxInt(dx, bitmap_bpp_, 0, 32);
	DDX_Text(dx, IDET_CAPT_FPS, frame_rate_);
	DDV_MinMaxInt(dx, frame_rate_, 1, 120);
	DDX_Text(dx, IDET_TIMEOUT_SEC, timeout_sec_);
	DDX_Text(dx, IDET_SKIP_FRAME, skip_frame_);
	DDX_Text(dx, IDET_RESIZE_RECT_WIDTH, resize_rect_width_);
	DDV_MinMaxInt(dx, resize_rect_width_, 1, kMaxFrameLength);
	DDX_Text(dx, IDET_RESIZE_RECT_HEIGHT, resize_rect_height_);
	DDV_MinMaxInt(dx, resize_rect_height_, 1, kMaxFrameLength);
	DDX_Check(dx, IDCH_RESIZE, resize_);
	DDX_Check(dx, IDCH_MOUSE_POINT_REC, mouse_point_rec_);
}

BEGIN_MESSAGE_MAP(MotionCaptureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_RECORD, &MotionCaptureDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDBT_RECORD_STOP, &MotionCaptureDlg::OnBnClickedRecordStop)
	ON_BN_CLICKED(IDBT_CAPT_AREA_SAMPLE, &MotionCaptureDlg::OnBnClickedSampleCaptArea)
	ON_BN_CLICKED(IDOK, &MotionCaptureDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL MotionCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	return TRUE;
}

// 最小化時のアイコン描画（ダイアログがメインウィンドウのため自前で描く）
void MotionCaptureDlg::OnPaint()
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

HCURSOR MotionCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_);
}

// Enter キーでダイアログが閉じないよう、何もしない
void MotionCaptureDlg::OnBnClickedOk()
{
}

void MotionCaptureDlg::OnBnClickedRecord()
{
	if (avi_.IsExecution())
	{
		if (MessageBox(_T("実行中です。中断しますか？"), _T("Warning"), MB_YESNO) == IDYES)
		{
			OnBnClickedRecordStop();
		}
		return;
	}

	if (!UpdateData())
	{
		return;
	}

	// 別プロセスが保存先を使っていないか、実際に開いて確かめる
	{
		CFile file;
		if (!file.Open(save_filename_, CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox(_T("別プロセスが使用中です。"), _T("Warning"), MB_OK);
			return;
		}
		file.Close();
	}

	avi_.SetSaveFileName(save_filename_);
	avi_.SetFrameRate(frame_rate_);
	avi_.SetRecordSec(timeout_sec_);
	avi_.SetSkipFrame(skip_frame_);
	avi_.SetCaptureRect(capt_rect_x_, capt_rect_y_, capt_rect_width_, capt_rect_height_);
	avi_.SetCaptureQuality(bitmap_bpp_);
	avi_.SetResize(resize_, resize_rect_width_, resize_rect_height_);
	avi_.SetRecordMousePoint(mouse_point_rec_);

	avi_.StartRecord();
}

void MotionCaptureDlg::OnBnClickedRecordStop()
{
	avi_.StopRecord();
}

void MotionCaptureDlg::OnBnClickedSampleCaptArea()
{
	if (!UpdateData())
	{
		return;
	}

	const CRect area(CPoint(capt_rect_x_, capt_rect_y_), CSize(capt_rect_width_, capt_rect_height_));
	SampleCaptAreaDlg dlg(area);
	dlg.DoModal();
}
