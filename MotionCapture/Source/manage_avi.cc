// manage_avi.cc : 画面を AVI ファイルに録画する

#include "stdafx.h"
#include "manage_avi.h"

namespace
{
	// マウスポインタを録画画像に描き込む
	// origin は記録領域の左上（画面座標）、scale_x / scale_y はリサイズの倍率
	void DrawCursor(HDC hdc, POINT origin, float scale_x, float scale_y)
	{
		CURSORINFO cursor_info = {};
		cursor_info.cbSize = sizeof(CURSORINFO);
		if (!GetCursorInfo(&cursor_info) || !(cursor_info.flags & CURSOR_SHOWING))
		{
			return;
		}

		ICONINFO icon_info;
		if (!GetIconInfo(cursor_info.hCursor, &icon_info))
		{
			return;
		}

		const int x = static_cast<int>((cursor_info.ptScreenPos.x - origin.x) * scale_x) - static_cast<int>(icon_info.xHotspot);
		const int y = static_cast<int>((cursor_info.ptScreenPos.y - origin.y) * scale_y) - static_cast<int>(icon_info.yHotspot);
		DrawIcon(hdc, x, y, cursor_info.hCursor);

		// GetIconInfo が作ったビットマップは呼び出し側で解放する
		if (icon_info.hbmMask)
		{
			DeleteObject(icon_info.hbmMask);
		}
		if (icon_info.hbmColor)
		{
			DeleteObject(icon_info.hbmColor);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 設定

void ManageAvi::SetSaveFileName(const CString& filename)
{
	save_filename_ = filename;
}

void ManageAvi::SetCaptureRect(int x, int y, int width, int height)
{
	capture_rect_.SetRect(x, y, x + width, y + height);
}

void ManageAvi::SetFrameRate(UINT frame_rate)
{
	frame_rate_ = frame_rate;
}

void ManageAvi::SetRecordSec(UINT record_sec)
{
	timeout_sec_ = record_sec;
}

void ManageAvi::SetSkipFrame(UINT skip_frame)
{
	skip_frame_ms_ = 1;
	if (skip_frame > 0)
	{
		skip_frame_ms_ = 1000 / skip_frame;
	}
}

void ManageAvi::SetCaptureQuality(UINT bitmap_bpp)
{
	bitmap_bpp_ = bitmap_bpp;
}

void ManageAvi::SetCompress(BOOL compress)
{
	compress_ = compress;
}

void ManageAvi::SetResize(BOOL resize, UINT resize_width, UINT resize_height)
{
	resize_ = resize;
	if (resize_)
	{
		resize_size_.x = resize_width;
		resize_size_.y = resize_height;
	}
}

void ManageAvi::SetRecordMousePoint(BOOL record_mouse_point)
{
	record_mouse_point_ = record_mouse_point;
}

// 録画の結果（kWmAppRecordFinished）の通知先
void ManageAvi::SetNotifyWindow(HWND notify_wnd)
{
	notify_wnd_ = notify_wnd;
}

/////////////////////////////////////////////////////////////////////////////
// 録画の開始・停止

void ManageAvi::StartRecord()
{
	executing_ = true;

	if (!workers_.Start(RecordThreadProc, this))
	{
		executing_ = false;
	}
}

void ManageAvi::StopRecord()
{
	executing_ = false;

	// 圧縮の選択ダイアログを開いたままだと録画スレッドが終われないので、キャンセルで閉じる
	CancelCompressorChoose();
}

// 録画スレッドはファイルを閉じるまで終わらないので、スレッドの終了を待てばファイルは閉じ終わっている。
// 待っている間もメッセージを処理する（録画スレッドからの結果の通知は、待ち終わる前に届く）
void ManageAvi::WaitForStop()
{
	workers_.WaitAll();
}

BOOL ManageAvi::IsRecording() const
{
	return executing_ ? TRUE : FALSE;
}

// 「記録停止」で executing_ は false になるが、録画スレッドはまだファイルを閉じている途中のことがある。
// そこで次の録画を始めたり終了したりしないよう、スレッドが終わるまでは実行中とみなす
BOOL ManageAvi::IsExecution()
{
	return (executing_ || workers_.IsRunning()) ? TRUE : FALSE;
}

CString ManageAvi::MakeResultMessage(AviError err, BOOL completed, CString* caption) const
{
	switch (err)
	{
	case AviError::kSuccess:
		if (completed)
		{
			// 停止されずに最後のフレームまで録画した
			*caption = _T("Warning");
			return _T("タイムアウトが発生しました。\n記録を停止しファイルを保存しました。");
		}
		*caption = _T("Information");
		return _T("ファイルを保存しました。");
	case AviError::kFileOpen:
		*caption = _T("Error");
		return _T("AVIファイルが開けませんでした。") + save_filename_;
	case AviError::kCreateStream:
		*caption = _T("Error");
		return _T("Streamが生成できませんでした。");
	case AviError::kCancelCompress:
		*caption = _T("Error");
		return _T("圧縮設定がキャンセルされました。");
	case AviError::kCreateCompressStream:
		*caption = _T("Error");
		return _T("圧縮Streamが生成できませんでした。");
	case AviError::kCreateBitmap:
		*caption = _T("Error");
		return _T("録画用の画像が作成できませんでした。bpp の設定を確認してください。");
	case AviError::kWriteFrame:
		*caption = _T("Error");
		return _T("AVIファイルに書き込めませんでした。ディスクの空き容量を確認してください。\n") + save_filename_;
	default:
		*caption = _T("Error");
		return CString();
	}
}

// 圧縮の選択ダイアログ（親なしで録画スレッドが出す）が開いていれば、キャンセルで閉じる
void ManageAvi::CancelCompressorChoose()
{
	const DWORD thread_id = choosing_thread_id_;
	if (thread_id == 0)
	{
		return;
	}

	::EnumThreadWindows(thread_id, [](HWND hwnd, LPARAM /*l_param*/) -> BOOL
	{
		// スレッド ID は使い回されるので、念のため自分のプロセスのダイアログだけにする
		DWORD process_id = 0;
		::GetWindowThreadProcessId(hwnd, &process_id);
		TCHAR class_name[16];
		if (process_id == ::GetCurrentProcessId()
			&& ::GetClassName(hwnd, class_name, _countof(class_name)) > 0
			&& _tcscmp(class_name, _T("#32770")) == 0)
		{
			::PostMessage(hwnd, WM_COMMAND, IDCANCEL, 0);
		}
		return TRUE;
	}, 0);
}

/////////////////////////////////////////////////////////////////////////////
// 録画処理

void ManageAvi::InitAviStreamInfo()
{
	ZeroMemory(&avi_stream_info_, sizeof(AVISTREAMINFO));
	avi_stream_info_.fccType = streamtypeVIDEO;
	avi_stream_info_.fccHandler = comptypeDIB;
	avi_stream_info_.dwScale = scale_;							// dwRate / dwScale がフレームレートになる
	avi_stream_info_.dwRate = frame_rate_;
	avi_stream_info_.dwLength = timeout_sec_ * frame_rate_;		// 録画時間(秒) = Length / FrameRate
	avi_stream_info_.dwQuality = quality_;

	// 録画する画像の大きさ。リサイズするときは縮小・拡大した後の大きさになる
	frame_size_ = resize_ ? CSize(resize_size_.x, resize_size_.y) : capture_rect_.Size();
	::SetRect(&avi_stream_info_.rcFrame, 0, 0, frame_size_.cx, frame_size_.cy);
}

void ManageAvi::InitBitmapInfo()
{
	ZeroMemory(&bitmap_info_, sizeof(BITMAPINFO));
	BITMAPINFOHEADER& header = bitmap_info_.bmiHeader;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = frame_size_.cx;
	header.biHeight = frame_size_.cy;
	header.biPlanes = 1;
	header.biBitCount = static_cast<WORD>(bitmap_bpp_);
	header.biCompression = BI_RGB;	// BI_JPEG は指定できなかった

	// 1行は 4 バイト境界にそろえる（bpp が 0 なら 0 になる。BI_RGB なら 0 でよい）
	header.biSizeImage = header.biHeight * ((header.biWidth * bitmap_bpp_ + 31) / 32) * 4;
}

// 圧縮方式を選ばせる（圧縮しないときは何もしない）。選ばれたら true で、cv は呼び出し側が ICCompressorFree する
bool ManageAvi::ChooseCompressor(COMPVARS* cv)
{
	if (!compress_)
	{
		return true;
	}

	*cv = {};
	cv->cbSize = sizeof(COMPVARS);
	cv->dwFlags = ICMF_COMPVARS_VALID;
	cv->fccHandler = comptypeDIB;
	cv->lQ = ICQUALITY_DEFAULT;

	// 選んでいる間に停止されたら StopRecord がダイアログを閉じられるよう、このスレッドを覚えておく。
	// 覚える前に停止されていたら、ダイアログを出さずにキャンセル扱いにする
	choosing_thread_id_ = ::GetCurrentThreadId();
	const BOOL chosen = executing_
		&& ICCompressorChoose(nullptr, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME, &bitmap_info_, nullptr, cv, nullptr);
	choosing_thread_id_ = 0;
	return chosen ? true : false;
}

AviError ManageAvi::CreateAviFile()
{
	if (AVIERR_OK != ::AVIFileOpen(&avi_file_, save_filename_, OF_CREATE | OF_WRITE, nullptr))
	{
		return AviError::kFileOpen;
	}

	if (AVIERR_OK != ::AVIFileCreateStream(avi_file_, &avi_stream_, &avi_stream_info_))
	{
		return AviError::kCreateStream;
	}

	return AviError::kSuccess;
}

// cv は ChooseCompressor で選んだ圧縮方式（圧縮しないときは使わない）
AviError ManageAvi::SetStreamFormat(const COMPVARS& cv)
{
	compress_avi_stream_ = nullptr;
	if (!compress_)
	{
		::AVIStreamSetFormat(avi_stream_, 0, &bitmap_info_, sizeof(BITMAPINFO));
		return AviError::kSuccess;
	}

	avi_stream_info_.fccHandler = cv.fccHandler;

	AVICOMPRESSOPTIONS opt;
	opt.fccType = streamtypeVIDEO;
	opt.fccHandler = cv.fccHandler;
	opt.dwKeyFrameEvery = cv.lKey;
	opt.dwQuality = cv.lQ;
	opt.dwBytesPerSecond = cv.lDataRate;
	opt.dwFlags = (cv.lDataRate > 0 ? AVICOMPRESSF_DATARATE  : 0)
				| (cv.lKey      > 0 ? AVICOMPRESSF_KEYFRAMES : 0);
	opt.lpFormat = nullptr;
	opt.cbFormat = 0;
	opt.lpParms = cv.lpState;
	opt.cbParms = cv.cbState;
	opt.dwInterleaveEvery = 0;

	// opt は cv.lpState を指しているので、cv はストリームを作った後に呼び出し側で解放する
	HRESULT hr = ::AVIMakeCompressedStream(&compress_avi_stream_, avi_stream_, &opt, nullptr);
	if (AVIERR_OK != hr)
	{
		return AviError::kCreateCompressStream;
	}
	::AVIStreamSetFormat(compress_avi_stream_, 0, &bitmap_info_, sizeof(BITMAPINFO));

	return AviError::kSuccess;
}

AviError ManageAvi::Record(DWORD* written_frames)
{
	*written_frames = 0;

	// 1フレーム分の画像（大きさは frame_size_）。
	// bpp が BI_RGB で使えない値（0 や 2 など）だと作れず pv_bits が nullptr になるので、書かずに終わる
	LPVOID pv_bits = nullptr;
	HBITMAP mem_bitmap = ::CreateDIBSection(nullptr, &bitmap_info_, DIB_RGB_COLORS, &pv_bits, nullptr, 0);
	if (mem_bitmap == nullptr || pv_bits == nullptr)
	{
		if (mem_bitmap != nullptr)
		{
			::DeleteObject(mem_bitmap);
		}
		return AviError::kCreateBitmap;
	}

	HDC mem_dc = ::CreateCompatibleDC(nullptr);
	HBITMAP old_bitmap = static_cast<HBITMAP>(::SelectObject(mem_dc, mem_bitmap));

	HDC dc_screen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), nullptr);

	const BITMAPINFOHEADER& header = bitmap_info_.bmiHeader;
	const CRect& src = capture_rect_;
	const float scale_x = static_cast<float>(frame_size_.cx) / src.Width();
	const float scale_y = static_cast<float>(frame_size_.cy) / src.Height();
	// 縮小で色が崩れないよう、間引きで縮める（HALFTONE はきれいだが毎フレームには重い）
	::SetStretchBltMode(mem_dc, COLORONCOLOR);

	PAVISTREAM stream = compress_ ? compress_avi_stream_ : avi_stream_;

	// ファイルを作った以上は、すぐ止められても最初の1フレームは書く（フレームのない AVI を残さない）
	AviError err = AviError::kSuccess;
	DWORD frame_no = 0;
	for (; frame_no == 0 || (frame_no < avi_stream_info_.dwLength && executing_); frame_no++)
	{
		// 画面の記録領域を取り込む
		if (resize_)
		{
			::StretchBlt(mem_dc, 0, 0, frame_size_.cx, frame_size_.cy,
				dc_screen, src.left, src.top, src.Width(), src.Height(), SRCCOPY);
		}
		else
		{
			::BitBlt(mem_dc, 0, 0, frame_size_.cx, frame_size_.cy, dc_screen, src.left, src.top, SRCCOPY);
		}

		if (record_mouse_point_)
		{
			DrawCursor(mem_dc, src.TopLeft(), scale_x, scale_y);
		}

		// ディスクがいっぱいなどで書けなければ、そこで止めてエラーにする（「保存しました」とは出さない）
		if (AVIERR_OK != ::AVIStreamWrite(stream, frame_no, 1, pv_bits, header.biSizeImage, AVIIF_KEYFRAME, nullptr, nullptr))
		{
			err = AviError::kWriteFrame;
			break;
		}
		::Sleep(skip_frame_ms_);
	}

	::SelectObject(mem_dc, old_bitmap);
	::DeleteObject(mem_bitmap);
	::DeleteDC(mem_dc);
	::DeleteDC(dc_screen);

	*written_frames = frame_no;
	return err;
}

void ManageAvi::ReleaseAvi()
{
	if (avi_stream_)
	{
		::AVIStreamRelease(avi_stream_);
		avi_stream_ = nullptr;
	}

	if (compress_avi_stream_)
	{
		::AVIStreamRelease(compress_avi_stream_);
		compress_avi_stream_ = nullptr;
	}

	if (avi_file_)
	{
		::AVIFileRelease(avi_file_);
		avi_file_ = nullptr;
	}
}

UINT ManageAvi::RecordThreadProc(LPVOID param)
{
	ManageAvi* avi = static_cast<ManageAvi*>(param);

	::AVIFileInit();

	avi->InitAviStreamInfo();
	avi->InitBitmapInfo();

	// 圧縮方式は、保存先のファイルを作る前に選ばせる。キャンセル（記録停止・閉じるによるものも含む）なら
	// 保存先には触らない（無ければ作らず、既存のファイルも切り詰めない）
	COMPVARS cv = {};
	AviError err = avi->ChooseCompressor(&cv) ? AviError::kSuccess : AviError::kCancelCompress;

	bool created_file = false;
	if (err == AviError::kSuccess)
	{
		// 録画が成立しなかったときに消してよいのは、この録画で新しく作ったファイルだけ
		created_file = ::GetFileAttributes(avi->save_filename_) == INVALID_FILE_ATTRIBUTES;
		err = avi->CreateAviFile();
		if (err == AviError::kSuccess)
		{
			err = avi->SetStreamFormat(cv);
		}
		if (avi->compress_)
		{
			::ICCompressorFree(&cv);
		}
	}

	DWORD written_frames = 0;
	BOOL completed = FALSE;
	if (err == AviError::kSuccess)
	{
		err = avi->Record(&written_frames);
		completed = avi->executing_ ? TRUE : FALSE;
	}

	// AVI ファイルはヘッダーと索引を閉じるときに書くので、結果を知らせる前に閉じる
	// （以前はメッセージボックスを閉じるまで開いたままで、その間にアプリを閉じるとファイルが壊れた）
	avi->ReleaseAvi();
	::AVIFileExit();

	// ストリームが作れなかったなどで1フレームも書けなかったら、この録画で作った（中身のない）ファイルは消す
	if (created_file && written_frames == 0)
	{
		::DeleteFile(avi->save_filename_);
	}

	avi->executing_ = false;

	// メッセージは UI スレッドに出させ、このスレッドはすぐ終わる
	// （ここでメッセージボックスを出すと、閉じるまでスレッドが終われず、アプリの終了で待ちきれないため）
	if (avi->notify_wnd_ != nullptr)
	{
		::PostMessage(avi->notify_wnd_, kWmAppRecordFinished, static_cast<WPARAM>(err), completed);
	}

	return TRUE;
}
