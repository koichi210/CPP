
// MotionCaptureDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "MotionCapture.h"
#include "MotionCaptureDlg.h"
#include "SampleCaptAreaDlg.h"
#include "ManageAvi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMotionCaptureDlg �_�C�A���O
CMotionCaptureDlg::CMotionCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotionCaptureDlg::IDD, pParent)
	, m_ResizeRectWidth(0)
	, m_ResizeRectHeight(0)
	, m_ResizeEnable(FALSE)
	, m_MousePointRec(FALSE)
	, m_SkipFrame(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// �O������w�肷��p�����[�^
	m_SaveFilename = "c:\\ScreenCapture.avi";
	m_MousePointRec = TRUE;

	m_FrameRate = 20;
	m_TimeoutSec = 10;
	m_SkipFrame = 0;

	m_CaptRectX = 0;
	m_CaptRectY = 0;
	m_CaptRectWidth = 1920;
	m_CaptRectHeight = 1080;
	m_BitmapBpp = 24;

	m_ResizeEnable = FALSE;
	m_ResizeRectWidth = 1024;
	m_ResizeRectHeight = 768;

}

void CMotionCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDET_SAVE_FILENAME, m_SaveFilename);
	DDX_Text(pDX, IDET_CAPT_RECT_X, m_CaptRectX);
	DDX_Text(pDX, IDET_CAPT_RECT_Y, m_CaptRectY);
	DDX_Text(pDX, IDET_CAPT_RECT_WIDTH, m_CaptRectWidth);
	DDX_Text(pDX, IDET_CAPT_RECT_HEIGHT, m_CaptRectHeight);
	DDX_Text(pDX, IDET_CAPT_BPP, m_BitmapBpp);
	DDV_MinMaxInt(pDX, m_BitmapBpp, 0, 32);
	DDX_Text(pDX, IDET_CAPT_FPS, m_FrameRate);
	DDV_MinMaxInt(pDX, m_FrameRate, 1, 120);
	DDX_Text(pDX, IDET_TIMEOUT_SEC, m_TimeoutSec);
	DDX_Text(pDX, IDET_SKIP_FRAME, m_SkipFrame);
	DDX_Text(pDX, IDET_RESIZE_RECT_WIDTH, m_ResizeRectWidth);
	DDX_Text(pDX, IDET_RESIZE_RECT_HEIGHT, m_ResizeRectHeight);
	DDX_Check(pDX, IDCH_RESIZE, m_ResizeEnable);
	DDX_Check(pDX, IDCH_MOUSE_POINT_REC, m_MousePointRec);
}

BEGIN_MESSAGE_MAP(CMotionCaptureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_RECORD, OnBnClickedRecord)
	ON_BN_CLICKED(IDBT_RECORD_STOP, OnBnClickedRecordStop)
	ON_BN_CLICKED(IDBT_CAPT_AREA_SAMPLE, OnBnClickedSampleCaptArea)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMotionCaptureDlg ���b�Z�[�W �n���h���[

BOOL CMotionCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CMotionCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CMotionCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMotionCaptureDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	//CDialogEx::OnOK();
}

void CMotionCaptureDlg::OnBnClickedRecord()
{
	if (m_Avi.IsExecution() )
	{
		int rt = MessageBox("���s���ł��B���f���܂����H", "Warning", MB_YESNO);
		if ( rt == IDYES )
		{
			OnBnClickedRecordStop();
		}
		return;
	}

	// GUI�̒l���擾
	UpdateData();

	// �ʃv���Z�X���A�N�Z�X�����`�F�b�N
	{
		CFile cfile;
		if ( cfile.Open(_T(m_SaveFilename), CFile::modeCreate | CFile::modeWrite) )
		{
			cfile.Close();
		}
		else
		{
			MessageBox("�ʃv���Z�X���g�p���ł��B", "Warning", MB_OK);
			return;
		}
	}

	// �p�����[�^��ݒ�
	m_Avi.SetSaveFileName(m_SaveFilename);
	m_Avi.SetFrameRate(m_FrameRate);
	m_Avi.SetRecordSec(m_TimeoutSec);
	m_Avi.SetSkipFrame(m_SkipFrame);
	m_Avi.SetCaptureRect(m_CaptRectX, m_CaptRectY, m_CaptRectWidth, m_CaptRectHeight);
	m_Avi.SetCaptureQuality(m_BitmapBpp);
	m_Avi.SetResize(m_ResizeEnable, m_ResizeRectWidth, m_ResizeRectHeight);
	m_Avi.SetRecordMousePoint(m_MousePointRec);

	// �L�^�J�n
	m_Avi.StartRecord();
}

void CMotionCaptureDlg::OnBnClickedRecordStop()
{
	// �L�^�^�X�N���~�߂�
	m_Avi.StopRecord();
}

void CMotionCaptureDlg::OnBnClickedSampleCaptArea()
{
	UpdateData();
	RECT rt = {m_CaptRectX, m_CaptRectY, m_CaptRectWidth, m_CaptRectHeight};

	SampleCaptAreaDlg dlg(rt, m_BitmapBpp);
	dlg.DoModal();
	return;
}
