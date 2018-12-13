
// ClipboardDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


// CClipboardDlg �_�C�A���O
class CClipboardDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CClipboardDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CLIPBOARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;
	CString m_Text;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	BOOL SetClipboardText( const CHAR *Str );

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCopyClipboard();
	DECLARE_MESSAGE_MAP()
};
