// 共通のユーティティ
#include "tchar.h"
#include "stdafx.h"
#include "utils.h"

// 文字列を結合してpathに格納
void MergePath(CString *path, CString dir, CString file, CString ext)
{
	*path = dir;
	AppendPath(path,file);
	AppendExt(path,ext);
}

// pathに格納されているフルパスを各文字列に分ける
void SplitPath(CString path, CString *dir, CString *file, CString *ext)
{
	char wk_drv[MAX_PATH];
	char wk_dir[MAX_PATH];
	char wk_file[MAX_PATH];
	char wk_ext[MAX_PATH];

	splitpath_s(path.GetString(), wk_drv, sizeof(wk_drv), wk_dir, sizeof(wk_dir), wk_file, sizeof(wk_file), wk_ext, sizeof(wk_ext));
	if( dir && wk_dir )
	{
		*dir = wk_drv;
		*dir += wk_dir;
	}
	if( file && wk_file )
	{
		*file = wk_file;
	}
	if( ext && wk_ext )
	{
		*ext = wk_ext;
	}
}

void AppendPath(CString *path, CString more)
{
	*path += "\\" + more;
}

void AppendExt(CString *path, CString dist)
{
//	*path += "." + dist;
	*path += dist;
}

void ReplaceString(CString oname, CString *nname, LPSTR srch, LPSTR rep, BOOL bDiff)
{
	char old_fname[MAX_PATH];
	char new_fname[MAX_PATH];
	char tmp[MAX_PATH];
	LPSTR pfname = NULL;
	UINT cp_num=0;	//コピする数
	UINT cp_start=0;//コピする先頭配列
	BOOL rt = FALSE;

	memset(new_fname, 0, sizeof(new_fname));
	memset(tmp, 0, sizeof(tmp));

	strcpy_s(old_fname,sizeof(old_fname),(LPSTR)oname.GetString());	// オリジナルファイル名をコピ
	for(UINT i=0; i<strlen(old_fname); i++)	// 1文字ずつ検索
	{
		if ( bDiff )
		{
			rt = strncmp(&old_fname[i], srch, strlen(srch));	// 指定文字数で目的の文字列を検索
		}
		else
		{
			rt = _strnicmp(&old_fname[i], srch, strlen(srch));	// 指定文字数で目的の文字列を検索
		}

		if(rt == 0)	// 指定文字数で目的の文字列を検索
		{
			strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//見つかる前までの文字列
			strcat_s(new_fname, sizeof(new_fname), tmp);	//最終的な目的の文字列
			if(rep != NULL)
			{
				// 置換対象文字があるなら、ここで実行
				strcat_s(new_fname, sizeof(new_fname), rep);
			}
			i += strlen(srch) - 1; //みつかった文字列分の検索は飛ばす
			cp_num=0;	//キャッシュのカウントをリセットする
			cp_start = i + 1;
		}
		else
		{
			cp_num++;
		}
	}

	// 対象文字列以降の文字を連結
	if ( cp_num )
	{
		strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//見つかる前までの文字列
		strcat_s(new_fname, sizeof(new_fname), tmp);	//最終的な目的の文字列
	}
	*nname = new_fname;
}

BOOL Browse(HWND m_hWnd, CString title, CString *path)
{
	char folderName[MAX_PATH] = { 0 };
	BROWSEINFO browseInfo = { 0 };
	LPITEMIDLIST pidlSelected = NULL;
	BOOL rt = FALSE;

	// BROWSEINFO作成
	browseInfo.hwndOwner = m_hWnd;
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = folderName;
	browseInfo.lpszTitle = title.GetBuffer(STR_MIN_SIZE);
	browseInfo.ulFlags = 0;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0; 
	browseInfo.ulFlags = (BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS);

	pidlSelected = SHBrowseForFolder(&browseInfo);
	
	title.ReleaseBuffer();

	if ( pidlSelected != NULL )
	{
		SHGetPathFromIDList(pidlSelected, folderName);
		*path = folderName;
		rt = TRUE;
	}

	return rt;
}

