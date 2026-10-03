// file_comparer.cc : 2つのファイルの内容をバイナリ比較する

#include "file_comparer.h"

#include <vector>

BOOL CFileComparer::CompareBinary(LPCTSTR file1, LPCTSTR file2)
{
	m_file1 = file1;
	m_file2 = file2;
	return CompareBinary();
}

// 大きなファイルでもメモリを使い過ぎないよう、少しずつ読んで比べる
BOOL CFileComparer::CompareBinary()
{
	const UINT openFlags = CFile::modeRead | CFile::shareDenyNone;
	m_error = ERR_NONE;

	CFile file1;
	if (!file1.Open(m_file1, openFlags))
	{
		m_error = ERR_FILE1_OPEN;
		return FALSE;
	}

	CFile file2;
	if (!file2.Open(m_file2, openFlags))
	{
		m_error = ERR_FILE2_OPEN;
		return FALSE;
	}

	if (file1.GetLength() != file2.GetLength())
	{
		return FALSE;
	}

	const UINT chunkSize = 64 * 1024;
	std::vector<BYTE> buffer1(chunkSize);
	std::vector<BYTE> buffer2(chunkSize);

	for (;;)
	{
		const UINT read1 = file1.Read(buffer1.data(), chunkSize);
		const UINT read2 = file2.Read(buffer2.data(), chunkSize);
		if (read1 != read2 || memcmp(buffer1.data(), buffer2.data(), read1) != 0)
		{
			return FALSE;
		}
		if (read1 == 0)
		{
			return TRUE;
		}
	}
}
