// file_comparer.h : 2つのファイルの内容をバイナリ比較する

#pragma once

#include <afxwin.h>

class CFileComparer
{
public:
	enum Error
	{
		ERR_NONE		= 0,
		ERR_FILE1_OPEN	= -1,
		ERR_FILE2_OPEN	= -2,
	};

	CFileComparer() = default;
	CFileComparer(LPCTSTR file1, LPCTSTR file2) : m_file1(file1), m_file2(file2) {}

	BOOL CompareBinary();								// 同じ内容なら TRUE
	BOOL CompareBinary(LPCTSTR file1, LPCTSTR file2);
	int GetError() const	{ return m_error; }		// 直前の比較で開けなかったファイル

private:
	CString	m_file1;
	CString	m_file2;
	int		m_error = ERR_NONE;
};
