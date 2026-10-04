// sample_capt_area_dlg.h : 記録領域の確認ダイアログ

#ifndef MOTIONCAPTURE_SOURCE_SAMPLE_CAPT_AREA_DLG_H_
#define MOTIONCAPTURE_SOURCE_SAMPLE_CAPT_AREA_DLG_H_

// 開いた時点の画面から記録領域を撮って、縮小して表示する
class SampleCaptAreaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SampleCaptAreaDlg)

public:
	SampleCaptAreaDlg(const CRect& capture_area, CWnd* parent = nullptr);

	enum { IDD = IDD_SAMPLE_CAPT_AREA_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	HBITMAP CaptureArea() const;

	CRect	capture_area_;		// 画面上の記録領域
};

#endif  // MOTIONCAPTURE_SOURCE_SAMPLE_CAPT_AREA_DLG_H_
