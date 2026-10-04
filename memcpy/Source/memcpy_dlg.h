// memcpy_dlg.h : メインダイアログ

#ifndef MEMCPY_SOURCE_MEMCPY_DLG_H_
#define MEMCPY_SOURCE_MEMCPY_DLG_H_

class MemcpyDlg : public CDialogEx
{
public:
	explicit MemcpyDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_MEMCPY_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
};

#endif  // MEMCPY_SOURCE_MEMCPY_DLG_H_
