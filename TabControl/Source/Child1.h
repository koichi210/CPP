// Child1.h : タブ（PageA）に表示する子ダイアログ

#pragma once

class CChild1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChild1)

public:
	explicit CChild1(CWnd* pParent = nullptr);

	enum { IDD = IDD_CHILD1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
};
