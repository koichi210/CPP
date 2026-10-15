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
	virtual void OnCancel() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedRecordStop();
	afx_msg void OnBnClickedSampleCaptArea();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEndSession(BOOL ending);
	afx_msg void OnTimer(UINT_PTR id_event);
	afx_msg LRESULT OnRecordFinished(WPARAM w_param, LPARAM l_param);	// 録画スレッドの終了通知
	DECLARE_MESSAGE_MAP()

private:
	bool StopRecordForClose();	// 録画中なら確認のうえ中断し、ファイルを閉じ終わるまで待つ（閉じてよければ true）
	void StopRecordAndWait();	// 確認せずに中断し、ファイルを閉じ終わるまで待つ
	bool ConfirmWhileRecording(LPCTSTR text);	// 録画中の確認（はいなら true）。その間に届いた結果は預かる
	void ShowPendingResult();	// 預かった録画の結果があれば出す
	void ShowRecordResult(AviError err, BOOL completed);

	HICON		icon_;
	ManageAvi	avi_;
	bool		closing_ = false;	// 終了処理中（録画の結果を表示しない）
	bool		confirming_ = false;	// 録画中の確認を出している（その間に届いた録画の結果は預かる）
	bool		has_pending_result_ = false;	// 確認の間に届いて預かっている録画の結果がある
	AviError	pending_err_ = AviError::kSuccess;
	BOOL		pending_completed_ = FALSE;

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
