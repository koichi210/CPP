#pragma once


// CChild2 �_�C�A���O

class CChild2 : public CDialogEx
{
	DECLARE_DYNAMIC(CChild2)

public:
	CChild2(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CChild2();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CHILD2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
