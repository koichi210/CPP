// progress_bar_single_thread_dlg.h : メインダイアログ

#pragma once

class ProgressBarSingleThreadDlg : public CDialogEx
{
public:
	explicit ProgressBarSingleThreadDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_PROGRESSBAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
	CProgressCtrl progress_;
};
