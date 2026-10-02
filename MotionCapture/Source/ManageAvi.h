// ManageAvi.h : 画面を AVI ファイルに録画する

#pragma once

#include <vfw.h>
#include <atomic>

#pragma comment (lib, "vfw32.lib")

enum class AviError
{
	Success,
	FileOpen,
	CreateStream,
	CancelCompress,
	CreateCompressStream,
};

class CManageAvi
{
public:
	CManageAvi() = default;

	// 録画前に呼び出し側が設定する
	void SetSaveFileName(const CString& filename);
	void SetCaptureRect(int top, int left, int right, int bottom);
	void SetFrameRate(UINT frameRate);
	void SetRecordSec(UINT recordSec);
	void SetSkipFrame(UINT skipFrame);
	void SetCaptureQuality(UINT bitmapBpp);
	void SetCompress(BOOL bCompress);
	void SetResize(BOOL bResize, UINT resizeWidth, UINT resizeHeight);
	void SetRecordMousePoint(BOOL bRecordMousePoint);

	// 録画は別スレッドで行う
	void StartRecord();
	void StopRecord();
	BOOL IsExecution() const;

private:
	static UINT RecordThreadProc(LPVOID pParam);

	void InitAviStreamInfo();
	void InitBitmapInfo();
	AviError CreateAviFile();
	AviError SetStreamFormat();
	void Record();
	void ReleaseAvi();

	// 録画スレッドと UI スレッドの両方から読み書きする
	std::atomic<bool> m_bExecuting{ false };

	// 設定
	CString	m_saveFilename;
	BOOL	m_bCompress = TRUE;
	BOOL	m_bResize = FALSE;
	BOOL	m_bRecordMousePoint = FALSE;
	POINT	m_resize = {};
	RECT	m_rect = {};			// right / bottom を録画サイズとして使う
	UINT	m_frameRate = 0;
	DWORD	m_scale = 1;			// 基本は等倍
	DWORD	m_quality = static_cast<DWORD>(-1);	// 0～10,000 (-1 はドライバーの既定値)
	UINT	m_bitmapBpp = 0;		// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)
	UINT	m_timeoutSec = 0;
	UINT	m_skipFrameMs = 0;		// フレーム間の待ち時間（ミリ秒）

	// 録画中に使う
	AVISTREAMINFO	m_aviStreamInfo = {};
	BITMAPINFO		m_bitmapInfo = {};
	PAVIFILE		m_paviFile = nullptr;
	PAVISTREAM		m_paviStream = nullptr;
	PAVISTREAM		m_pcompressAviStream = nullptr;
};
