// pc_hang_up_dlg.h : メインダイアログ（スレッドを作り続けて PC をハングさせる実験）

#pragma once

class PCHangUpDlg : public CDialogEx
{
public:
	explicit PCHangUpDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_PCHANGUP_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedHangUp();
	DECLARE_MESSAGE_MAP()

private:
	static UINT HangUpThreadProc(LPVOID param);

	HICON icon_;
};
