// SendSpoolFileDlg.h : ヘッダー ファイル
//
BOOL SpoolJob(HANDLE hPrinter, LPSTR SpoolName);

#pragma once
#define MAX_BUFF	(4096)

// CSendSpoolFileDlg ダイアログ
class CSendSpoolFileDlg : public CDialog
{
// コンストラクション
public:
	CSendSpoolFileDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

	void AddMyPrinter(DWORD PrinterEnumId);	// プリンタ追加

// ダイアログ データ
	enum { IDD = IDD_SPOOLJOB2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBrowse();
	afx_msg void OnExecute();
};
