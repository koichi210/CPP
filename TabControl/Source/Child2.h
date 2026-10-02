// Child2.h : タブ（PageB）に表示する子ダイアログ

#pragma once

class CChild2 : public CDialogEx
{
	DECLARE_DYNAMIC(CChild2)

public:
	explicit CChild2(CWnd* pParent = nullptr);

	enum { IDD = IDD_CHILD2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
};
