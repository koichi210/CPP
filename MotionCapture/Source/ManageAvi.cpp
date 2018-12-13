
// ManageAvi.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ManageAvi.h"


CManageAvi::CManageAvi()
{
	m_Quality = -1;
	m_Scale = 1;
	m_IsExecution = FALSE;

	m_CompressEnable = TRUE;
	m_ResizeEnable = FALSE;
	m_RecordMousePointEnable = FALSE;
	m_FrameRate = 0;
	m_BitmapBpp = 0;
	m_TimeoutSec = 0;
	m_SkipFrame = 0;
	m_SaveFilename = "";

	ZeroMemory(&m_Resize, sizeof(POINT));
	ZeroMemory(&m_rect, sizeof(RECT));
}

CManageAvi::~CManageAvi()
{
}

//	ユーザーが使用 /////////////////////////////////////////////////////////////
void CManageAvi::SetSaveFileName(CString Filename)
{
	m_SaveFilename = Filename;
}	

void CManageAvi::SetCaptureRect(RECT rect)
{
	m_rect = rect;
}

void CManageAvi::SetCaptureRect(int top, int left, int right, int bottom)
{
	m_rect.top = top;
	m_rect.left = left;
	m_rect.right = right;
	m_rect.bottom = bottom;
}

void CManageAvi::SetFrameRate(UINT FrameRate)
{
	m_FrameRate = FrameRate;
}

void CManageAvi::SetRecordSec(UINT RecordSec)
{
	m_TimeoutSec = RecordSec;
}

void CManageAvi::SetSkipFrame(UINT SkipFrame)
{
	m_SkipFrame = 1;
	if ( SkipFrame > 0 )
	{
		m_SkipFrame = 1000 / SkipFrame;
	}
}

void CManageAvi::SetCaptureQuality(UINT BitmpBpp)
{
	m_BitmapBpp = BitmpBpp;
}

void CManageAvi::SetCompress(BOOL CompressEnable)
{
	m_CompressEnable = CompressEnable;
}

void CManageAvi::SetResize(BOOL ResizeEnable, UINT ResizeWidth, UINT ResizeHeight)
{
	m_ResizeEnable = ResizeEnable;
	if ( m_ResizeEnable )
	{
		m_Resize.x = ResizeWidth;
		m_Resize.y = ResizeHeight;
	}
}

void CManageAvi::SetRecordMousePoint(BOOL RecordMousePointEnable)
{
	m_RecordMousePointEnable = RecordMousePointEnable;
}

void CManageAvi::StopRecord()
{
	m_IsExecution = FALSE;
}

void CManageAvi::StartRecord()
{
	// 実行フラグを設定
	m_IsExecution = TRUE;

	AfxBeginThread(ProcThread, this);
}

BOOL CManageAvi::IsExecution()
{
	return m_IsExecution;
}

// プロセスが使用 //////////////////////////////////////////////////////////////
void CManageAvi::GetAviStreamInfo()
{
	ZeroMemory(&m_aviStreamInfo, sizeof(AVISTREAMINFO));
	m_aviStreamInfo.fccType = streamtypeVIDEO;
	m_aviStreamInfo.fccHandler = comptypeDIB;
	m_aviStreamInfo.dwScale = m_Scale;							// For video streams, this rate should be the frame rate.
	m_aviStreamInfo.dwRate = m_FrameRate;						//フレームレート
	m_aviStreamInfo.dwLength = m_TimeoutSec * m_FrameRate;		// RecordTime = Length / FrameRate (sec)
	m_aviStreamInfo.dwQuality = m_Quality;
	m_aviStreamInfo.rcFrame = m_rect; 
}

