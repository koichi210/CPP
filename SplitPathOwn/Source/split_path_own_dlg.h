// SplitPathOwnDlg.h : メインダイアログ

#pragma once

class SplitPathOwnDlg : public CDialogEx
{
public:
	explicit SplitPathOwnDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_SPLITPATHOWN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	// _splitpath を使わずにパスを分解する自前実装（学習用）。区切りは '/' のみ対応
	void SplitPath(const char* file_full_path, char* drive, char* dir, char* file);

	HICON icon_;
};
