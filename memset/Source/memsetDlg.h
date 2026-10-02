// memsetDlg.h : メインダイアログ

#pragma once

class CMemsetDlg : public CDialogEx
{
public:
	explicit CMemsetDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_memset_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedExe();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	int m_fillValue = 0;
};
