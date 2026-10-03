// common_util.cc : パス・文字列・ダイアログの共通関数（MBCS / Shift-JIS 前提）

#include "common_util.h"

#include <ShlObj.h>

#ifdef _UNICODE
#error "common_util は MBCS（文字セット: マルチバイト）専用です"
#endif

static_assert(sizeof("あ") == 3, "文字列リテラルを Shift-JIS で埋め込む必要があります（/execution-charset:.932）");

namespace
{
	bool IsSeparator(TCHAR c)
	{
		return c == _T('\\') || c == _T('/');
	}

	// 末尾が区切り文字か（2バイト文字の2バイト目の 0x5C を誤判定しない）
	bool EndsWithSeparator(const CString& path)
	{
		if (path.IsEmpty())
		{
			return false;
		}
		LPCTSTR begin = path;
		LPCTSTR last = ::CharPrev(begin, begin + path.GetLength());
		return IsSeparator(*last);
	}

	// 半角 0x20〜0x7D に対応する全角文字
	const TCHAR kHankakuFirst = 0x20;
	const TCHAR kHankakuLast = 0x7D;
	const LPCTSTR kZenkakuTable[] =
	{
		"　", "！", "”", "＃", "＄", "％", "＆", "’", "（", "）", "＊", "＋", "，", "－", "．", "／",
		"０", "１", "２", "３", "４", "５", "６", "７", "８", "９", "：", "；", "＜", "＝", "＞", "？",
		"＠", "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ",
		"Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ", "Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "Ｚ", "［", "￥", "］", "＾", "＿",
		"‘", "ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ", "ｈ", "ｉ", "ｊ", "ｋ", "ｌ", "ｍ", "ｎ", "ｏ",
		"ｐ", "ｑ", "ｒ", "ｓ", "ｔ", "ｕ", "ｖ", "ｗ", "ｘ", "ｙ", "ｚ", "｛", "｜", "｝",
	};
	static_assert(_countof(kZenkakuTable) == kHankakuLast - kHankakuFirst + 1, "変換表の個数が範囲と一致しません");
}

/////////////////////////////////////////////////////////////////////////////
// パス

void MergePath(CString& path, LPCTSTR drive, LPCTSTR dir, LPCTSTR file, LPCTSTR ext)
{
	AppendPath(path, drive);
	AppendPath(path, dir);
	AppendPath(path, file);
	AppendExt(path, ext);
}

void SplitPath(LPCTSTR path, CString* drive, CString* dir, CString* file, CString* ext)
{
	TCHAR driveBuf[_MAX_DRIVE] = {};
	TCHAR dirBuf[_MAX_DIR] = {};
	TCHAR fileBuf[_MAX_FNAME] = {};
	TCHAR extBuf[_MAX_EXT] = {};

	if (path != nullptr)
	{
		_tsplitpath_s(path, driveBuf, _countof(driveBuf), dirBuf, _countof(dirBuf), fileBuf, _countof(fileBuf), extBuf, _countof(extBuf));
	}

	if (drive != nullptr)	*drive = driveBuf;
	if (dir != nullptr)		*dir = dirBuf;
	if (file != nullptr)	*file = fileBuf;
	if (ext != nullptr)		*ext = extBuf;
}

void AppendPath(CString& path, LPCTSTR element)
{
	if (element == nullptr || *element == _T('\0'))
	{
		return;
	}
	if (path.IsEmpty())
	{
		path = element;
		return;
	}

	const bool pathHasSeparator = EndsWithSeparator(path);
	const bool elementHasSeparator = IsSeparator(*element);

	if (pathHasSeparator && elementHasSeparator)
	{
		path += element + 1;
	}
	else if (!pathHasSeparator && !elementHasSeparator)
	{
		path += _T('\\');
		path += element;
	}
	else
	{
		path += element;
	}
}

void AppendExt(CString& path, LPCTSTR ext)
{
	if (ext == nullptr || *ext == _T('\0'))
	{
		return;
	}
	if (*ext != _T('.'))
	{
		path += _T('.');
	}
	path += ext;
}

BOOL BrowseFolder(HWND hOwner, LPCTSTR title, CString& path)
{
	TCHAR folder[MAX_PATH] = {};

	BROWSEINFO browseInfo = {};
	browseInfo.hwndOwner = hOwner;
	browseInfo.pszDisplayName = folder;
	browseInfo.lpszTitle = title;
	browseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;

	PIDLIST_ABSOLUTE pidl = ::SHBrowseForFolder(&browseInfo);
	if (pidl == nullptr)
	{
		return FALSE;
	}

	const BOOL bResult = ::SHGetPathFromIDList(pidl, folder);
	::CoTaskMemFree(pidl);

	if (bResult)
	{
		path = folder;
	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// 文字列

// 2バイト文字の途中から一致させないよう、1文字ずつ進めながら比較する
CString ReplaceString(const CString& source, LPCTSTR search, LPCTSTR replace, BOOL bCaseSensitive)
{
	const int searchLength = (search != nullptr) ? lstrlen(search) : 0;
	if (searchLength == 0)
	{
		return source;
	}

	CString result;
	LPCTSTR p = source;
	while (*p != _T('\0'))
	{
		const int compare = bCaseSensitive
			? _tcsncmp(p, search, searchLength)
			: _tcsnicmp(p, search, searchLength);

		if (compare == 0)
		{
			if (replace != nullptr)
			{
				result += replace;
			}
			p += searchLength;
		}
		else
		{
			LPCTSTR next = ::CharNext(p);
			result.Append(p, static_cast<int>(next - p));
			p = next;
		}
	}
	return result;
}

CString HankakuToZenkaku(const CString& source)
{
	CString result;

	for (LPCTSTR p = source; *p != _T('\0'); )
	{
		const unsigned char c = static_cast<unsigned char>(*p);
		if (IsSjisLeadByte(c) && p[1] != _T('\0'))
		{
			// 2バイト文字はそのまま
			result.Append(p, 2);
			p += 2;
			continue;
		}

		if (kHankakuFirst <= c && c <= kHankakuLast)
		{
			result += kZenkakuTable[c - kHankakuFirst];
		}
		else
		{
			result += static_cast<TCHAR>(c);
		}
		p++;
	}

	return result;
}

CString ZenkakuToHankaku(const CString& source)
{
	CString result;

	for (LPCTSTR p = source; *p != _T('\0'); )
	{
		const unsigned char c = static_cast<unsigned char>(*p);
		if (!IsSjisLeadByte(c) || p[1] == _T('\0'))
		{
			result += static_cast<TCHAR>(c);
			p++;
			continue;
		}

		int index = -1;
		for (int i = 0; i < _countof(kZenkakuTable); i++)
		{
			if (kZenkakuTable[i][0] == p[0] && kZenkakuTable[i][1] == p[1])
			{
				index = i;
				break;
			}
		}

		if (index >= 0)
		{
			result += static_cast<TCHAR>(kHankakuFirst + index);
		}
		else
		{
			result.Append(p, 2);
		}
		p += 2;
	}

	return result;
}

/////////////////////////////////////////////////////////////////////////////
// ダイアログ

BOOL SetDlgItemTextAll(HWND hDlg, const DLGITEMTEXT* table, int count)
{
	if (table == nullptr)
	{
		return FALSE;
	}

	for (int i = 0; i < count; i++)
	{
		if (table[i].stringId != 0)
		{
			CString text;
			text.LoadString(table[i].stringId);
			::SetDlgItemText(hDlg, table[i].ctrlId, text);
		}
	}
	return TRUE;
}
