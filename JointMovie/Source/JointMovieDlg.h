// JointMovieDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#define GROUP_MAX	(8)

// CJointMovieDlg �_�C�A���O
class CJointMovieDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CJointMovieDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_JOINTMOVIE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnExecute();
	DECLARE_MESSAGE_MAP()
};