void CManageAvi::GetBitmapInfo()
{
	ZeroMemory(&m_bitmapInfo, sizeof(BITMAPINFO));
	m_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitmapInfo.bmiHeader.biWidth = m_aviStreamInfo.rcFrame.right;
	m_bitmapInfo.bmiHeader.biHeight = m_aviStreamInfo.rcFrame.bottom;
	m_bitmapInfo.bmiHeader.biPlanes = 1;
	m_bitmapInfo.bmiHeader.biBitCount = m_BitmapBpp;
	m_bitmapInfo.bmiHeader.biCompression = BI_RGB;	// TODO：BI_JPEGに設定できない

	if ( m_BitmapBpp != 0 )
	{
		m_bitmapInfo.bmiHeader.biSizeImage = m_bitmapInfo.bmiHeader.biHeight * ((3 * m_bitmapInfo.bmiHeader.biWidth + 3) / 4) * 4;
	}
	else
	{
		// This may be set to zero for BI_RGB bitmaps
		// If biCompression is BI_JPEG or BI_PNG, biSizeImage indicates the size of the JPEG or PNG image buffer, respectively.
		m_bitmapInfo.bmiHeader.biSizeImage = 0;
	}
}

AVI_ERROR_T CManageAvi::CreateAviFile()
{
	if (AVIERR_OK != ::AVIFileOpen(&m_paviFile, m_SaveFilename, OF_CREATE | OF_WRITE, NULL))
	{
		return AVI_ERROR_FILE_OPEN;
	}

	if (AVIERR_OK != ::AVIFileCreateStream(m_paviFile, &m_paviStream, &m_aviStreamInfo))
	{
		return AVI_ERROR_CREATE_STREAM;
	}

	return AVI_ERROR_SUCCESS;
}

