#pragma once


// CChild1 �_�C�A���O

class CChild1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChild1)

public:
	CChild1(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CChild1();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CHILD1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
