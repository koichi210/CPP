
// SignedUnsignedDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


// CSignedUnsignedDlg �_�C�A���O
class CSignedUnsignedDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CSignedUnsignedDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SIGNEDUNSIGNED_DIALOG };

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
