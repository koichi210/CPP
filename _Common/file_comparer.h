// file_comparer.h : 2つのファイルの内容をバイナリ比較する

#pragma once

#include <afxwin.h>

class FileComparer
{
public:
	enum Error
	{
		kErrNone		= 0,
		kErrFile1Open	= -1,
		kErrFile2Open	= -2,
	};

	FileComparer() = default;
	FileComparer(LPCTSTR file1, LPCTSTR file2) : file1_(file1), file2_(file2) {}

	BOOL CompareBinary();								// 同じ内容なら TRUE
	BOOL CompareBinary(LPCTSTR file1, LPCTSTR file2);
	int GetError() const	{ return error_; }		// 直前の比較で開けなかったファイル

private:
	CString	file1_;
	CString	file2_;
	int		error_ = kErrNone;
};
