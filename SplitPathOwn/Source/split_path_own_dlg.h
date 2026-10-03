// SplitPathOwnDlg.h : メインダイアログ

#pragma once

class CSplitPathOwnDlg : public CDialogEx
{
public:
	explicit CSplitPathOwnDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_SPLITPATHOWN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	// _splitpath を使わずにパスを分解する自前実装（学習用）。区切りは '/' のみ対応
	void SplitPath(const char* pFileFullPath, char* pDrive, char* pDir, char* pFile);

	HICON m_hIcon;
};
