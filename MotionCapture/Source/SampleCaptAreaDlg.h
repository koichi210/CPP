// SampleCaptAreaDlg.h : 記録領域の確認ダイアログ

#pragma once

class CSampleCaptAreaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSampleCaptAreaDlg)

public:
	CSampleCaptAreaDlg(const RECT& rt, UINT bitmapBpp, CWnd* pParent = nullptr);

	enum { IDD = IDD_SAMPLE_CAPT_AREA_DIALOG };

protected:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

	void PreView();

	// 記録領域のキャプチャ（試作中。まだどこからも呼ばれていない）
	BOOL WriteBitmap(LPCTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits);
	void InitBitmapInfo();
	void ScreenCapture();

private:
	RECT		m_preview;
	UINT		m_bitmapBpp;
	BITMAPINFO	m_bitmapInfo = {};
};
