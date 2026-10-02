// SampleCaptAreaDlg.cpp : 記録領域の確認ダイアログ

#include "stdafx.h"
#include "MotionCapture.h"
#include "SampleCaptAreaDlg.h"
#include "afxdialogex.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace
{
	constexpr int PICTURE_BOX_WIDTH		= 700;
	constexpr int PICTURE_BOX_HEIGHT	= 400;
}

IMPLEMENT_DYNAMIC(CSampleCaptAreaDlg, CDialogEx)

CSampleCaptAreaDlg::CSampleCaptAreaDlg(const RECT& rt, UINT bitmapBpp, CWnd* pParent)
	: CDialogEx(IDD, pParent)
	, m_preview(rt)
	, m_bitmapBpp(bitmapBpp)
{
}

BEGIN_MESSAGE_MAP(CSampleCaptAreaDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CSampleCaptAreaDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	PreView();
}

void CSampleCaptAreaDlg::InitBitmapInfo()
{
	ZeroMemory(&m_bitmapInfo, sizeof(BITMAPINFO));
	BITMAPINFOHEADER& header = m_bitmapInfo.bmiHeader;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = m_preview.right - m_preview.left;
	header.biHeight = m_preview.bottom - m_preview.top;
	header.biPlanes = 1;
	header.biBitCount = static_cast<WORD>(m_bitmapBpp);
	header.biCompression = BI_RGB;	// BI_JPEG は指定できなかった

	if (m_bitmapBpp != 0)
	{
		header.biSizeImage = header.biHeight * ((3 * header.biWidth + 3) / 4) * 4;
	}
	else
	{
		// BI_RGB なら 0 でよい
		header.biSizeImage = 0;
	}
}

void CSampleCaptAreaDlg::ScreenCapture()
{
	HDC hMemDC = ::CreateCompatibleDC(nullptr);
	LPVOID pvBits;

	InitBitmapInfo();
	HBITMAP hMemBitmap = ::CreateDIBSection(nullptr, &m_bitmapInfo, DIB_RGB_COLORS, &pvBits, nullptr, 0);
	HBITMAP hOldBitmap = static_cast<HBITMAP>(::SelectObject(hMemDC, hMemBitmap));

	HDC dcScreen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), nullptr);

	::BitBlt(hMemDC, 0, 0, m_bitmapInfo.bmiHeader.biWidth, m_bitmapInfo.bmiHeader.biHeight, dcScreen, 0, 0, SRCCOPY);

	::SelectObject(hMemDC, hOldBitmap);
	::DeleteObject(hMemBitmap);
	::DeleteDC(hMemDC);
	::DeleteDC(dcScreen);
}

void CSampleCaptAreaDlg::PreView()
{
	const CString samplePath = _T("c:\\Sample.bmp");

	if (!PathFileExists(samplePath))
	{
		MessageBox(_T("ファイルオープンに失敗しました。\n") + samplePath);
		return;
	}

	HBITMAP hBitmap = static_cast<HBITMAP>(::LoadImage(
		AfxGetInstanceHandle(),
		samplePath,
		IMAGE_BITMAP,
		PICTURE_BOX_WIDTH,
		PICTURE_BOX_HEIGHT,
		LR_LOADFROMFILE));

	// ビットマップはピクチャーコントロールに渡したまま残す
	CStatic* pPictureBox = static_cast<CStatic*>(GetDlgItem(IDPC_SAMPLE));
	pPictureBox->SetBitmap(hBitmap);
}

// 24bpp の BMP ファイルとして書き出す
BOOL CSampleCaptAreaDlg::WriteBitmap(LPCTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits)
{
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD dwResult;
	const DWORD dwSizeImage = nHeight * ((3 * nWidth + 3) / 4) * 4;

	BITMAPFILEHEADER bmfHeader = {};
	bmfHeader.bfType    = 0x4D42;	// "BM"
	bmfHeader.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSizeImage;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwResult, nullptr);

	BITMAPINFOHEADER bmiHeader = {};
	bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth       = nWidth;
	bmiHeader.biHeight      = nHeight;
	bmiHeader.biPlanes      = 1;
	bmiHeader.biBitCount    = 24;
	bmiHeader.biSizeImage   = dwSizeImage;
	bmiHeader.biCompression = BI_RGB;

	WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwResult, nullptr);

	WriteFile(hFile, lpBits, dwSizeImage, &dwResult, nullptr);

	CloseHandle(hFile);

	return TRUE;
}
