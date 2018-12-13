// SendSpoolFileDlg.h : �w�b�_�[ �t�@�C��
//
BOOL SpoolJob(HANDLE hPrinter, LPSTR SpoolName);

#pragma once
#define MAX_BUFF	(4096)

// CSendSpoolFileDlg �_�C�A���O
class CSendSpoolFileDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSendSpoolFileDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

	void AddMyPrinter(DWORD PrinterEnumId);	// �v�����^�ǉ�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SPOOLJOB2_DIALOG };

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
	afx_msg void OnBrowse();
	afx_msg void OnExecute();
};
