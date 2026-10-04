// sample_capt_area_dlg.cc : 記録領域の確認ダイアログ

#include "stdafx.h"
#include "motion_capture.h"
#include "sample_capt_area_dlg.h"
#include "afxdialogex.h"

#include <algorithm>

namespace
{
	// 撮った画像を表示する最大の大きさ（ピクセル）
	constexpr int kPictureBoxWidth	= 700;
	constexpr int kPictureBoxHeight	= 400;
}

IMPLEMENT_DYNAMIC(SampleCaptAreaDlg, CDialogEx)

SampleCaptAreaDlg::SampleCaptAreaDlg(const CRect& capture_area, CWnd* parent)
	: CDialogEx(IDD, parent)
	, capture_area_(capture_area)
{
}

BEGIN_MESSAGE_MAP(SampleCaptAreaDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// 表示される前に撮るので、このダイアログ自身は写り込まない
BOOL SampleCaptAreaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString caption;
	caption.Format(_T("キャプチャ領域 (%d, %d) %d x %d"),
		capture_area_.left, capture_area_.top, capture_area_.Width(), capture_area_.Height());
	SetWindowText(caption);

	// 表示中のビットマップは OnDestroy で解放する
	CStatic* picture_box = static_cast<CStatic*>(GetDlgItem(IDPC_SAMPLE));
	picture_box->SetBitmap(CaptureArea());

	return TRUE;
}

// ピクチャーコントロールは渡したビットマップを解放しないので、ここで解放する
void SampleCaptAreaDlg::OnDestroy()
{
	CStatic* picture_box = static_cast<CStatic*>(GetDlgItem(IDPC_SAMPLE));
	HBITMAP bitmap = picture_box->SetBitmap(nullptr);
	if (bitmap != nullptr)
	{
		::DeleteObject(bitmap);
	}

	CDialogEx::OnDestroy();
}

// 画面の記録領域を、縦横比を保ったまま表示枠に収まる大きさで撮る
HBITMAP SampleCaptAreaDlg::CaptureArea() const
{
	const int src_width = capture_area_.Width();
	const int src_height = capture_area_.Height();
	const double scale = (std::min)({ 1.0,
		static_cast<double>(kPictureBoxWidth) / src_width,
		static_cast<double>(kPictureBoxHeight) / src_height });
	const int width = (std::max)(1, static_cast<int>(src_width * scale));
	const int height = (std::max)(1, static_cast<int>(src_height * scale));

	HDC dc_screen = ::GetDC(nullptr);
	HDC mem_dc = ::CreateCompatibleDC(dc_screen);
	HBITMAP bitmap = ::CreateCompatibleBitmap(dc_screen, width, height);
	HBITMAP old_bitmap = static_cast<HBITMAP>(::SelectObject(mem_dc, bitmap));

	// 確認用なので、録画より時間をかけてきれいに縮める
	::SetStretchBltMode(mem_dc, HALFTONE);
	::SetBrushOrgEx(mem_dc, 0, 0, nullptr);
	::StretchBlt(mem_dc, 0, 0, width, height,
		dc_screen, capture_area_.left, capture_area_.top, src_width, src_height, SRCCOPY);

	::SelectObject(mem_dc, old_bitmap);
	::DeleteDC(mem_dc);
	::ReleaseDC(nullptr, dc_screen);

	return bitmap;
}
