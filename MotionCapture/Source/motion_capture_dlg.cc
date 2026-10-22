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

	// 別プロセスが保存先を使っていないか、共有なしで実際に開いて確かめる。
	// 既存のファイルは切り詰めない（以前は CFile::modeCreate で空にしていたので、圧縮の選択をキャンセルすると
	// 元の中身が消えていた）。無ければ作って確かめ、閉じると同時に消す（0バイトのファイルを残さない）
	bool CanOpenSaveFile(const CString& filename)
	{
		HANDLE file = ::CreateFile(filename, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (file == INVALID_HANDLE_VALUE && ::GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			file = ::CreateFile(filename, GENERIC_WRITE | DELETE, 0, nullptr, CREATE_NEW,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, nullptr);
		}
		if (file == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		::CloseHandle(file);
		return true;
	}
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
	// 1/4/8 はカラーテーブル（パレット）が要るので扱わない。BI_RGB で画面をそのまま写せる 16/24/32 だけにする
	if (dx->m_bSaveAndValidate && bitmap_bpp_ != 16 && bitmap_bpp_ != 24 && bitmap_bpp_ != 32)
	{
		AfxMessageBox(_T("bpp は 16・24・32 のどれかを入力してください。"), MB_ICONEXCLAMATION);
		dx->Fail();
	}
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
		if (!ConfirmWhileRecording(_T("実行中です。中断して閉じますか？")))
		{
			// 閉じないので、確認している間に届いた結果（タイムアウトなど）はここで出す
			ShowPendingResult();
			return false;
		}
		// 閉じるので、確認している間に届いた結果は出さない（録画がもう終わっていれば、下の待ちはすぐ終わる）
		has_pending_result_ = false;
	}

	StopRecordAndWait();
	return true;
}

// 録画中の確認（はい/いいえ）。答えるまでの間に録画が終わっても、結果のメッセージを確認の上に重ねて出さないよう
// OnRecordFinished に預からせる。預かった結果は、呼び出し側が答えに応じて ShowPendingResult で出すか捨てる
bool MotionCaptureDlg::ConfirmWhileRecording(LPCTSTR text)
{
	confirming_ = true;
	const bool yes = (MessageBox(text, _T("Warning"), MB_YESNO) == IDYES);
	confirming_ = false;
	return yes;
}

void MotionCaptureDlg::ShowPendingResult()
{
	if (!has_pending_result_)
	{
		return;
	}
	has_pending_result_ = false;
	ShowRecordResult(pending_err_, pending_completed_);
}

void MotionCaptureDlg::ShowRecordResult(AviError err, BOOL completed)
{
	// 閉じる途中は終了を妨げないよう、何も表示しない（「保存しました」も、閉じると答えた後には出さない）
	if (closing_)
	{
		return;
	}

	CString caption;
	const CString msg = avi_.MakeResultMessage(err, completed, &caption);
	MessageBox(msg, caption, MB_OK);
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
	const AviError err = static_cast<AviError>(w_param);
	const BOOL completed = static_cast<BOOL>(l_param);

	// 確認を出している最中なら、答えが出るまで預かる
	if (confirming_ && !closing_)
	{
		has_pending_result_ = true;
		pending_err_ = err;
		pending_completed_ = completed;
		return 0;
	}

	ShowRecordResult(err, completed);
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
		if (ConfirmWhileRecording(_T("実行中です。中断しますか？")))
		{
			OnBnClickedRecordStop();
		}
		// 確認している間に録画が終わっていたら、その結果を確認の後に出す
		ShowPendingResult();
		return;
	}

	if (!UpdateData())
	{
		return;
	}

	if (!CanOpenSaveFile(save_filename_))
	{
		MessageBox(_T("別プロセスが使用中です。"), _T("Warning"), MB_OK);
		return;
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
