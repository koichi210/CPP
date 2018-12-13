#pragma once

#include "stdafx.h"
#include <vfw.h>

#pragma comment (lib, "vfw32.lib")

typedef enum {
	AVI_ERROR_SUCCESS,
	AVI_ERROR_FILE_OPEN,
	AVI_ERROR_CREATE_STREAM,
	AVI_ERROR_CANCEL_COMPRESS,
	AVI_ERROR_CREATE_COMPRESS_STREAM,
s} AVI_ERROR_T;


// CMotionCaptureDlg ダイアログ
class CManageAvi
{
public:
	CManageAvi();	// コンストラクター
	~CManageAvi();	// デストラクタ

	// ユーザーが使用
	void SetSaveFileName(CString Filename);
	void SetCaptureRect(RECT rect);
	void SetCaptureRect(int top, int left, int right, int bottom);
	void SetFrameRate(UINT FrameRate);
	void SetRecordSec(UINT RecordSec);
	void SetSkipFrame(UINT SkipFrame);

	void SetCaptureQuality(UINT BitmpBpp);
	void SetCompress(BOOL CompressEnable);
	void SetResize(BOOL ResizeEnable, UINT ResizeWidth, UINT ResizeHeight);
	void SetRecordMousePoint(BOOL RecordMousePointEnable);
	void StopRecord();
	void StartRecord();
	BOOL IsExecution();

	// プロセスが使用
	void GetAviStreamInfo();
	void GetBitmapInfo();
	AVI_ERROR_T CreateAviFile();
	AVI_ERROR_T SeStreamFormat();
	void Record();

public:
	// 処理系
	BOOL m_IsExecution;	// 実行中フラグ

	// 設定
	CString m_SaveFilename;
	BOOL m_CompressEnable;
	BOOL m_ResizeEnable;
	BOOL m_RecordMousePointEnable;
	POINT m_Resize;
	RECT m_rect;
	UINT m_FrameRate;
	DWORD m_Scale;		// 基本は等倍
	DWORD m_Quality;	// 0～10,000 (-1 is drivers default)
	UINT m_BitmapBpp;	// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)
	UINT m_TimeoutSec;
	UINT m_SkipFrame;

	// プロセスが使用
	AVISTREAMINFO m_aviStreamInfo;
	BITMAPINFO m_bitmapInfo;
	PAVIFILE m_paviFile;
	PAVISTREAM m_paviStream;
	PAVISTREAM m_pcompressAviStream;
	LPVOID pvBits;

};

UINT ProcThread(LPVOID pParam);	// 録画処理
void DrawCursor(HDC hdc, float ScaleX, float ScaleY);	// マウスポインタ描画
