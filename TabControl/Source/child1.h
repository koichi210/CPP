// Child1.h : タブ（PageA）に表示する子ダイアログ

#pragma once

class Child1 : public CDialogEx
{
	DECLARE_DYNAMIC(Child1)

public:
	explicit Child1(CWnd* parent = nullptr);

	enum { IDD = IDD_CHILD1 };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;

	DECLARE_MESSAGE_MAP()
};
