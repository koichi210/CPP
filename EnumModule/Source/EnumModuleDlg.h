// EnumModuleDlg.h : メインダイアログ（プロセスが読み込んでいるモジュールを列挙）

#pragma once

class CEnumModuleDlg : public CDialogEx
{
public:
	explicit CEnumModuleDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_ENUMMODULE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedGetModulename();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	CString m_strProcessName;
	CString m_strResult;
};
