
// EnumModuleDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


// CEnumModuleDlg �_�C�A���O
class CEnumModuleDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CEnumModuleDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ENUMMODULE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

	CString m_Result;
	CString m_ProcessName;

// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedGetModulename();
	DECLARE_MESSAGE_MAP()
};

CString PrintModuleName( DWORD processID );
BOOL GetPhysicalFileName(LPCTSTR szFileName, LPTSTR szRealFileName, SIZE_T nBufSize);
