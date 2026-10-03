// common_util.h : パス・文字列・ダイアログの共通関数（MBCS / Shift-JIS 前提）

#ifndef COMMON_COMMON_UTIL_H_
#define COMMON_COMMON_UTIL_H_

#include <afxwin.h>

// Shift-JIS の2バイト文字の1バイト目か
inline bool IsSjisLeadByte(unsigned char c)
{
	return (0x81 <= c && c <= 0x9F) || (0xE0 <= c && c <= 0xFC);
}

/////////////////////////////////////////////////////////////////////////////
// パス

// path の後ろにドライブ・ディレクトリ・ファイル名・拡張子を順に連結する（空の要素は飛ばす）
void MergePath(CString& path, LPCTSTR drive, LPCTSTR dir, LPCTSTR file, LPCTSTR ext);

// パスをドライブ・ディレクトリ・ファイル名・拡張子に分解する（不要な出力は nullptr）
void SplitPath(LPCTSTR path, CString* drive, CString* dir, CString* file, CString* ext);

// 区切りの "\" を過不足なく補って連結する
void AppendPath(CString& path, LPCTSTR element);

// "." を補って拡張子を連結する（ext は "txt" / ".txt" どちらでもよい）
void AppendExt(CString& path, LPCTSTR ext);

// フォルダ選択ダイアログ
BOOL BrowseFolder(HWND owner, LPCTSTR title, CString& path);

/////////////////////////////////////////////////////////////////////////////
// 文字列

// search を replace に置換する（replace が nullptr なら削除）
CString ReplaceString(const CString& source, LPCTSTR search, LPCTSTR replace, BOOL case_sensitive);

// 半角英数記号 ⇔ 全角英数記号
CString HankakuToZenkaku(const CString& source);
CString ZenkakuToHankaku(const CString& source);

/////////////////////////////////////////////////////////////////////////////
// ダイアログ

// コントロールIDと文字列リソースIDの組
struct DlgItemText
{
	int		ctrl_id;
	int		string_id;	// 0 なら設定しない
};

// 文字列リソースをまとめてコントロールに設定する
BOOL SetDlgItemTextAll(HWND dlg, const DlgItemText* table, int count);

#endif  // COMMON_COMMON_UTIL_H_
