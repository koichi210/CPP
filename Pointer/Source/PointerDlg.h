
// PointerDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


typedef struct TEST {
    UINT param1;
    UINT param2;
    UINT param3;
}TEST_T;


// CPointerDlg �_�C�A���O
class CPointerDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CPointerDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[
	void GetParam(TEST_T** p );
	TEST_T* GetParam();

	TEST_T m_test;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_POINTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()
};
