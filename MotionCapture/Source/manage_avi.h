// manage_avi.h : 画面を AVI ファイルに録画する

#ifndef MOTIONCAPTURE_SOURCE_MANAGE_AVI_H_
#define MOTIONCAPTURE_SOURCE_MANAGE_AVI_H_

#include <vfw.h>
#include <atomic>

#include "worker_threads.h"

#pragma comment (lib, "vfw32.lib")

// 録画スレッドが AVI ファイルを閉じ終えたときに、通知先ウィンドウへ送る
// WPARAM: AviError、LPARAM: 停止されずに最後のフレームまで録画したら TRUE
constexpr UINT kWmAppRecordFinished = WM_APP + 1;

enum class AviError
{
	kSuccess,
	kFileOpen,
	kCreateStream,
	kCancelCompress,
	kCreateCompressStream,
	kCreateBitmap,
};

class ManageAvi
{
public:
	ManageAvi() = default;

	// 録画前に呼び出し側が設定する
	void SetSaveFileName(const CString& filename);
	void SetCaptureRect(int x, int y, int width, int height);
	void SetFrameRate(UINT frame_rate);
	void SetRecordSec(UINT record_sec);
	void SetSkipFrame(UINT skip_frame);
	void SetCaptureQuality(UINT bitmap_bpp);
	void SetCompress(BOOL compress);
	void SetResize(BOOL resize, UINT resize_width, UINT resize_height);
	void SetRecordMousePoint(BOOL record_mouse_point);
	void SetNotifyWindow(HWND notify_wnd);

	// 録画は別スレッドで行う
	void StartRecord();
	void StopRecord();		// 停止を伝えるだけで、待たない
	void WaitForStop();		// 録画スレッドが AVI ファイルを閉じ終わるまで待つ（先に StopRecord を呼ぶこと）
	BOOL IsRecording() const;	// 録画中（圧縮の選択中も含む）
	BOOL IsExecution();		// 録画中、または停止後にファイルを閉じている途中

	// 録画の結果（kWmAppRecordFinished の WPARAM / LPARAM）を、画面に出すメッセージにする
	CString MakeResultMessage(AviError err, BOOL completed, CString* caption) const;

private:
	static UINT RecordThreadProc(LPVOID param);
	void CancelCompressorChoose();

	void InitAviStreamInfo();
	void InitBitmapInfo();
	bool ChooseCompressor(COMPVARS* cv);
	AviError CreateAviFile();
	AviError SetStreamFormat(const COMPVARS& cv);
	DWORD Record();		// 書いたフレーム数を返す（1フレーム分の画像が作れなければ 0）
	void ReleaseAvi();

	// 録画スレッドと UI スレッドの両方から読み書きする
	std::atomic<bool> executing_{ false };
	// 圧縮の選択ダイアログを出している録画スレッドの ID（出していないときは 0）
	std::atomic<DWORD> choosing_thread_id_{ 0 };
	WorkerThreads workers_;
	HWND notify_wnd_ = nullptr;

	// 設定
	CString	save_filename_;
	BOOL	compress_ = TRUE;
	BOOL	resize_ = FALSE;
	BOOL	record_mouse_point_ = FALSE;
	POINT	resize_size_ = {};
	CRect	capture_rect_;		// 画面上の記録領域
	CSize	frame_size_;		// AVI の1フレームの大きさ（リサイズするときはリサイズ後の大きさ）
	UINT	frame_rate_ = 0;
	DWORD	scale_ = 1;			// 基本は等倍
	DWORD	quality_ = static_cast<DWORD>(-1);	// 0～10,000 (-1 はドライバーの既定値)
	UINT	bitmap_bpp_ = 0;		// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)
	UINT	timeout_sec_ = 0;
	UINT	skip_frame_ms_ = 0;		// フレーム間の待ち時間（ミリ秒）

	// 録画中に使う
	AVISTREAMINFO	avi_stream_info_ = {};
	BITMAPINFO		bitmap_info_ = {};
	PAVIFILE		avi_file_ = nullptr;
	PAVISTREAM		avi_stream_ = nullptr;
	PAVISTREAM		compress_avi_stream_ = nullptr;
};

#endif  // MOTIONCAPTURE_SOURCE_MANAGE_AVI_H_
