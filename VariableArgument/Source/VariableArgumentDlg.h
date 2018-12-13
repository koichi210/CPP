
// VariableArgumentDlg.h : ヘッダー ファイル
//

#pragma once


// CVariableArgumentDlg ダイアログ
class CVariableArgumentDlg : public CDialogEx
{
// コンストラクション
public:
	CVariableArgumentDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_VARIABLEARGUMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	CString m_Input;
	CString m_Replace;
	CString m_Output;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonExecC();
	afx_msg void OnBnClickedButtonExeCpp();
	DECLARE_MESSAGE_MAP()
};
