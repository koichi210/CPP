// memsetDlg.h : メインダイアログ

#ifndef MEMSET_SOURCE_MEMSET_DLG_H_
#define MEMSET_SOURCE_MEMSET_DLG_H_

class MemsetDlg : public CDialogEx
{
public:
	explicit MemsetDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_memset_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedExe();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
	int fill_value_ = 0;
};

#endif  // MEMSET_SOURCE_MEMSET_DLG_H_