BOOL SetDlgItemTextAll(HWND hDlg, const TABLE *pTbl, int iMax)
{
	int		i;
	char	szBuf[MAX_PATH];
	HINSTANCE hInst = GetModuleHandle(NULL);

	if ( ! pTbl )
		return FALSE;

	for( i=0; i < iMax; i++ )
	{
		if ( pTbl[i].iStrId )
		{
			LoadString(hInst, pTbl[i].iStrId, szBuf, sizeof(szBuf));
			SetDlgItemText(hDlg, pTbl[i].iCtrlId, szBuf);
		}
	}

	return TRUE;
}

int han2zen(char *str)
{
	char	*buf,*p,*ptr;

	buf=(char *)calloc(strlen(str)*2+1,sizeof(char));
	for(ptr=str,p=buf;*ptr!='\0';*ptr++){
		if(issjiskanji(*ptr)){
			/* SJIS漢字1バイト目の場合 */
			*p=*ptr;
			p++;
			ptr++;
			*p=*ptr;
			p++;
			*p='\0';
			continue;
		}

		switch((int)*ptr){
			case    ' ': strcpy(p,"　");p+=2;break;
			case    '!': strcpy(p,"！");p+=2;break;
			case    '"': strcpy(p,"”");p+=2;break;
			case    '#': strcpy(p,"＃");p+=2;break;
			case    '$': strcpy(p,"＄");p+=2;break;
			case    '%': strcpy(p,"％");p+=2;break;
			case    '&': strcpy(p,"＆");p+=2;break;
			case    '\'': strcpy(p,"’");p+=2;break;
			case    '(': strcpy(p,"（");p+=2;break;
			case    ')': strcpy(p,"）");p+=2;break;
			case    '*': strcpy(p,"＊");p+=2;break;
			case    '+': strcpy(p,"＋");p+=2;break;
			case    ',': strcpy(p,"，");p+=2;break;
			case    '-': strcpy(p,"－");p+=2;break;
			case    '.': strcpy(p,"．");p+=2;break;
			case    '/': strcpy(p,"／");p+=2;break;
			case    '0': strcpy(p,"０");p+=2;break;
			case    '1': strcpy(p,"１");p+=2;break;
			case    '2': strcpy(p,"２");p+=2;break;
			case    '3': strcpy(p,"３");p+=2;break;
			case    '4': strcpy(p,"４");p+=2;break;
			case    '5': strcpy(p,"５");p+=2;break;
			case    '6': strcpy(p,"６");p+=2;break;
			case    '7': strcpy(p,"７");p+=2;break;
			case    '8': strcpy(p,"８");p+=2;break;
			case    '9': strcpy(p,"９");p+=2;break;
			case    ':': strcpy(p,"：");p+=2;break;
			case    ';': strcpy(p,"；");p+=2;break;
			case    '<': strcpy(p,"＜");p+=2;break;
			case    '=': strcpy(p,"＝");p+=2;break;
			case    '>': strcpy(p,"＞");p+=2;break;
			case    '?': strcpy(p,"？");p+=2;break;
			case    '@': strcpy(p,"＠");p+=2;break;
			case    'A': strcpy(p,"Ａ");p+=2;break;
			case    'B': strcpy(p,"Ｂ");p+=2;break;
			case    'C': strcpy(p,"Ｃ");p+=2;break;
			case    'D': strcpy(p,"Ｄ");p+=2;break;
			case    'E': strcpy(p,"Ｅ");p+=2;break;
			case    'F': strcpy(p,"Ｆ");p+=2;break;
			case    'G': strcpy(p,"Ｇ");p+=2;break;
			case    'H': strcpy(p,"Ｈ");p+=2;break;
			case    'I': strcpy(p,"Ｉ");p+=2;break;
			case    'J': strcpy(p,"Ｊ");p+=2;break;
			case    'K': strcpy(p,"Ｋ");p+=2;break;
			case    'L': strcpy(p,"Ｌ");p+=2;break;
			case    'M': strcpy(p,"Ｍ");p+=2;break;
			case    'N': strcpy(p,"Ｎ");p+=2;break;
			case    'O': strcpy(p,"Ｏ");p+=2;break;
			case    'P': strcpy(p,"Ｐ");p+=2;break;
			case    'Q': strcpy(p,"Ｑ");p+=2;break;
			case    'R': strcpy(p,"Ｒ");p+=2;break;
			case    'S': strcpy(p,"Ｓ");p+=2;break;
			case    'T': strcpy(p,"Ｔ");p+=2;break;
			case    'U': strcpy(p,"Ｕ");p+=2;break;
			case    'V': strcpy(p,"Ｖ");p+=2;break;
			case    'W': strcpy(p,"Ｗ");p+=2;break;
			case    'X': strcpy(p,"Ｘ");p+=2;break;
			case    'Y': strcpy(p,"Ｙ");p+=2;break;
			case    'Z': strcpy(p,"Ｚ");p+=2;break;
			case    '[': strcpy(p,"［");p+=2;break;
			case    '\\': strcpy(p,"￥");p+=2;break;
			case    ']': strcpy(p,"］");p+=2;break;
			case    '^': strcpy(p,"＾");p+=2;break;
			case    '_': strcpy(p,"＿");p+=2;break;
			case    '`': strcpy(p,"‘");p+=2;break;
			case    'a': strcpy(p,"ａ");p+=2;break;
			case    'b': strcpy(p,"ｂ");p+=2;break;
			case    'c': strcpy(p,"ｃ");p+=2;break;
			case    'd': strcpy(p,"ｄ");p+=2;break;
			case    'e': strcpy(p,"ｅ");p+=2;break;
			case    'f': strcpy(p,"ｆ");p+=2;break;
			case    'g': strcpy(p,"ｇ");p+=2;break;
			case    'h': strcpy(p,"ｈ");p+=2;break;
			case    'i': strcpy(p,"ｉ");p+=2;break;
			case    'j': strcpy(p,"ｊ");p+=2;break;
			case    'k': strcpy(p,"ｋ");p+=2;break;
			case    'l': strcpy(p,"ｌ");p+=2;break;
			case    'm': strcpy(p,"ｍ");p+=2;break;
			case    'n': strcpy(p,"ｎ");p+=2;break;
			case    'o': strcpy(p,"ｏ");p+=2;break;
			case    'p': strcpy(p,"ｐ");p+=2;break;
			case    'q': strcpy(p,"ｑ");p+=2;break;
			case    'r': strcpy(p,"ｒ");p+=2;break;
			case    's': strcpy(p,"ｓ");p+=2;break;
			case    't': strcpy(p,"ｔ");p+=2;break;
			case    'u': strcpy(p,"ｕ");p+=2;break;
			case    'v': strcpy(p,"ｖ");p+=2;break;
			case    'w': strcpy(p,"ｗ");p+=2;break;
			case    'x': strcpy(p,"ｘ");p+=2;break;
			case    'y': strcpy(p,"ｙ");p+=2;break;
			case    'z': strcpy(p,"ｚ");p+=2;break;
			case    '{': strcpy(p,"｛");p+=2;break;
			case    '|': strcpy(p,"｜");p+=2;break;
			case    '}': strcpy(p,"｝");p+=2;break;
			default:
				*p=*ptr;
				p++;
				*p='\0';
				break;
		}
	}
	strcpy(str,buf);
	free(buf);

	return(0);
}

