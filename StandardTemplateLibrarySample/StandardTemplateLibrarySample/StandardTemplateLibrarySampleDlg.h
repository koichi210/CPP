
// StandardTemplateLibrarySampleDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


// CStandardTemplateLibrarySampleDlg �_�C�A���O
class CStandardTemplateLibrarySampleDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CStandardTemplateLibrarySampleDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_STANDARDTEMPLATELIBRARYSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
