// manage_avi.h : 画面を AVI ファイルに録画する

#ifndef MOTIONCAPTURE_SOURCE_MANAGE_AVI_H_
#define MOTIONCAPTURE_SOURCE_MANAGE_AVI_H_

#include <vfw.h>
#include <atomic>

#pragma comment (lib, "vfw32.lib")

enum class AviError
{
	kSuccess,
	kFileOpen,
	kCreateStream,
	kCancelCompress,
	kCreateCompressStream,
};

class ManageAvi
{
public:
	ManageAvi() = default;

	// 録画前に呼び出し側が設定する
	void SetSaveFileName(const CString& filename);
	void SetCaptureRect(int top, int left, int right, int bottom);
	void SetFrameRate(UINT frame_rate);
	void SetRecordSec(UINT record_sec);
	void SetSkipFrame(UINT skip_frame);
	void SetCaptureQuality(UINT bitmap_bpp);
	void SetCompress(BOOL compress);
	void SetResize(BOOL resize, UINT resize_width, UINT resize_height);
	void SetRecordMousePoint(BOOL record_mouse_point);

	// 録画は別スレッドで行う
	void StartRecord();
	void StopRecord();
	BOOL IsExecution() const;

private:
	static UINT RecordThreadProc(LPVOID param);

	void InitAviStreamInfo();
	void InitBitmapInfo();
	AviError CreateAviFile();
	AviError SetStreamFormat();
	void Record();
	void ReleaseAvi();

	// 録画スレッドと UI スレッドの両方から読み書きする
	std::atomic<bool> executing_{ false };

	// 設定
	CString	save_filename_;
	BOOL	compress_ = TRUE;
	BOOL	resize_ = FALSE;
	BOOL	record_mouse_point_ = FALSE;
	POINT	resize_size_ = {};
	RECT	rect_ = {};			// right / bottom を録画サイズとして使う
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
