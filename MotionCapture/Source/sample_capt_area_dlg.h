// sample_capt_area_dlg.h : 記録領域の確認ダイアログ

#ifndef MOTIONCAPTURE_SOURCE_SAMPLE_CAPT_AREA_DLG_H_
#define MOTIONCAPTURE_SOURCE_SAMPLE_CAPT_AREA_DLG_H_

class SampleCaptAreaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SampleCaptAreaDlg)

public:
	SampleCaptAreaDlg(const RECT& rt, UINT bitmap_bpp, CWnd* parent = nullptr);

	enum { IDD = IDD_SAMPLE_CAPT_AREA_DIALOG };

protected:
	afx_msg void OnShowWindow(BOOL show, UINT status);
	DECLARE_MESSAGE_MAP()

	void PreView();

	// 記録領域のキャプチャ（試作中。まだどこからも呼ばれていない）
	BOOL WriteBitmap(LPCTSTR file_name, int width, int height, LPVOID bits);
	void InitBitmapInfo();
	void ScreenCapture();

private:
	RECT		preview_;
	UINT		bitmap_bpp_;
	BITMAPINFO	bitmap_info_ = {};
};

#endif  // MOTIONCAPTURE_SOURCE_SAMPLE_CAPT_AREA_DLG_H_
