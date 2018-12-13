
// EnumModuleDlg.h : ヘッダー ファイル
//

#pragma once


// CEnumModuleDlg ダイアログ
class CEnumModuleDlg : public CDialogEx
{
// コンストラクション
public:
	CEnumModuleDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_ENUMMODULE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	CString m_Result;
	CString m_ProcessName;

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedGetModulename();
	DECLARE_MESSAGE_MAP()
};

CString PrintModuleName( DWORD processID );
BOOL GetPhysicalFileName(LPCTSTR szFileName, LPTSTR szRealFileName, SIZE_T nBufSize);
