// sample_capt_area_dlg.cc : 記録領域の確認ダイアログ

#include "stdafx.h"
#include "motion_capture.h"
#include "sample_capt_area_dlg.h"
#include "afxdialogex.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace
{
	constexpr int kPictureBoxWidth		= 700;
	constexpr int kPictureBoxHeight	= 400;
}

IMPLEMENT_DYNAMIC(SampleCaptAreaDlg, CDialogEx)

SampleCaptAreaDlg::SampleCaptAreaDlg(const RECT& rt, UINT bitmap_bpp, CWnd* parent)
	: CDialogEx(IDD, parent)
	, preview_(rt)
	, bitmap_bpp_(bitmap_bpp)
{
}

BEGIN_MESSAGE_MAP(SampleCaptAreaDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void SampleCaptAreaDlg::OnShowWindow(BOOL show, UINT status)
{
	CDialogEx::OnShowWindow(show, status);
	PreView();
}

void SampleCaptAreaDlg::InitBitmapInfo()
{
	ZeroMemory(&bitmap_info_, sizeof(BITMAPINFO));
	BITMAPINFOHEADER& header = bitmap_info_.bmiHeader;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = preview_.right - preview_.left;
	header.biHeight = preview_.bottom - preview_.top;
	header.biPlanes = 1;
	header.biBitCount = static_cast<WORD>(bitmap_bpp_);
	header.biCompression = BI_RGB;	// BI_JPEG は指定できなかった

	if (bitmap_bpp_ != 0)
	{
		header.biSizeImage = header.biHeight * ((3 * header.biWidth + 3) / 4) * 4;
	}
	else
	{
		// BI_RGB なら 0 でよい
		header.biSizeImage = 0;
	}
}

void SampleCaptAreaDlg::ScreenCapture()
{
	HDC mem_dc = ::CreateCompatibleDC(nullptr);
	LPVOID pv_bits;

	InitBitmapInfo();
	HBITMAP mem_bitmap = ::CreateDIBSection(nullptr, &bitmap_info_, DIB_RGB_COLORS, &pv_bits, nullptr, 0);
	HBITMAP old_bitmap = static_cast<HBITMAP>(::SelectObject(mem_dc, mem_bitmap));

	HDC dc_screen = ::CreateDC(_T("DISPLAY"), _T("DISPLAY"), _T("DISPLAY"), nullptr);

	::BitBlt(mem_dc, 0, 0, bitmap_info_.bmiHeader.biWidth, bitmap_info_.bmiHeader.biHeight, dc_screen, 0, 0, SRCCOPY);

	::SelectObject(mem_dc, old_bitmap);
	::DeleteObject(mem_bitmap);
	::DeleteDC(mem_dc);
	::DeleteDC(dc_screen);
}

void SampleCaptAreaDlg::PreView()
{
	const CString sample_path = _T("c:\\Sample.bmp");

	if (!PathFileExists(sample_path))
	{
		MessageBox(_T("ファイルオープンに失敗しました。\n") + sample_path);
		return;
	}

	HBITMAP bitmap = static_cast<HBITMAP>(::LoadImage(
		AfxGetInstanceHandle(),
		sample_path,
		IMAGE_BITMAP,
		kPictureBoxWidth,
		kPictureBoxHeight,
		LR_LOADFROMFILE));

	// ビットマップはピクチャーコントロールに渡したまま残す
	CStatic* picture_box = static_cast<CStatic*>(GetDlgItem(IDPC_SAMPLE));
	picture_box->SetBitmap(bitmap);
}

// 24bpp の BMP ファイルとして書き出す
BOOL SampleCaptAreaDlg::WriteBitmap(LPCTSTR file_name, int width, int height, LPVOID bits)
{
	HANDLE file = CreateFile(file_name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD result;
	const DWORD size_image = height * ((3 * width + 3) / 4) * 4;

	BITMAPFILEHEADER bmf_header = {};
	bmf_header.bfType    = 0x4D42;	// "BM"
	bmf_header.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size_image;
	bmf_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(file, &bmf_header, sizeof(BITMAPFILEHEADER), &result, nullptr);

	BITMAPINFOHEADER bmi_header = {};
	bmi_header.biSize        = sizeof(BITMAPINFOHEADER);
	bmi_header.biWidth       = width;
	bmi_header.biHeight      = height;
	bmi_header.biPlanes      = 1;
	bmi_header.biBitCount    = 24;
	bmi_header.biSizeImage   = size_image;
	bmi_header.biCompression = BI_RGB;

	WriteFile(file, &bmi_header, sizeof(BITMAPINFOHEADER), &result, nullptr);

	WriteFile(file, bits, size_image, &result, nullptr);

	CloseHandle(file);

	return TRUE;
}
