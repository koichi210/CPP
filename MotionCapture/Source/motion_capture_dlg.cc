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

	// 終了時に録画スレッドを待っている間、圧縮の選択ダイアログが遅れて開いても閉じられるよう、停止を伝え直す
	constexpr UINT_PTR kStopRecordTimerId = 1;
	constexpr UINT kStopRecordIntervalMs = 100;
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
	ON_WM_ENDSESSION()
	ON_WM_TIMER()
	ON_MESSAGE(kWmAppRecordFinished, &MotionCaptureDlg::OnRecordFinished)
END_MESSAGE_MAP()

BOOL MotionCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_, TRUE);
	SetIcon(icon_, FALSE);

	avi_.SetNotifyWindow(GetSafeHwnd());

	return TRUE;
}

// Esc・閉じるボタン。録画中のまま閉じると AVI ファイルが壊れるので、止めてファイルを閉じ終わってから閉じる
void MotionCaptureDlg::OnCancel()
{
	if (StopRecordForClose())
	{
		CDialogEx::OnCancel();
	}
}

bool MotionCaptureDlg::StopRecordForClose()
{
	// 停止後にファイルを閉じている途中なら、確認はせずに待つだけにする
	if (avi_.IsRecording())
	{
		if (MessageBox(_T("実行中です。中断して閉じますか？"), _T("Warning"), MB_YESNO) != IDYES)
		{
			return false;
		}
	}

	StopRecordAndWait();
	return true;
}

// シャットダウン・ログオフでは、この後すぐプロセスごと終了させられるので、確認せずに止めてファイルを閉じさせる
void MotionCaptureDlg::OnEndSession(BOOL ending)
{
	if (ending)
	{
		StopRecordAndWait();
	}
	CDialogEx::OnEndSession(ending);
}

void MotionCaptureDlg::StopRecordAndWait()
{
	closing_ = true;
	avi_.StopRecord();

	// 圧縮の選択ダイアログが開く直前に止めた場合は StopRecord で閉じきれないので、待っている間も伝え直す
	SetTimer(kStopRecordTimerId, kStopRecordIntervalMs, nullptr);
	EnableWindow(FALSE);	// 待っている間に操作させない
	avi_.WaitForStop();
	EnableWindow(TRUE);
	KillTimer(kStopRecordTimerId);
}

void MotionCaptureDlg::OnTimer(UINT_PTR id_event)
{
	if (id_event == kStopRecordTimerId)
	{
		avi_.StopRecord();
		return;
	}
	CDialogEx::OnTimer(id_event);
}

// 録画スレッドはファイルを閉じてから知らせてくるので、ここでは結果を表示するだけでよい
LRESULT MotionCaptureDlg::OnRecordFinished(WPARAM w_param, LPARAM l_param)
{
	// 閉じる途中は終了を妨げないよう、何も表示しない（「保存しました」も、閉じると答えた後には出さない）
	if (closing_)
	{
		return 0;
	}

	CString caption;
	const CString msg = avi_.MakeResultMessage(static_cast<AviError>(w_param), static_cast<BOOL>(l_param), &caption);
	MessageBox(msg, caption, MB_OK);
	return 0;
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
