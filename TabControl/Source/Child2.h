#pragma once


// CChild2 ダイアログ

class CChild2 : public CDialogEx
{
	DECLARE_DYNAMIC(CChild2)

public:
	CChild2(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CChild2();

// ダイアログ データ
	enum { IDD = IDD_CHILD2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
