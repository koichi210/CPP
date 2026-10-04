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
	TCHAR drive_buf[_MAX_DRIVE] = {};
	TCHAR dir_buf[_MAX_DIR] = {};
	TCHAR file_buf[_MAX_FNAME] = {};
	TCHAR ext_buf[_MAX_EXT] = {};

	if (path != nullptr)
	{
		_tsplitpath_s(path, drive_buf, _countof(drive_buf), dir_buf, _countof(dir_buf), file_buf, _countof(file_buf), ext_buf, _countof(ext_buf));
	}

	if (drive != nullptr)	*drive = drive_buf;
	if (dir != nullptr)		*dir = dir_buf;
	if (file != nullptr)	*file = file_buf;
	if (ext != nullptr)		*ext = ext_buf;
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

	const bool path_has_separator = EndsWithSeparator(path);
	const bool element_has_separator = IsSeparator(*element);

	if (path_has_separator && element_has_separator)
	{
		path += element + 1;
	}
	else if (!path_has_separator && !element_has_separator)
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

BOOL BrowseFolder(HWND owner, LPCTSTR title, CString& path)
{
	TCHAR folder[MAX_PATH] = {};

	BROWSEINFO browse_info = {};
	browse_info.hwndOwner = owner;
	browse_info.pszDisplayName = folder;
	browse_info.lpszTitle = title;
	browse_info.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;

	PIDLIST_ABSOLUTE pidl = ::SHBrowseForFolder(&browse_info);
	if (pidl == nullptr)
	{
		return FALSE;
	}

	const BOOL result = ::SHGetPathFromIDList(pidl, folder);
	::CoTaskMemFree(pidl);

	if (result)
	{
		path = folder;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// 文字列

// 2バイト文字の途中から一致させないよう、1文字ずつ進めながら比較する
CString ReplaceString(const CString& source, LPCTSTR search, LPCTSTR replace, BOOL case_sensitive)
{
	const int search_length = (search != nullptr) ? lstrlen(search) : 0;
	if (search_length == 0)
	{
		return source;
	}

	CString result;
	result.Preallocate(source.GetLength());
	LPCTSTR p = source;
	while (*p != _T('\0'))
	{
		const int compare = case_sensitive
			? _tcsncmp(p, search, search_length)
			: _tcsnicmp(p, search, search_length);

		if (compare == 0)
		{
			if (replace != nullptr)
			{
				result += replace;
			}
			p += search_length;
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
	result.Preallocate(source.GetLength() * 2);

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
	result.Preallocate(source.GetLength());

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

BOOL SetDlgItemTextAll(HWND dlg, const DlgItemText* table, int count)
{
	if (table == nullptr)
	{
		return FALSE;
	}

	CString text;
	for (int i = 0; i < count; i++)
	{
		if (table[i].string_id != 0)
		{
			text.LoadString(table[i].string_id);
			::SetDlgItemText(dlg, table[i].ctrl_id, text);
		}
	}
	return TRUE;
}
