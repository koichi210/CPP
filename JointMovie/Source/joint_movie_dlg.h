// JointMovieDlg.h : メインダイアログ（複数の動画ファイルを copy /B で連結）

#pragma once

class CJointMovieDlg : public CDialog
{
public:
	explicit CJointMovieDlg(CWnd* pParent = nullptr);

	enum { IDD = IDD_JOINTMOVIE_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse(UINT nID);
	afx_msg void OnExecute();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};
