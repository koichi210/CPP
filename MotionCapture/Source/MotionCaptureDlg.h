// MotionCaptureDlg.h : メインダイアログ

#pragma once

#include "ManageAvi.h"

class CMotionCaptureDlg : public CDialogEx
{
public:
	CMotionCaptureDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_MOTIONCAPTURE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedRecordStop();
	afx_msg void OnBnClickedSampleCaptArea();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

private:
	HICON		m_hIcon;
	CManageAvi	m_avi;

	// 画面の設定値
	CString	m_saveFilename{ _T("c:\\ScreenCapture.avi") };

	int		m_frameRate = 20;
	UINT	m_timeoutSec = 10;
	UINT	m_skipFrame = 0;

	int		m_captRectX = 0;
	int		m_captRectY = 0;
	int		m_captRectWidth = 1920;
	int		m_captRectHeight = 1080;
	UINT	m_bitmapBpp = 24;		// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)

	int		m_resizeRectWidth = 1024;
	int		m_resizeRectHeight = 768;
	BOOL	m_bResize = FALSE;

	BOOL	m_bMousePointRec = TRUE;
};
