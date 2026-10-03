// manage_avi.cc : 画面を AVI ファイルに録画する

#include "stdafx.h"
#include "manage_avi.h"

namespace
{
	// マウスポインタを録画画像に描き込む（Scale はリサイズの倍率）
	void DrawCursor(HDC hdc, float scaleX, float scaleY)
	{
		CURSORINFO cursorInfo;
		cursorInfo.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&cursorInfo);

		ICONINFO iconInfo;
		if (!GetIconInfo(cursorInfo.hCursor, &iconInfo))
		{
			return;
		}

		int x = static_cast<int>(cursorInfo.ptScreenPos.x * scaleX) - iconInfo.xHotspot;
		int y = static_cast<int>(cursorInfo.ptScreenPos.y * scaleY) - iconInfo.yHotspot;
		DrawIcon(hdc, x, y, cursorInfo.hCursor);

		// GetIconInfo が作ったビットマップは呼び出し側で解放する
		if (iconInfo.hbmMask)
		{
			DeleteObject(iconInfo.hbmMask);
		}
		if (iconInfo.hbmColor)
		{
			DeleteObject(iconInfo.hbmColor);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 設定

void CManageAvi::SetSaveFileName(const CString& filename)
{
	m_saveFilename = filename;
}

void CManageAvi::SetCaptureRect(int top, int left, int right, int bottom)
{
	m_rect.top = top;
	m_rect.left = left;
	m_rect.right = right;
	m_rect.bottom = bottom;
}

void CManageAvi::SetFrameRate(UINT frameRate)
{
	m_frameRate = frameRate;
}

void CManageAvi::SetRecordSec(UINT recordSec)
{
	m_timeoutSec = recordSec;
}

void CManageAvi::SetSkipFrame(UINT skipFrame)
{
	m_skipFrameMs = 1;
	if (skipFrame > 0)
	{
		m_skipFrameMs = 1000 / skipFrame;
	}
}

void CManageAvi::SetCaptureQuality(UINT bitmapBpp)
{
	m_bitmapBpp = bitmapBpp;
}

void CManageAvi::SetCompress(BOOL bCompress)
{
	m_bCompress = bCompress;
}

void CManageAvi::SetResize(BOOL bResize, UINT resizeWidth, UINT resizeHeight)
{
	m_bResize = bResize;
	if (m_bResize)
	{
		m_resize.x = resizeWidth;
		m_resize.y = resizeHeight;
	}
}

void CManageAvi::SetRecordMousePoint(BOOL bRecordMousePoint)
{
	m_bRecordMousePoint = bRecordMousePoint;
}

/////////////////////////////////////////////////////////////////////////////
// 録画の開始・停止

void CManageAvi::StartRecord()
{
	m_bExecuting = true;

	AfxBeginThread(RecordThreadProc, this);
}

void CManageAvi::StopRecord()
{
	m_bExecuting = false;
}

BOOL CManageAvi::IsExecution() const
{
	return m_bExecuting ? TRUE : FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 録画処理

void CManageAvi::InitAviStreamInfo()
{
	ZeroMemory(&m_aviStreamInfo, sizeof(AVISTREAMINFO));
	m_aviStreamInfo.fccType = streamtypeVIDEO;
	m_aviStreamInfo.fccHandler = comptypeDIB;
	m_aviStreamInfo.dwScale = m_scale;							// dwRate / dwScale がフレームレートになる
	m_aviStreamInfo.dwRate = m_frameRate;
	m_aviStreamInfo.dwLength = m_timeoutSec * m_frameRate;		// 録画時間(秒) = Length / FrameRate
	m_aviStreamInfo.dwQuality = m_quality;
	m_aviStreamInfo.rcFrame = m_rect;
}

void CManageAvi::InitBitmapInfo()
{
	ZeroMemory(&m_bitmapInfo, sizeof(BITMAPINFO));
	BITMAPINFOHEADER& header = m_bitmapInfo.bmiHeader;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = m_aviStreamInfo.rcFrame.right;
	header.biHeight = m_aviStreamInfo.rcFrame.bottom;
	header.biPlanes = 1;
	header.biBitCount = static_cast<WORD>(m_bitmapBpp);
	header.biCompression = BI_RGB;	// BI_JPEG は指定できなかった

	if (m_bitmapBpp != 0)
	{
		// 1行は 4 バイト境界にそろえる
		header.biSizeImage = header.biHeight * ((header.biWidth * m_bitmapBpp + 31) / 32) * 4;
	}
	else
	{
		// BI_RGB なら 0 でよい
		header.biSizeImage = 0;
	}
}

AviError CManageAvi::CreateAviFile()
{
	if (AVIERR_OK != ::AVIFileOpen(&m_paviFile, m_saveFilename, OF_CREATE | OF_WRITE, nullptr))
	{
		return AviError::FileOpen;
	}

	if (AVIERR_OK != ::AVIFileCreateStream(m_paviFile, &m_paviStream, &m_aviStreamInfo))
	{
		return AviError::CreateStream;
	}

	return AviError::Success;
}

AviError CManageAvi::SetStreamFormat()
{
	m_pcompressAviStream = nullptr;
	if (!m_bCompress)
	{
		::AVIStreamSetFormat(m_paviStream, 0, &m_bitmapInfo, sizeof(BITMAPINFO));
		return AviError::Success;
	}

	COMPVARS cv = {};
	cv.cbSize = sizeof(COMPVARS);
	cv.dwFlags = ICMF_COMPVARS_VALID;
	cv.fccHandler = comptypeDIB;
	cv.lQ = ICQUALITY_DEFAULT;
	if (!ICCompressorChoose(nullptr, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME, &m_bitmapInfo, nullptr, &cv, nullptr))
	{
		return AviError::CancelCompress;
	}

	m_aviStreamInfo.fccHandler = cv.fccHandler;

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
	HRESULT hr = ::AVIMakeCompressedStream(&m_pcompressAviStream, m_paviStream, &opt, nullptr);
	::ICCompressorFree(&cv);
	if (AVIERR_OK != hr)
	{
		return AviError::CreateCompressStream;
	}
	::AVIStreamSetFormat(m_pcompressAviStream, 0, &m_bitmapInfo, sizeof(BITMAPINFO));

	return AviError::Success;
}

void CManageAvi::Record()
{
	HDC hMemDC = ::CreateCompatibleDC(nullptr);
	LPVOID pvBits;

	// 既知の問題: リサイズ有効時、画面外のマウスポインタまで拾ってしまう。
	//             m_bitmapInfo の width と height を見直す必要あり。
	HBITMAP hMemBitmap = ::CreateDIBSection(nullptr, &m_bitmapInfo, DIB_RGB_COLORS, &pvBits, nullptr, 0);
	HBITMAP hOldBitmap = static_cast<HBITMAP>(::SelectObject(hMemDC, hMemBitmap));

	HDC dcScreen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), nullptr);

	const BITMAPINFOHEADER& header = m_bitmapInfo.bmiHeader;
	float scaleX = 1.0;
	float scaleY = 1.0;
	if (m_bResize)
	{
		scaleX = static_cast<float>(m_resize.x) / header.biWidth;
		scaleY = static_cast<float>(m_resize.y) / header.biHeight;
	}

	PAVISTREAM pStream = m_bCompress ? m_pcompressAviStream : m_paviStream;

	for (DWORD dwFrameNo = 0; dwFrameNo < m_aviStreamInfo.dwLength; dwFrameNo++)
	{
		if (!m_bExecuting)
		{
			break;
		}

		if (m_bResize)
		{
			::StretchBlt(hMemDC, 0, 0, m_resize.x, m_resize.y,
				dcScreen, 0, 0, header.biWidth, header.biHeight, SRCCOPY);
		}
		else
		{
			::BitBlt(hMemDC, 0, 0, header.biWidth, header.biHeight, dcScreen, 0, 0, SRCCOPY);
		}

		if (m_bRecordMousePoint)
		{
			DrawCursor(hMemDC, scaleX, scaleY);
		}

		::AVIStreamWrite(pStream, dwFrameNo, 1, pvBits, header.biSizeImage, AVIIF_KEYFRAME, nullptr, nullptr);
		::Sleep(m_skipFrameMs);
	}

	::SelectObject(hMemDC, hOldBitmap);
	::DeleteObject(hMemBitmap);
	::DeleteDC(hMemDC);
	::DeleteDC(dcScreen);
}

void CManageAvi::ReleaseAvi()
{
	if (m_paviStream)
	{
		::AVIStreamRelease(m_paviStream);
		m_paviStream = nullptr;
	}

	if (m_pcompressAviStream)
	{
		::AVIStreamRelease(m_pcompressAviStream);
		m_pcompressAviStream = nullptr;
	}

	if (m_paviFile)
	{
		::AVIFileRelease(m_paviFile);
		m_paviFile = nullptr;
	}
}

UINT CManageAvi::RecordThreadProc(LPVOID pParam)
{
	CManageAvi* pAvi = static_cast<CManageAvi*>(pParam);

	::AVIFileInit();

	pAvi->InitAviStreamInfo();

	AviError err = pAvi->CreateAviFile();
	if (err == AviError::Success)
	{
		pAvi->InitBitmapInfo();
		err = pAvi->SetStreamFormat();
	}

	if (err == AviError::Success)
	{
		pAvi->Record();

		if (pAvi->m_bExecuting)
		{
			// 停止されずに最後のフレームまで録画した
			MessageBox(nullptr, _T("タイムアウトが発生しました。\n記録を停止しファイルを保存しました。"), _T("Warning"), MB_OK);
		}
		else
		{
			MessageBox(nullptr, _T("ファイルを保存しました。"), _T("Infomation"), MB_OK);
		}
	}
	else
	{
		CString msg;
		switch (err)
		{
		case AviError::FileOpen:
			msg = _T("AVIファイルが開けませんでした。") + pAvi->m_saveFilename;
			break;
		case AviError::CreateStream:
			msg = _T("Streamが生成できませんでした。");
			break;
		case AviError::CancelCompress:
			msg = _T("圧縮設定がキャンセルされました。");
			break;
		case AviError::CreateCompressStream:
			msg = _T("圧縮Streamが生成できませんでした。");
			break;
		default:
			break;
		}

		MessageBox(nullptr, msg, _T("Error"), MB_OK);
	}

	pAvi->ReleaseAvi();
	::AVIFileExit();

	pAvi->m_bExecuting = false;

	return TRUE;
}
