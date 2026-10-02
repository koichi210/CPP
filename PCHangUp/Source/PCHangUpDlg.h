// PCHangUpDlg.h : メインダイアログ（スレッドを作り続けて PC をハングさせる実験）

#pragma once

class CPCHangUpDlg : public CDialogEx
{
public:
	explicit CPCHangUpDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_PCHANGUP_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedHangUp();
	DECLARE_MESSAGE_MAP()

private:
	static UINT HangUpThreadProc(LPVOID pParam);

	HICON m_hIcon;
};
