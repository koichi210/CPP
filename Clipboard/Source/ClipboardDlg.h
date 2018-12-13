
// ClipboardDlg.h : ヘッダー ファイル
//

#pragma once


// CClipboardDlg ダイアログ
class CClipboardDlg : public CDialogEx
{
// コンストラクション
public:
	CClipboardDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_CLIPBOARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	CString m_Text;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	BOOL SetClipboardText( const CHAR *Str );

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCopyClipboard();
	DECLARE_MESSAGE_MAP()
};
