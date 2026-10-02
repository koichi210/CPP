// ProgressBar_SingleThreadDlg.h : メインダイアログ

#pragma once

class CProgressBar_SingleThreadDlg : public CDialogEx
{
public:
	explicit CProgressBar_SingleThreadDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_PROGRESSBAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	CProgressCtrl m_progress;
};