int zen2han(char *str)
{
char	*buf,*p,*ptr;

	buf=(char *)calloc(strlen(str)+1,sizeof(char));

	for(ptr=str,p=buf;*ptr!='\0';*ptr++){
		if(strncmp(ptr,"　",2)==0){*p=' ';p++;ptr++;}
		else if(strncmp(ptr,"！",2)==0){*p='!';p++;ptr++;}
		else if(strncmp(ptr,"”",2)==0){*p='"';p++;ptr++;}
		else if(strncmp(ptr,"＃",2)==0){*p='#';p++;ptr++;}
		else if(strncmp(ptr,"＄",2)==0){*p='$';p++;ptr++;}
		else if(strncmp(ptr,"％",2)==0){*p='%';p++;ptr++;}
		else if(strncmp(ptr,"＆",2)==0){*p='&';p++;ptr++;}
		else if(strncmp(ptr,"’",2)==0){*p='\'';p++;ptr++;}
		else if(strncmp(ptr,"（",2)==0){*p='(';p++;ptr++;}
		else if(strncmp(ptr,"）",2)==0){*p=')';p++;ptr++;}
		else if(strncmp(ptr,"＊",2)==0){*p='*';p++;ptr++;}
		else if(strncmp(ptr,"＋",2)==0){*p='+';p++;ptr++;}
		else if(strncmp(ptr,"，",2)==0){*p=',';p++;ptr++;}
		else if(strncmp(ptr,"－",2)==0){*p='-';p++;ptr++;}
		else if(strncmp(ptr,"．",2)==0){*p='.';p++;ptr++;}
		else if(strncmp(ptr,"／",2)==0){*p='/';p++;ptr++;}
		else if(strncmp(ptr,"０",2)==0){*p='0';p++;ptr++;}
		else if(strncmp(ptr,"１",2)==0){*p='1';p++;ptr++;}
		else if(strncmp(ptr,"２",2)==0){*p='2';p++;ptr++;}
		else if(strncmp(ptr,"３",2)==0){*p='3';p++;ptr++;}
		else if(strncmp(ptr,"４",2)==0){*p='4';p++;ptr++;}
		else if(strncmp(ptr,"５",2)==0){*p='5';p++;ptr++;}
		else if(strncmp(ptr,"６",2)==0){*p='6';p++;ptr++;}
		else if(strncmp(ptr,"７",2)==0){*p='7';p++;ptr++;}
		else if(strncmp(ptr,"８",2)==0){*p='8';p++;ptr++;}
		else if(strncmp(ptr,"９",2)==0){*p='9';p++;ptr++;}
		else if(strncmp(ptr,"：",2)==0){*p=':';p++;ptr++;}
		else if(strncmp(ptr,"；",2)==0){*p=';';p++;ptr++;}
		else if(strncmp(ptr,"＜",2)==0){*p='<';p++;ptr++;}
		else if(strncmp(ptr,"＝",2)==0){*p='=';p++;ptr++;}
		else if(strncmp(ptr,"＞",2)==0){*p='>';p++;ptr++;}
		else if(strncmp(ptr,"？",2)==0){*p='?';p++;ptr++;}
		else if(strncmp(ptr,"＠",2)==0){*p='@';p++;ptr++;}
		else if(strncmp(ptr,"Ａ",2)==0){*p='A';p++;ptr++;}
		else if(strncmp(ptr,"Ｂ",2)==0){*p='B';p++;ptr++;}
		else if(strncmp(ptr,"Ｃ",2)==0){*p='C';p++;ptr++;}
		else if(strncmp(ptr,"Ｄ",2)==0){*p='D';p++;ptr++;}
		else if(strncmp(ptr,"Ｅ",2)==0){*p='E';p++;ptr++;}
		else if(strncmp(ptr,"Ｆ",2)==0){*p='F';p++;ptr++;}
		else if(strncmp(ptr,"Ｇ",2)==0){*p='G';p++;ptr++;}
		else if(strncmp(ptr,"Ｈ",2)==0){*p='H';p++;ptr++;}
		else if(strncmp(ptr,"Ｉ",2)==0){*p='I';p++;ptr++;}
		else if(strncmp(ptr,"Ｊ",2)==0){*p='J';p++;ptr++;}
		else if(strncmp(ptr,"Ｋ",2)==0){*p='K';p++;ptr++;}
		else if(strncmp(ptr,"Ｌ",2)==0){*p='L';p++;ptr++;}
		else if(strncmp(ptr,"Ｍ",2)==0){*p='M';p++;ptr++;}
		else if(strncmp(ptr,"Ｎ",2)==0){*p='N';p++;ptr++;}
		else if(strncmp(ptr,"Ｏ",2)==0){*p='O';p++;ptr++;}
		else if(strncmp(ptr,"Ｐ",2)==0){*p='P';p++;ptr++;}
		else if(strncmp(ptr,"Ｑ",2)==0){*p='Q';p++;ptr++;}
		else if(strncmp(ptr,"Ｒ",2)==0){*p='R';p++;ptr++;}
		else if(strncmp(ptr,"Ｓ",2)==0){*p='S';p++;ptr++;}
		else if(strncmp(ptr,"Ｔ",2)==0){*p='T';p++;ptr++;}
		else if(strncmp(ptr,"Ｕ",2)==0){*p='U';p++;ptr++;}
		else if(strncmp(ptr,"Ｖ",2)==0){*p='V';p++;ptr++;}
		else if(strncmp(ptr,"Ｗ",2)==0){*p='W';p++;ptr++;}
		else if(strncmp(ptr,"Ｘ",2)==0){*p='X';p++;ptr++;}
		else if(strncmp(ptr,"Ｙ",2)==0){*p='Y';p++;ptr++;}
		else if(strncmp(ptr,"Ｚ",2)==0){*p='Z';p++;ptr++;}
		else if(strncmp(ptr,"［",2)==0){*p='[';p++;ptr++;}
		else if(strncmp(ptr,"￥",2)==0){*p='\\';p++;ptr++;}
		else if(strncmp(ptr,"］",2)==0){*p=']';p++;ptr++;}
		else if(strncmp(ptr,"＾",2)==0){*p='^';p++;ptr++;}
		else if(strncmp(ptr,"＿",2)==0){*p='_';p++;ptr++;}
		else if(strncmp(ptr,"‘",2)==0){*p='`';p++;ptr++;}
		else if(strncmp(ptr,"ａ",2)==0){*p='a';p++;ptr++;}
		else if(strncmp(ptr,"ｂ",2)==0){*p='b';p++;ptr++;}
		else if(strncmp(ptr,"ｃ",2)==0){*p='c';p++;ptr++;}
		else if(strncmp(ptr,"ｄ",2)==0){*p='d';p++;ptr++;}
		else if(strncmp(ptr,"ｅ",2)==0){*p='e';p++;ptr++;}
		else if(strncmp(ptr,"ｆ",2)==0){*p='f';p++;ptr++;}
		else if(strncmp(ptr,"ｇ",2)==0){*p='g';p++;ptr++;}
		else if(strncmp(ptr,"ｈ",2)==0){*p='h';p++;ptr++;}
		else if(strncmp(ptr,"ｉ",2)==0){*p='i';p++;ptr++;}
		else if(strncmp(ptr,"ｊ",2)==0){*p='j';p++;ptr++;}
		else if(strncmp(ptr,"ｋ",2)==0){*p='k';p++;ptr++;}
		else if(strncmp(ptr,"ｌ",2)==0){*p='l';p++;ptr++;}
		else if(strncmp(ptr,"ｍ",2)==0){*p='m';p++;ptr++;}
		else if(strncmp(ptr,"ｎ",2)==0){*p='n';p++;ptr++;}
		else if(strncmp(ptr,"ｏ",2)==0){*p='o';p++;ptr++;}
		else if(strncmp(ptr,"ｐ",2)==0){*p='p';p++;ptr++;}
		else if(strncmp(ptr,"ｑ",2)==0){*p='q';p++;ptr++;}
		else if(strncmp(ptr,"ｒ",2)==0){*p='r';p++;ptr++;}
		else if(strncmp(ptr,"ｓ",2)==0){*p='s';p++;ptr++;}
		else if(strncmp(ptr,"ｔ",2)==0){*p='t';p++;ptr++;}
		else if(strncmp(ptr,"ｕ",2)==0){*p='u';p++;ptr++;}
		else if(strncmp(ptr,"ｖ",2)==0){*p='v';p++;ptr++;}
		else if(strncmp(ptr,"ｗ",2)==0){*p='w';p++;ptr++;}
		else if(strncmp(ptr,"ｘ",2)==0){*p='x';p++;ptr++;}
		else if(strncmp(ptr,"ｙ",2)==0){*p='y';p++;ptr++;}
		else if(strncmp(ptr,"ｚ",2)==0){*p='z';p++;ptr++;}
		else if(strncmp(ptr,"｛",2)==0){*p='{';p++;ptr++;}
		else if(strncmp(ptr,"｜",2)==0){*p='|';p++;ptr++;}
		else if(strncmp(ptr,"｝",2)==0){*p='}';p++;ptr++;}
		else{ *p=*ptr; p++; }
	}
	strcpy(str,buf);
	free(buf);

	return(0);
}
