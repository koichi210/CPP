
// VariableArgumentDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


// CVariableArgumentDlg �_�C�A���O
class CVariableArgumentDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CVariableArgumentDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_VARIABLEARGUMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;
	CString m_Input;
	CString m_Replace;
	CString m_Output;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonExecC();
	afx_msg void OnBnClickedButtonExeCpp();
	DECLARE_MESSAGE_MAP()
};
