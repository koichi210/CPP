// Child2.h : タブ（PageB）に表示する子ダイアログ

#pragma once

class Child2 : public CDialogEx
{
	DECLARE_DYNAMIC(Child2)

public:
	explicit Child2(CWnd* parent = nullptr);

	enum { IDD = IDD_CHILD2 };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;

	DECLARE_MESSAGE_MAP()
};
