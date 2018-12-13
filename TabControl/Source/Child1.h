#pragma once


// CChild1 ダイアログ

class CChild1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChild1)

public:
	CChild1(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CChild1();

// ダイアログ データ
	enum { IDD = IDD_CHILD1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
