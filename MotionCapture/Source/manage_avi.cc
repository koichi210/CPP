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

/////////////////////////////////////////////////////////////////////////////
// 録画の開始・停止

void ManageAvi::StartRecord()
{
	executing_ = true;

	AfxBeginThread(RecordThreadProc, this);
}

void ManageAvi::StopRecord()
{
	executing_ = false;
}

BOOL ManageAvi::IsExecution() const
{
	return executing_ ? TRUE : FALSE;
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

AviError ManageAvi::SetStreamFormat()
{
	compress_avi_stream_ = nullptr;
	if (!compress_)
	{
		::AVIStreamSetFormat(avi_stream_, 0, &bitmap_info_, sizeof(BITMAPINFO));
		return AviError::kSuccess;
	}

	COMPVARS cv = {};
	cv.cbSize = sizeof(COMPVARS);
	cv.dwFlags = ICMF_COMPVARS_VALID;
	cv.fccHandler = comptypeDIB;
	cv.lQ = ICQUALITY_DEFAULT;
	if (!ICCompressorChoose(nullptr, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME, &bitmap_info_, nullptr, &cv, nullptr))
	{
		return AviError::kCancelCompress;
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

	// opt は cv.lpState を指しているので、ストリームを作ってから解放する
	HRESULT hr = ::AVIMakeCompressedStream(&compress_avi_stream_, avi_stream_, &opt, nullptr);
	::ICCompressorFree(&cv);
	if (AVIERR_OK != hr)
	{
		return AviError::kCreateCompressStream;
	}
	::AVIStreamSetFormat(compress_avi_stream_, 0, &bitmap_info_, sizeof(BITMAPINFO));

	return AviError::kSuccess;
}

void ManageAvi::Record()
{
	HDC mem_dc = ::CreateCompatibleDC(nullptr);
	LPVOID pv_bits;

	// 1フレーム分の画像（大きさは frame_size_）
	HBITMAP mem_bitmap = ::CreateDIBSection(nullptr, &bitmap_info_, DIB_RGB_COLORS, &pv_bits, nullptr, 0);
	HBITMAP old_bitmap = static_cast<HBITMAP>(::SelectObject(mem_dc, mem_bitmap));

	HDC dc_screen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), nullptr);

	const BITMAPINFOHEADER& header = bitmap_info_.bmiHeader;
	const CRect& src = capture_rect_;
	const float scale_x = static_cast<float>(frame_size_.cx) / src.Width();
	const float scale_y = static_cast<float>(frame_size_.cy) / src.Height();
	// 縮小で色が崩れないよう、間引きで縮める（HALFTONE はきれいだが毎フレームには重い）
	::SetStretchBltMode(mem_dc, COLORONCOLOR);

	PAVISTREAM stream = compress_ ? compress_avi_stream_ : avi_stream_;

	for (DWORD frame_no = 0; frame_no < avi_stream_info_.dwLength && executing_; frame_no++)
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

		::AVIStreamWrite(stream, frame_no, 1, pv_bits, header.biSizeImage, AVIIF_KEYFRAME, nullptr, nullptr);
		::Sleep(skip_frame_ms_);
	}

	::SelectObject(mem_dc, old_bitmap);
	::DeleteObject(mem_bitmap);
	::DeleteDC(mem_dc);
	::DeleteDC(dc_screen);
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

	AviError err = avi->CreateAviFile();
	if (err == AviError::kSuccess)
	{
		avi->InitBitmapInfo();
		err = avi->SetStreamFormat();
	}

	if (err == AviError::kSuccess)
	{
		avi->Record();

		if (avi->executing_)
		{
			// 停止されずに最後のフレームまで録画した
			MessageBox(nullptr, _T("タイムアウトが発生しました。\n記録を停止しファイルを保存しました。"), _T("Warning"), MB_OK);
		}
		else
		{
			MessageBox(nullptr, _T("ファイルを保存しました。"), _T("Information"), MB_OK);
		}
	}
	else
	{
		CString msg;
		switch (err)
		{
		case AviError::kFileOpen:
			msg = _T("AVIファイルが開けませんでした。") + avi->save_filename_;
			break;
		case AviError::kCreateStream:
			msg = _T("Streamが生成できませんでした。");
			break;
		case AviError::kCancelCompress:
			msg = _T("圧縮設定がキャンセルされました。");
			break;
		case AviError::kCreateCompressStream:
			msg = _T("圧縮Streamが生成できませんでした。");
			break;
		default:
			break;
		}

		MessageBox(nullptr, msg, _T("Error"), MB_OK);
	}

	avi->ReleaseAvi();
	::AVIFileExit();

	avi->executing_ = false;

	return TRUE;
}
