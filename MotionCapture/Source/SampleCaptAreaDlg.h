#pragma once


#define PICTURE_BOX_WIDTH	(700)
#define PICTURE_BOX_HEIGHT	(400)

// SampleCaptAreaDlg �_�C�A���O
class SampleCaptAreaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SampleCaptAreaDlg)

public:
	SampleCaptAreaDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	SampleCaptAreaDlg(RECT rt, UINT BitmapBpp, CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~SampleCaptAreaDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SAMPLE_CAPT_AREA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

	void PreView();
	BOOL WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits);
	void GetBitmapInfo();
	void ScreenCapture();

private:
	RECT m_Preview;
	UINT m_BitmapBpp;
	BITMAPINFO m_bitmapInfo;
};

void SampleCaptArea();
BOOL SaveBitmapFile(HBITMAP bmp,LPSTR filename);