AVI_ERROR_T CManageAvi::SeStreamFormat()
{
	ZeroMemory(&m_pcompressAviStream, sizeof(PAVISTREAM));
	if ( m_CompressEnable )
	{
		COMPVARS cv;
		memset(&cv, 0, sizeof(COMPVARS));
		cv.cbSize = sizeof(COMPVARS);
		cv.dwFlags = ICMF_COMPVARS_VALID;
		cv.fccHandler = comptypeDIB;
		cv.lQ = ICQUALITY_DEFAULT;
		if ((ICCompressorChoose(NULL, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME, &m_bitmapInfo, NULL, &cv, NULL)) == FALSE)
		{
			return AVI_ERROR_CANCEL_COMPRESS;
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
		opt.lpFormat = NULL;
		opt.cbFormat = 0;
		opt.lpParms = cv.lpState;
		opt.cbParms = cv.cbState;
		opt.dwInterleaveEvery = 0;
		::ICCompressorFree(&cv);

		if (AVIERR_OK != ::AVIMakeCompressedStream(&m_pcompressAviStream, m_paviStream, &opt, NULL))
		{
			return AVI_ERROR_CREATE_COMPRESS_STREAM;
		}
		::AVIStreamSetFormat(m_pcompressAviStream, 0, &m_bitmapInfo, sizeof(BITMAPINFO));
	}
	else
	{
		::AVIStreamSetFormat(m_paviStream, 0, &m_bitmapInfo, sizeof(BITMAPINFO));
	}

	return AVI_ERROR_SUCCESS;
}

void CManageAvi::Record()
{
	HDC hMemDC = ::CreateCompatibleDC(NULL);
	LPVOID pvBits;

	// TODO：リサイズ有効時、画面外のマウスポインタまで拾ってしまう。
	//       m_bitmapInfoのwidthとheightを見直す必要あり。
	HBITMAP hMemBitmap = ::CreateDIBSection(NULL, &m_bitmapInfo, DIB_RGB_COLORS, &pvBits, NULL, 0);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

	HDC dcScreen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), NULL);

	float ScaleX = 1.0;
	float ScaleY = 1.0;
	if ( m_ResizeEnable )
	{
		ScaleX = (float)m_Resize.x / m_bitmapInfo.bmiHeader.biWidth;
		ScaleY = (float)m_Resize.y / m_bitmapInfo.bmiHeader.biHeight;
	}

	// キャプチャ
	for (DWORD dwFrameNo = 0; dwFrameNo < m_aviStreamInfo.dwLength; dwFrameNo++)
	{
		// 停止要求がきたら終了
		if ( m_IsExecution == FALSE )
		{
			break;
		}

		if ( m_ResizeEnable )
		{
			// リサイズ
			::StretchBlt(hMemDC,					// コピー先のデバイスコンテキストのハンドル
				0,									// コピー先長方形の左上隅の x 座標
				0,									// コピー先長方形の左上隅の y 座標
				m_Resize.x,							// コピー先長方形の幅
				m_Resize.y,							// コピー先長方形の高さ
				dcScreen,							// コピー元のデバイスコンテキストのハンドル
				0,									// コピー元長方形の左上隅の x 座標
				0,									// コピー元長方形の左上隅の y 座標
				m_bitmapInfo.bmiHeader.biWidth,		// コピー元長方形の幅
				m_bitmapInfo.bmiHeader.biHeight,	// コピー元長方形の高さ
				SRCCOPY);							// ラスタオペレーションコード
		}
		else
		{
			::BitBlt(hMemDC, 0, 0, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight, dcScreen, 0, 0, SRCCOPY);
		}

		if ( m_RecordMousePointEnable )
		{
			DrawCursor(hMemDC, ScaleX, ScaleY);
		}

		if ( m_CompressEnable )
		{
			::AVIStreamWrite(m_pcompressAviStream, dwFrameNo, 1, pvBits, m_bitmapInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
		}
		else
		{
			::AVIStreamWrite(m_paviStream, dwFrameNo, 1, pvBits, m_bitmapInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
		}
		::Sleep(m_SkipFrame);
	}

	::SelectObject(hMemDC, hOldBitmap);
	::DeleteObject(hMemBitmap);
	::DeleteDC(hMemDC);
	::DeleteDC(dcScreen);
}

// プロセス ////////////////////////////////////////////////////////////////////
UINT ProcThread(LPVOID pParam)
{
	CManageAvi* Avi=(CManageAvi*)pParam;
	AVI_ERROR_T Success = AVI_ERROR_SUCCESS;

	try
	{
		::AVIFileInit();

		Avi->GetAviStreamInfo();

		Success = Avi->CreateAviFile();
		if ( Success != AVI_ERROR_SUCCESS )
		{
			throw Success;
		}

		Avi->GetBitmapInfo();

		Success = Avi->SeStreamFormat();
		if ( Success != AVI_ERROR_SUCCESS )
		{
			throw Success;
		}

		// メイン処理
		Avi->Record();
		
		if ( Avi->IsExecution() )
		{
			// タイムアウト
			MessageBox(NULL, "タイムアウトが発生しました。\n記録を停止しファイルを保存しました。", "Warning", MB_OK);
		}
		else
		{
			// 停止ボタン
			MessageBox(NULL, "ファイルを保存しました。", "Infomation", MB_OK);
		}

	}
	catch(AVI_ERROR_T err)
	{
		CString msg;
		switch (err)
		{
		case AVI_ERROR_FILE_OPEN :
			msg = "AVIファイルが開けませんでした。" + Avi->m_SaveFilename;
			break;

		case 	AVI_ERROR_CREATE_STREAM :
			msg = "Streamが生成できませんでした。";
			break;

		case 	AVI_ERROR_CANCEL_COMPRESS :
			msg = "圧縮設定がキャンセルされました。";
			break;

		case 	AVI_ERROR_CREATE_COMPRESS_STREAM :
			msg = "圧縮Streamが生成できませんでした。";
			break;

		default :
			break;
		}

		MessageBox(NULL, msg, "Error", MB_OK);
	}

	//finally
	{
		if ( Avi->m_paviStream )
		{
			::AVIStreamRelease(Avi->m_paviStream);
		}

		if ( Avi->m_pcompressAviStream )
		{
			::AVIStreamRelease(Avi->m_pcompressAviStream);
		}

		if ( Avi->m_paviFile )
		{
			::AVIFileRelease(Avi->m_paviFile);
		}
		::AVIFileExit();

		// 実行終了
		Avi->m_IsExecution = FALSE;
	}

	return TRUE;
}

void DrawCursor(HDC hdc, float ScaleX, float ScaleY)
{
	CURSORINFO cursorInfo;
	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);

	ICONINFO   iconInfo;
	GetIconInfo(cursorInfo.hCursor, &iconInfo);

	int x = (int)(cursorInfo.ptScreenPos.x * ScaleX) - iconInfo.xHotspot;
	int y = (int)(cursorInfo.ptScreenPos.y * ScaleY) - iconInfo.yHotspot;
	DrawIcon(hdc, x, y, cursorInfo.hCursor);
}
