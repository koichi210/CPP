// signed_unsigned_dlg.h : メインダイアログ

#ifndef SIGNEDUNSIGNED_SIGNEDUNSIGNED_SIGNED_UNSIGNED_DLG_H_
#define SIGNEDUNSIGNED_SIGNEDUNSIGNED_SIGNED_UNSIGNED_DLG_H_

class SignedUnsignedDlg : public CDialogEx
{
public:
	explicit SignedUnsignedDlg(CWnd* parent = nullptr);

	enum { IDD = IDD_SIGNEDUNSIGNED_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* dx) override;
	virtual BOOL OnInitDialog() override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	HICON icon_;
};

#endif  // SIGNEDUNSIGNED_SIGNEDUNSIGNED_SIGNED_UNSIGNED_DLG_H_
