// file_comparer.cc : 2つのファイルの内容をバイナリ比較する

#include "file_comparer.h"

#include <vector>

BOOL FileComparer::CompareBinary(LPCTSTR file1, LPCTSTR file2)
{
	file1_ = file1;
	file2_ = file2;
	return CompareBinary();
}

// 大きなファイルでもメモリを使い過ぎないよう、少しずつ読んで比べる
BOOL FileComparer::CompareBinary()
{
	const UINT open_flags = CFile::modeRead | CFile::shareDenyNone;
	error_ = kErrNone;

	CFile file1;
	if (!file1.Open(file1_, open_flags))
	{
		error_ = kErrFile1Open;
		return FALSE;
	}

	CFile file2;
	if (!file2.Open(file2_, open_flags))
	{
		error_ = kErrFile2Open;
		return FALSE;
	}

	if (file1.GetLength() != file2.GetLength())
	{
		return FALSE;
	}

	const UINT chunk_size = 64 * 1024;
	std::vector<BYTE> buffer1(chunk_size);
	std::vector<BYTE> buffer2(chunk_size);

	for (;;)
	{
		const UINT read1 = file1.Read(buffer1.data(), chunk_size);
		const UINT read2 = file2.Read(buffer2.data(), chunk_size);
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
