
// MotionCaptureDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include <vfw.h>
#include "ManageAvi.h"


// CMotionCaptureDlg �_�C�A���O
class CMotionCaptureDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CMotionCaptureDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MOTIONCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

	// GUI�ݒ�[�t�@�C����]
	CString m_SaveFilename;

	// GUI�ݒ�[�L�^]
	int m_FrameRate;
	UINT m_TimeoutSec;
	UINT m_SkipFrame;

	// GUI�ݒ�[Capt]
	int m_CaptRectX;
	int m_CaptRectY;
	int m_CaptRectWidth;
	int m_CaptRectHeight;
	UINT m_BitmapBpp;		// 0,1,4,8,16,24,32 (0 is implied by the JPEG or PNG format)

	// GUI�ݒ�[Resize]
	int m_ResizeRectWidth;
	int m_ResizeRectHeight;
	BOOL m_ResizeEnable;

	// GUI�ݒ�[�}�E�X�|�C���^]
	BOOL m_MousePointRec;
};
