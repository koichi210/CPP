// joint_movie_dlg.h : メインダイアログ（複数の動画ファイルを copy /B で連結）

#ifndef JOINTMOVIE_SOURCE_JOINT_MOVIE_DLG_H_
#define JOINTMOVIE_SOURCE_JOINT_MOVIE_DLG_H_

class JointMovieDlg : public CDialog
{
public:
	explicit JointMovieDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_JOINTMOVIE_DIALOG };

protected:
	virtual BOOL OnInitDialog() override;

	afx_msg void OnSysCommand(UINT id, LPARAM param);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse(UINT id);
	afx_msg void OnExecute();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
};

#endif  // JOINTMOVIE_SOURCE_JOINT_MOVIE_DLG_H_
