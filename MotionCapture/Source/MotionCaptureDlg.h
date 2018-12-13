
// MotionCaptureDlg.h : ヘッダー ファイル
//

#pragma once
#include <vfw.h>
#include "ManageAvi.h"


// CMotionCaptureDlg ダイアログ
class CMotionCaptureDlg : public CDialogEx
{
// コンストラクション
public:
	CMotionCaptureDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_MOTIONCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedRecordStop();
	afx_msg void OnBnClickedSampleCaptArea();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

public :
	CManageAvi m_Avi;

	// GUI設定[ファイル名]
	CString m_SaveFilename;

	// GUI設定[記録]
	int m_FrameRate;
	UINT m_TimeoutSec;
	UINT m_SkipFrame;

	// GUI設定[Capt]
	int m_CaptRectX;
	int m_CaptRectY;
	int m_CaptRectWidth;
	int m_CaptRectHeight;
	UINT m_BitmapBpp;		// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)

	// GUI設定[Resize]
	int m_ResizeRectWidth;
	int m_ResizeRectHeight;
	BOOL m_ResizeEnable;

	// GUI設定[マウスポインタ]
	BOOL m_MousePointRec;
};
