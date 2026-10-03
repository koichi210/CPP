// motion_capture_dlg.h : メインダイアログ

#ifndef MOTIONCAPTURE_SOURCE_MOTION_CAPTURE_DLG_H_
#define MOTIONCAPTURE_SOURCE_MOTION_CAPTURE_DLG_H_

#include "manage_avi.h"

class MotionCaptureDlg : public CDialogEx
{
public:
	MotionCaptureDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_MOTIONCAPTURE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedRecordStop();
	afx_msg void OnBnClickedSampleCaptArea();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

private:
	HICON		icon_;
	ManageAvi	avi_;

	// 画面の設定値
	CString	save_filename_{ _T("c:\\ScreenCapture.avi") };

	int		frame_rate_ = 20;
	UINT	timeout_sec_ = 10;
	UINT	skip_frame_ = 0;

	int		capt_rect_x_ = 0;
	int		capt_rect_y_ = 0;
	int		capt_rect_width_ = 1920;
	int		capt_rect_height_ = 1080;
	UINT	bitmap_bpp_ = 24;		// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)

	int		resize_rect_width_ = 1024;
	int		resize_rect_height_ = 768;
	BOOL	resize_ = FALSE;

	BOOL	mouse_point_rec_ = TRUE;
};

#endif  // MOTIONCAPTURE_SOURCE_MOTION_CAPTURE_DLG_H_
