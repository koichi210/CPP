/////////////////////////////////////////////////////////////////////////////
// 共通のモジュールソース
/////////////////////////////////////////////////////////////////////////////

#include "tchar.h"
#include "stdafx.h"
#include "StandardTemplate.h"

#if USE_IMM_H
#include <imm.h> //ImmAssociateContext(),imm32.lib
#endif


/////////////////////////////////////////////////////////////////////////////
// Global関数定義
/////////////////////////////////////////////////////////////////////////////
// 文字列を結合してpathに格納
//void MyMergePath(
//				CString *cspPath	: [OUT] 結合したパス
//				CString csDrv		: [IN] 結合するドライブ名
//				CString csDir		: [IN] 結合するディレクトリ名
//				CString csFile		: [IN] 結合するファイル名
//				CString csExt		: [IN] 結合する拡張子
void MyMergePath(CString *cspPath, CString csDrv, CString csDir, CString csFile, CString csExt)
{
	// 出力先がNULLだったら終了
	if ( cspPath == NULL )
	{
		return;
	}

	// Drv文字列が指定されていたらセット
	if ( ! csDrv.IsEmpty() )
	{
		MyConnection(cspPath, csDrv);
	}

	// Dir文字列が指定されていたらセット
	if ( ! csDir.IsEmpty() )
	{
		MyConnection(cspPath, csDir);
	}

	// File文字列が指定されていたらセット
	if ( ! csFile.IsEmpty() )
	{
		MyConnection(cspPath, csFile);
	}

	// Ext文字列が指定されていたらセット
	if ( ! csExt.IsEmpty() )
	{
		AppendExt(cspPath, csExt);
	}
}


/////////////////////////////////////////////////////////////////////////////
// pathに格納されているフルパスを各文字列に分ける
//void MySplitPath(
//				CString csPath		: [IN] 分解元のパス
//				CString *cspDrv		: [OUT] 抽出したドライブ名
//				CString *cspDir		: [OUT] 抽出したディレクトリ名
//				CString *cspFile	: [OUT] 抽出したファイル名
//				CString *cspExt		: [OUT] 抽出した拡張子
void MySplitPath(CString csPath, CString *cspDrv, CString *cspDir, CString *cspFile, CString *cspExt)
{
	char wk_drv[MAX_PATH];
	char wk_dir[MAX_PATH];
	char wk_file[MAX_PATH];
	char wk_ext[MAX_PATH];
	
	// 対象文字列がないので終了
	if ( csPath.IsEmpty() )
	{
		return;
	}

	splitpath_s(csPath.GetString(), wk_drv, sizeof(wk_drv), wk_dir, sizeof(wk_dir), wk_file, sizeof(wk_file), wk_ext, sizeof(wk_ext));

	// Drv出力先がある & 代入する wk_drvがある
	if( cspDrv && strlen(wk_drv) )
	{
		*cspDrv = wk_drv;
	}

	// Dir出力先がある & 代入する wk_dirがある
	if( cspDir && strlen(wk_dir) )
	{
		*cspDir = wk_dir;
	}

	// File出力先がある & 代入する wk_fileがある
	if( cspFile && strlen(wk_file) )
	{
		*cspFile = wk_file;
	}

	// Ext出力先がある & 代入する wk_extがある
	if( cspExt && strlen(wk_ext) )
	{
		*cspExt = wk_ext;
	}
}


/////////////////////////////////////////////////////////////////////////////
// 文字を追加するとき、"\\"の付加要否を考慮
//void MyConnection(
//			CString *csFile1	: [OUT] 出力先
//			CString csFile2		: [IN] 入力元
void MyConnection(CString *csFile1, CString csFile2)
{
	if ( csFile1->IsEmpty() )
	{
		// 初めてセットする場合には、そのまま代入
		*csFile1 = csFile2;
	}
	else
	{
		// すでにセットされていた場合には、"\\"を追加する必要があるのでAppendPathを使用
		AppendPath(csFile1, csFile2);
	}
}


/////////////////////////////////////////////////////////////////////////////
// 文字を追加
//void AppendPath(
//			CString *path		: [OUT] 出力先
//			CString string		: [IN] 入力元
void AppendPath(CString *path, CString string)
{
	*path += "\\" + string;
}


/////////////////////////////////////////////////////////////////////////////
// 拡張子を追加
//void AppendExt(
//			CString *path		: [OUT] 出力先
//			CString string		: [IN] 入力元
void AppendExt(CString *path, CString string)
{
	*path += "." + string;
}


/////////////////////////////////////////////////////////////////////////////
// 文字列置換
// void ReplaceString(
//			CString oname		: [IN] 置換前のオリジナルファイル名
//			CString *nname		: [OUT] 置換後の新しいファイル名
//			LPSTR srch			: [IN] 検索対象文字列
//			LPSTR rep			: [IN] 置換対象の文字列
//			BOOL bDiffBigSmall	: [IN] 大文字小文字の区別可否(TRUE=区別しない)
void ReplaceString(CString oname, CString *nname, LPSTR srch, LPSTR rep, BOOL bDiffBigSmall)
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
		if ( bDiffBigSmall )
		{
			rt = strncmp(&old_fname[i], srch, strlen(srch));	// 指定文字数で目的の文字列を検索 & 大文字小文字を区別しない
		}
		else
		{
			rt = _strnicmp(&old_fname[i], srch, strlen(srch));	// 指定文字数で目的の文字列を検索 & 大文字小文字を区別する
		}

		if(rt == 0 )	// 指定文字数で目的の文字列を検索
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


/////////////////////////////////////////////////////////////////////////////
// ダイアログ
//BOOL Browse(
//		HWND m_hWnd		: [IN] ダイアログハンドル
//		CString title	: [IN] ダイアログのタイトル
//		CString *path	: [OUT] 取得したパス
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


/////////////////////////////////////////////////////////////////////////////
// ダイアログ内のコントロールに文字列を割り当てる
//BOOL SetDlgItemTextAll(
//			HWND hDlg			: [IN] ダイアログのハンドル
//			const TABLE *pTbl	: [IN] IDと文字列をセットにしたTABLE
//			int iMax			: [IN] TABLEのセット数
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


/////////////////////////////////////////////////////////////////////////////
// 半角文字を全角文字へ変換
//int han2zen(
//		char *str	: [IN/OUT] 変換対象の半角文字
void han2zen(char *str)
{
	char *buf,*p,*ptr;
	BOOL bLarge;

	buf = (char *)calloc(strlen(str)*2+1, sizeof(char));
	for(ptr=str, p=buf; *ptr!='\0'; *ptr++)
	{
		if ( issjiskanji(*ptr) )
		{
			/* SJIS漢字1バイト目の場合 */
			*p=*ptr;
			p++;
			ptr++;
			*p=*ptr;
			p++;
			*p='\0';
			continue;
		}

		bLarge = TRUE;
		switch((int)*ptr){
			case ' ': strcpy(p,"　"); break;
			case '!': strcpy(p,"！"); break;
			case '"': strcpy(p,"”"); break;
			case '#': strcpy(p,"＃"); break;
			case '$': strcpy(p,"＄"); break;
			case '%': strcpy(p,"％"); break;
			case '&': strcpy(p,"＆"); break;
			case '\'': strcpy(p,"’"); break;
			case '(': strcpy(p,"（"); break;
			case ')': strcpy(p,"）"); break;
			case '*': strcpy(p,"＊"); break;
			case '+': strcpy(p,"＋"); break;
			case ',': strcpy(p,"，"); break;
			case '-': strcpy(p,"－"); break;
			case '.': strcpy(p,"．"); break;
			case '/': strcpy(p,"／"); break;
			case '0': strcpy(p,"０"); break;
			case '1': strcpy(p,"１"); break;
			case '2': strcpy(p,"２"); break;
			case '3': strcpy(p,"３"); break;
			case '4': strcpy(p,"４"); break;
			case '5': strcpy(p,"５"); break;
			case '6': strcpy(p,"６"); break;
			case '7': strcpy(p,"７"); break;
			case '8': strcpy(p,"８"); break;
			case '9': strcpy(p,"９"); break;
			case ':': strcpy(p,"："); break;
			case ';': strcpy(p,"；"); break;
			case '<': strcpy(p,"＜"); break;
			case '=': strcpy(p,"＝"); break;
			case '>': strcpy(p,"＞"); break;
			case '?': strcpy(p,"？"); break;
			case '@': strcpy(p,"＠"); break;
			case 'A': strcpy(p,"Ａ"); break;
			case 'B': strcpy(p,"Ｂ"); break;
			case 'C': strcpy(p,"Ｃ"); break;
			case 'D': strcpy(p,"Ｄ"); break;
			case 'E': strcpy(p,"Ｅ"); break;
			case 'F': strcpy(p,"Ｆ"); break;
			case 'G': strcpy(p,"Ｇ"); break;
			case 'H': strcpy(p,"Ｈ"); break;
			case 'I': strcpy(p,"Ｉ"); break;
			case 'J': strcpy(p,"Ｊ"); break;
			case 'K': strcpy(p,"Ｋ"); break;
			case 'L': strcpy(p,"Ｌ"); break;
			case 'M': strcpy(p,"Ｍ"); break;
			case 'N': strcpy(p,"Ｎ"); break;
			case 'O': strcpy(p,"Ｏ"); break;
			case 'P': strcpy(p,"Ｐ"); break;
			case 'Q': strcpy(p,"Ｑ"); break;
			case 'R': strcpy(p,"Ｒ"); break;
			case 'S': strcpy(p,"Ｓ"); break;
			case 'T': strcpy(p,"Ｔ"); break;
			case 'U': strcpy(p,"Ｕ"); break;
			case 'V': strcpy(p,"Ｖ"); break;
			case 'W': strcpy(p,"Ｗ"); break;
			case 'X': strcpy(p,"Ｘ"); break;
			case 'Y': strcpy(p,"Ｙ"); break;
			case 'Z': strcpy(p,"Ｚ"); break;
			case '[': strcpy(p,"［"); break;
			case '\\': strcpy(p,"￥"); break;
			case ']': strcpy(p,"］"); break;
			case '^': strcpy(p,"＾"); break;
			case '_': strcpy(p,"＿"); break;
			case '`': strcpy(p,"‘"); break;
			case 'a': strcpy(p,"ａ"); break;
			case 'b': strcpy(p,"ｂ"); break;
			case 'c': strcpy(p,"ｃ"); break;
			case 'd': strcpy(p,"ｄ"); break;
			case 'e': strcpy(p,"ｅ"); break;
			case 'f': strcpy(p,"ｆ"); break;
			case 'g': strcpy(p,"ｇ"); break;
			case 'h': strcpy(p,"ｈ"); break;
			case 'i': strcpy(p,"ｉ"); break;
			case 'j': strcpy(p,"ｊ"); break;
			case 'k': strcpy(p,"ｋ"); break;
			case 'l': strcpy(p,"ｌ"); break;
			case 'm': strcpy(p,"ｍ"); break;
			case 'n': strcpy(p,"ｎ"); break;
			case 'o': strcpy(p,"ｏ"); break;
			case 'p': strcpy(p,"ｐ"); break;
			case 'q': strcpy(p,"ｑ"); break;
			case 'r': strcpy(p,"ｒ"); break;
			case 's': strcpy(p,"ｓ"); break;
			case 't': strcpy(p,"ｔ"); break;
			case 'u': strcpy(p,"ｕ"); break;
			case 'v': strcpy(p,"ｖ"); break;
			case 'w': strcpy(p,"ｗ"); break;
			case 'x': strcpy(p,"ｘ"); break;
			case 'y': strcpy(p,"ｙ"); break;
			case    'z': strcpy(p,"ｚ"); break;
			case    '{': strcpy(p,"｛"); break;
			case    '|': strcpy(p,"｜"); break;
			case    '}': strcpy(p,"｝"); break;
			default:
				*p=*ptr;
				*p='\0';
				bLarge = FALSE;
				break;
		}
		
		if ( bLarge )
		{
			p+=2;
		}
		else
		{
			p++;
		}
	}

	strcpy(str,buf);
	free(buf);
}


/////////////////////////////////////////////////////////////////////////////
// 全角文字を半角文字へ置換
//int zen2han(
//		char *str	: [IN/OUT] 変換対象の全角文字
void zen2han(char *str)
{
	char	*buf,*p,*ptr;
	BOOL	bLarge;

	buf=(char *)calloc(strlen(str)+1,sizeof(char));

	for(ptr=str, p=buf; *ptr!='\0'; *ptr++,p++)
	{
		bLarge = TRUE;
		if ( strncmp(ptr, "　", 2) == 0 ){*p=' ';}
		else if ( strncmp(ptr, "！", 2) == 0 ){*p='!';}
		else if ( strncmp(ptr, "”", 2) == 0 ){*p='"';}
		else if ( strncmp(ptr, "＃", 2) == 0 ){*p='#';}
		else if ( strncmp(ptr, "＄", 2) == 0 ){*p='$';}
		else if ( strncmp(ptr, "％", 2) == 0 ){*p='%';}
		else if ( strncmp(ptr, "＆", 2) == 0 ){*p='&';}
		else if ( strncmp(ptr, "’", 2) == 0 ){*p='\'';}
		else if ( strncmp(ptr, "（", 2) == 0 ){*p='(';}
		else if ( strncmp(ptr, "）", 2) == 0 ){*p=')';}
		else if ( strncmp(ptr, "＊", 2) == 0 ){*p='*';}
		else if ( strncmp(ptr, "＋", 2) == 0 ){*p='+';}
		else if ( strncmp(ptr, "，", 2) == 0 ){*p=',';}
		else if ( strncmp(ptr, "－", 2) == 0 ){*p='-';}
		else if ( strncmp(ptr, "．", 2) == 0 ){*p='.';}
		else if ( strncmp(ptr, "／", 2) == 0 ){*p='/';}
		else if ( strncmp(ptr, "０", 2) == 0 ){*p='0';}
		else if ( strncmp(ptr, "１", 2) == 0 ){*p='1';}
		else if ( strncmp(ptr, "２", 2) == 0 ){*p='2';}
		else if ( strncmp(ptr, "３", 2) == 0 ){*p='3';}
		else if ( strncmp(ptr, "４", 2) == 0 ){*p='4';}
		else if ( strncmp(ptr, "５", 2) == 0 ){*p='5';}
		else if ( strncmp(ptr, "６", 2) == 0 ){*p='6';}
		else if ( strncmp(ptr, "７", 2) == 0 ){*p='7';}
		else if ( strncmp(ptr, "８", 2) == 0 ){*p='8';}
		else if ( strncmp(ptr, "９", 2) == 0 ){*p='9';}
		else if ( strncmp(ptr, "：", 2) == 0 ){*p=':';}
		else if ( strncmp(ptr, "；", 2) == 0 ){*p=';';}
		else if ( strncmp(ptr, "＜", 2) == 0 ){*p='<';}
		else if ( strncmp(ptr, "＝", 2) == 0 ){*p='=';}
		else if ( strncmp(ptr, "＞", 2) == 0 ){*p='>';}
		else if ( strncmp(ptr, "？", 2) == 0 ){*p='?';}
		else if ( strncmp(ptr, "＠", 2) == 0 ){*p='@';}
		else if ( strncmp(ptr, "Ａ", 2) == 0 ){*p='A';}
		else if ( strncmp(ptr, "Ｂ", 2) == 0 ){*p='B';}
		else if ( strncmp(ptr, "Ｃ", 2) == 0 ){*p='C';}
		else if ( strncmp(ptr, "Ｄ", 2) == 0 ){*p='D';}
		else if ( strncmp(ptr, "Ｅ", 2) == 0 ){*p='E';}
		else if ( strncmp(ptr, "Ｆ", 2) == 0 ){*p='F';}
		else if ( strncmp(ptr, "Ｇ", 2) == 0 ){*p='G';}
		else if ( strncmp(ptr, "Ｈ", 2) == 0 ){*p='H';}
		else if ( strncmp(ptr, "Ｉ", 2) == 0 ){*p='I';}
		else if ( strncmp(ptr, "Ｊ", 2) == 0 ){*p='J';}
		else if ( strncmp(ptr, "Ｋ", 2) == 0 ){*p='K';}
		else if ( strncmp(ptr, "Ｌ", 2) == 0 ){*p='L';}
		else if ( strncmp(ptr, "Ｍ", 2) == 0 ){*p='M';}
		else if ( strncmp(ptr, "Ｎ", 2) == 0 ){*p='N';}
		else if ( strncmp(ptr, "Ｏ", 2) == 0 ){*p='O';}
		else if ( strncmp(ptr, "Ｐ", 2) == 0 ){*p='P';}
		else if ( strncmp(ptr, "Ｑ", 2) == 0 ){*p='Q';}
		else if ( strncmp(ptr, "Ｒ", 2) == 0 ){*p='R';}
		else if ( strncmp(ptr, "Ｓ", 2) == 0 ){*p='S';}
		else if ( strncmp(ptr, "Ｔ", 2) == 0 ){*p='T';}
		else if ( strncmp(ptr, "Ｕ", 2) == 0 ){*p='U';}
		else if ( strncmp(ptr, "Ｖ", 2) == 0 ){*p='V';}
		else if ( strncmp(ptr, "Ｗ", 2) == 0 ){*p='W';}
		else if ( strncmp(ptr, "Ｘ", 2) == 0 ){*p='X';}
		else if ( strncmp(ptr, "Ｙ", 2) == 0 ){*p='Y';}
		else if ( strncmp(ptr, "Ｚ", 2) == 0 ){*p='Z';}
		else if ( strncmp(ptr, "［", 2) == 0 ){*p='[';}
		else if ( strncmp(ptr, "￥", 2) == 0 ){*p='\\';}
		else if ( strncmp(ptr, "］", 2) == 0 ){*p=']';}
		else if ( strncmp(ptr, "＾", 2) == 0 ){*p='^';}
		else if ( strncmp(ptr, "＿", 2) == 0 ){*p='_';}
		else if ( strncmp(ptr, "‘", 2) == 0 ){*p='`';}
		else if ( strncmp(ptr, "ａ", 2) == 0 ){*p='a';}
		else if ( strncmp(ptr, "ｂ", 2) == 0 ){*p='b';}
		else if ( strncmp(ptr, "ｃ", 2) == 0 ){*p='c';}
		else if ( strncmp(ptr, "ｄ", 2) == 0 ){*p='d';}
		else if ( strncmp(ptr, "ｅ", 2) == 0 ){*p='e';}
		else if ( strncmp(ptr, "ｆ", 2) == 0 ){*p='f';}
		else if ( strncmp(ptr, "ｇ", 2) == 0 ){*p='g';}
		else if ( strncmp(ptr, "ｈ", 2) == 0 ){*p='h';}
		else if ( strncmp(ptr, "ｉ", 2) == 0 ){*p='i';}
		else if ( strncmp(ptr, "ｊ", 2) == 0 ){*p='j';}
		else if ( strncmp(ptr, "ｋ", 2) == 0 ){*p='k';}
		else if ( strncmp(ptr, "ｌ", 2) == 0 ){*p='l';}
		else if ( strncmp(ptr, "ｍ", 2) == 0 ){*p='m';}
		else if ( strncmp(ptr, "ｎ", 2) == 0 ){*p='n';}
		else if ( strncmp(ptr, "ｏ", 2) == 0 ){*p='o';}
		else if ( strncmp(ptr, "ｐ", 2) == 0 ){*p='p';}
		else if ( strncmp(ptr, "ｑ", 2) == 0 ){*p='q';}
		else if ( strncmp(ptr, "ｒ", 2) == 0 ){*p='r';}
		else if ( strncmp(ptr, "ｓ", 2) == 0 ){*p='s';}
		else if ( strncmp(ptr, "ｔ", 2) == 0 ){*p='t';}
		else if ( strncmp(ptr, "ｕ", 2) == 0 ){*p='u';}
		else if ( strncmp(ptr, "ｖ", 2) == 0 ){*p='v';}
		else if ( strncmp(ptr, "ｗ", 2) == 0 ){*p='w';}
		else if ( strncmp(ptr, "ｘ", 2) == 0 ){*p='x';}
		else if ( strncmp(ptr, "ｙ", 2) == 0 ){*p='y';}
		else if ( strncmp(ptr, "ｚ", 2) == 0 ){*p='z';}
		else if ( strncmp(ptr, "｛", 2) == 0 ){*p='{';}
		else if ( strncmp(ptr, "｜", 2) == 0 ){*p='|';}
		else if ( strncmp(ptr, "｝", 2) == 0 ){*p='}';}
		else{ *p=*ptr; bLarge=FALSE; }

		if ( bLarge ) {ptr++;}
	}
	strcpy(str,buf);
	free(buf);
}


/////////////////////////////////////////////////////////////////////////////
// Class定義
/////////////////////////////////////////////////////////////////////////////
// CMyStaticImage ビットマップを描画
CMyStaticImage::CMyStaticImage()
{
	m_idBitmap = 0;
}


CMyStaticImage::~CMyStaticImage()
{
}


BEGIN_MESSAGE_MAP(CMyStaticImage, CStatic)
	//{{AFX_MSG_MAP(CMyStaticImage)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// プロジェクト内に作成したビットマップのIDを渡す
void CMyStaticImage::SetBitmapId(int id)
{
	m_idBitmap = id;
}


// m_idBitmapで指定されているビットマップを描画する
void CMyStaticImage::OnPaint()
{
	CBitmap		bmp;

	if ( bmp.LoadBitmap(m_idBitmap) )
	{
		BITMAP		bm;
		RECT		rc;
		CBrush		br;
		CDC			dcOffScreen;	//off screen
		CDC			dcMask;			//mask
		CDC			dcImage;		//image
		CBitmap		bmpOffScreen;
		CBitmap		bmpMask;
		CBitmap		*bmpOldOffScreen;
		CBitmap		*oldBmpMask;
		CBitmap		*oldBmpImage;

		bmp.GetBitmap(&bm);

		GetClientRect(&rc);

		CPaintDC dc(this);

		// Create memory DCs to work with
		dcOffScreen.CreateCompatibleDC(&dc);
		dcMask.CreateCompatibleDC(&dc);
		dcImage.CreateCompatibleDC(&dc);

		// Create a monochrome bitmap for the mask
		bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

		// Select the mono bitmap into its DC
		oldBmpMask = dcMask.SelectObject(&bmpMask);

		// Select the image bitmap into its DC
		oldBmpImage = dcImage.SelectObject(&bmp);

		// Set the transparency color to be the top-left pixel
		dcImage.SetBkColor(dcImage.GetPixel(bm.bmWidth - 1, 0));

		// Make the mask
		dcMask.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);

		//Fill the background of the bitmap with BTNFACE color
		br.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		dc.FillRect(&rc, &br);

		// Create a bitmap for the off-screen DC that is really color compatible with the destination DC.
		bmpOffScreen.CreateBitmap(bm.bmWidth, bm.bmHeight, (BYTE)dc.GetDeviceCaps(PLANES), (BYTE)dc.GetDeviceCaps(BITSPIXEL), NULL);

		// Select the buffer bitmap into the off-screen DC
		bmpOldOffScreen = dcOffScreen.SelectObject(&bmpOffScreen);

		// Copy the image of the destination rectangle to the off-screen buffer DC so we can play with it
		dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc, 0, 0, SRCCOPY);

		// XOR the image with the destination
		dcOffScreen.SetBkColor(RGB(255, 255, 255)); //white
		dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCINVERT);

		// AND the destination with the mask
		dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcMask, 0,0, SRCAND);

		// XOR the destination with the image again
		dcOffScreen.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCINVERT);

		// Copy the resultant image back to the screen DC
//		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcOffScr, 0, 0, SRCCOPY);
		dc.StretchBlt(0, 0, rc.right - rc.left, rc.bottom - rc.top, &dcOffScreen, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		dcOffScreen.SelectObject(bmpOldOffScreen);
		dcImage.SelectObject(oldBmpImage);
		dcMask.SelectObject(oldBmpMask);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMyRistrictedSBCS SBCS文字の入力をコントロール（制限）する
CMyRistrictedSBCS::CMyRistrictedSBCS()
{
	m_dwCharKind = F_DEFAULT;
	m_pszExceptions = NULL;
	m_bDisableCopyAndPaste = FALSE;
}


CMyRistrictedSBCS::~CMyRistrictedSBCS()
{
	if ( m_pszExceptions ) free(m_pszExceptions);
}


BEGIN_MESSAGE_MAP(CMyRistrictedSBCS, CEdit)
	//{{AFX_MSG_MAP(CMyRistrictedSBCS)
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// 文字の種別を設定
void CMyRistrictedSBCS::SetCharacterKind(DWORD dwRange)
{
	m_dwCharKind = dwRange;
}


// 判定対象外（例外）として特定の文字を登録する
void CMyRistrictedSBCS::SetExceptionCharacters(char *pszExceptions)
{
	if ( pszExceptions )
	{
		int		len;

		len = lstrlen(pszExceptions);
		m_pszExceptions = (char *)malloc(len + 1);
		if ( m_pszExceptions )
		{
			lstrcpyn(m_pszExceptions, pszExceptions, len + 1);
		}
	}
}


// コピペを無効にする
void CMyRistrictedSBCS::DisableCopyAndPaste(BOOL bDisableCopyAndPaste)
{
	m_bDisableCopyAndPaste = bDisableCopyAndPaste;
}


// IMEを無効にする（別途ライブラリが必要）
void CMyRistrictedSBCS::OnSetFocus(CWnd *pOldWnd)
{
#if USE_IMM_H
	ImmAssociateContext(this->m_hWnd, NULL); //disable IME
#endif
	CEdit::OnSetFocus(pOldWnd);
}


// キー入力があったときに文字の判定
void CMyRistrictedSBCS::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL	bCallOnChar = TRUE;

	if ( ! iscntrl(nChar) ) //制御コード cf.仮想キーコード(VK_BACK, VK_TAB, etc, ...) ???
	{
		switch( m_dwCharKind )
		{
		case F_DIGIT :

			if ( ! isdigit(nChar) )
				bCallOnChar = FALSE;
			break;

		case F_DIGIT_SIGN :

			if ( nChar != '-' )
			{
				if ( ! isdigit(nChar) )
					bCallOnChar = FALSE;
			}
			break;

		case F_DECIMAL :

			if ( nChar != '.' )
			{
				if ( ! isdigit(nChar) )
					bCallOnChar = FALSE;
			}
			break;

		case F_DECIMAL_SIGN :

			if ( nChar != '.' && nChar != '-' )
			{
				if ( ! isdigit(nChar) )
					bCallOnChar = FALSE;
			}
			break;

		case F_ASCII :

			if ( 0x0080 <= nChar )
				bCallOnChar = FALSE;
			break;

		case F_DEFAULT : // no break
		default        :
			break;
		}

		// 例外指定されていた文字だったら、処理対象外とする
		if ( bCallOnChar && m_pszExceptions )
		{
			char	*p;

			for( p=m_pszExceptions; *p != '\0'; p++ )
			{
				if ( nChar == (BYTE)*p )
				{
					bCallOnChar = FALSE;
					break;
				}
			}
		}
	}

	// コピペが無効設定のとき、コピペされたら処理対象外とする
	if ( bCallOnChar && m_bDisableCopyAndPaste )
	{
		if ( nChar == 0x0003 || //Copy
			 nChar == 0x0016 || //Paste
			 nChar == 0x0018 )  //Cut
			bCallOnChar = FALSE;
	}

	if ( bCallOnChar ) CEdit::OnChar(nChar, nRepCnt, nFlags);
}


// コピペ無効設定のとき、コンテキストメニューでも無効にする
void CMyRistrictedSBCS::OnContextMenu(CWnd *pWnd, CPoint point)
{
	if ( ! m_bDisableCopyAndPaste ) CEdit::OnContextMenu(pWnd, point);
}


/////////////////////////////////////////////////////////////////////////////
// CMyEdit エディットボックスを作る
IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{
	m_Parent = NULL;
	m_State = CMyEdit::STATE_OK;
	m_Value = "";
	m_PossibleKind = USESTRING_ANY;
	m_MaxLength = 0;
}


CMyEdit::CMyEdit(CWnd *parent, CPoint position)
{
	m_Parent = parent;
	m_State = CMyEdit::STATE_OK;
	m_Value = "";
	m_Pos = position;
	m_PossibleKind = USESTRING_ANY;
	m_MaxLength = 0;
}


CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// エディットボックスを作成する
BOOL CMyEdit::Create(DWORD nKind, UINT nMaxLength, LPCTSTR str, const RECT &rect, CWnd *pParentWnd)
{
	BOOL	bReturn;

	bReturn = CreateEx(WS_EX_TOPMOST, _T("Edit"), str, WS_POPUP|WS_VISIBLE|ES_AUTOHSCROLL , rect, pParentWnd, NULL);
	if ( bReturn )
	{
		m_PossibleKind = nKind;
		m_MaxLength = nMaxLength;
		ShowWindow(SW_SHOW);
#if USE_IMM_H
		ImmAssociateContext(this->m_hWnd, NULL); //disable IME
#endif
		SetFocus();
	}

	return bReturn;
}


int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CEdit::OnCreate(lpCreateStruct) == -1 )
		return -1;

	if ( m_Parent != NULL )
	{
		SetFont(m_Parent->GetFont());
		SetSel(0, -1);
	}

	return 0;
}


void CMyEdit::OnDestroy()
{
	CWnd *parent = GetParent();

	GetWindowText(m_Value);

	if ( m_Parent != NULL )
		m_Parent->PostMessage(WM_MYEDIT_KILLFOCUS, (WPARAM)m_State, 0);
	else if ( parent != NULL )
		parent->PostMessage(WM_MYEDIT_KILLFOCUS, (WPARAM)m_State, 0);

	CEdit::OnDestroy();
}


void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL isDefOperation = FALSE;
	BOOL isCtrlString = FALSE;

	switch( nChar )
	{
	case VK_RETURN :
		m_State = CMyEdit::STATE_OK;
		DestroyWindow();
		break;

	case VK_ESCAPE :
		m_State = CMyEdit::STATE_NG;
		DestroyWindow();
		break;

	case VK_BACK :
		isDefOperation = TRUE;	// 文字削除は無条件で許可
		isCtrlString = TRUE;	// 制御文字の場合、文字数制限なし
		break;

	case VK_SPACE :
		isDefOperation = TRUE;	// スペースは無条件で許可
		break;

	default :
		if ( isdigit(nChar) && (m_PossibleKind & USESTRING_DIGIT) ||	// 数値入力を許可
			 isalpha(nChar) && (m_PossibleKind & USESTRING_ALPHA) ||	// a～z入力を許可
			 m_PossibleKind == USESTRING_ANY )							// すべての文字を許可
		{
			isDefOperation = TRUE;
			break;
		}
	}

	if ( isDefOperation )
	{
		if ( ! isCtrlString )
		{
			if ( m_MaxLength != 0 )
			{
				CString temp;
				GetWindowText(temp);
				if ( m_MaxLength <= temp.GetLength() )
					return ;
			}
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}


void CMyEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	// クリップボード経由で入力されると困るので、コンテキストメニューを表示させない。
	//CEdit::OnRButtonUp(nFlags, point);
}


void CMyEdit::OnKillFocus(CWnd *pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
//CMyList リストボックスを作る
CMyList::CMyList()
{
	m_Index = (-1);
	m_Parent = NULL;
}


CMyList::CMyList(CWnd *parent)
{
	m_Index = (-1);
	m_Parent = parent;
}


CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListBox)
	//{{AFX_MSG_MAP(CMyList)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// リストボックスを作成する
BOOL CMyList::Create(CString *strarray, DWORD num, const RECT &rect, CWnd *pParentWnd)
{
	BOOL	bReturn;
	DWORD	i;

	bReturn = CreateEx(WS_EX_TOPMOST, _T("LISTBOX"), "", WS_BORDER|WS_VISIBLE|WS_VSCROLL|WS_POPUP|LBS_NOINTEGRALHEIGHT, rect, pParentWnd, NULL, NULL);
	if ( bReturn )
	{
		for( i=0; i < num; i++ ) AddString(*(strarray + i));
		SetFocus();
	}

	return bReturn;
}


int CMyList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CListBox::OnCreate(lpCreateStruct) == -1 )
		return -1;

	// 親のフォントを設定
	if ( m_Parent != NULL )
		this->SetFont(m_Parent->GetFont());

	return 0;
}


void CMyList::OnDestroy()
{
	CListBox::OnDestroy();

	CWnd *parent = GetParent();

	if ( m_Parent != NULL )
		m_Parent->PostMessage(WM_MYLIST_KILLFOCUS, 0, 0);
	else if ( parent != NULL )
		parent->PostMessage(WM_MYLIST_KILLFOCUS, 0, 0);
}


void CMyList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar == VK_ESCAPE )
	{
		DestroyWindow();
	}
	else if ( nChar == VK_RETURN )
	{
		int index = GetCurSel();

		if ( index != LB_ERR )
			m_Index = index;

		DestroyWindow();
	}
	else
	{
		CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}


void CMyList::OnLButtonUp(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonUp(nFlags, point);

	int index = GetCurSel();

	if ( index != LB_ERR )
	{
		m_Index = index;
		DestroyWindow();
	}
}


void CMyList::OnSetFocus(CWnd *pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);
}


void CMyList::OnKillFocus(CWnd *pOldWnd)
{
	CListBox::OnKillFocus(pOldWnd);
	DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl リストコントロールを作る
IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{
	m_Pos = CPoint(0, 0);
	m_MyList = NULL;
	m_ListElements = NULL;
	m_NumElements = 0;
	m_EditKind = CMyEdit::USESTRING_ANY;
	m_MaxLength = 0;
	m_SelItem = 0;
	m_SelSubItem = 0;
	m_CurItem = 0;
	m_CurSubItem = 0;

	m_MyEdit = NULL;
	m_SideHeader = FALSE;
	SetLineColor();
}


CMyListCtrl::~CMyListCtrl()
{
	if ( m_ListElements != NULL )
		delete [] m_ListElements;
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_MYLIST_KILLFOCUS, OnMyListKillFocus)
	ON_MESSAGE(WM_MYEDIT_KILLFOCUS, OnMyEditKillFocus)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMyListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();

	if ( m_MyList != NULL )
		delete m_MyList;

	if ( m_MyEdit != NULL )
		delete m_MyEdit;
}


DWORD CMyListCtrl::SetElements(CString *strarray, DWORD num)
{
	this->m_ListElements = new CString[num];
	this->m_NumElements = num;

	for( DWORD i=0; i < num; i++ )
		*(m_ListElements + i) = *(strarray + i);

	return num;
}


void CMyListCtrl::SetEditPossible(DWORD nKind)
{
	m_EditKind = nKind;
}


void CMyListCtrl::CenterJustifyHeader()
{
	CHeaderCtrl *Head = GetHeaderCtrl();
	int nCount = Head->GetItemCount();
	HDITEM hdi;

	hdi.mask = HDI_FORMAT;
	hdi.fmt = (HDF_CENTER | HDF_STRING);

	for( int i=0; i < nCount; i++ )
	{
		Head->SetItem(i, &hdi);
	}
}


void CMyListCtrl::UseSideHeader(BOOL state)
{
	m_SideHeader = state;

	if ( state == TRUE && m_Pos.x == 0 )
		m_Pos.x = 1;
}


COLORREF CMyListCtrl::SetLineColor(COLORREF rgb)
{
	this->m_LineColor = rgb;
	return m_LineColor;
}


void CMyListCtrl::SetMaxLength(int nMaxLength)
{
	m_MaxLength = nMaxLength;
}


void CMyListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CHeaderCtrl *Head = GetHeaderCtrl();
	int xMax = Head->GetItemCount();
	int yMax = GetItemCount();
	BOOL isDefOperation = TRUE;
	CRect rect;

	// 何も選択していない状態のときもあるので、何かしら選択しているときのみキー判定を行うようにする
	if ( GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) != (-1) )
	{
		switch( nChar )
		{
		case VK_RIGHT :
			m_Pos.x ++;
			if ( xMax <= m_Pos.x )
				m_Pos.x = xMax - 1;
			break;

		case VK_LEFT :
			m_Pos.x --;
			if ( m_Pos.x == 0 && this->m_SideHeader == TRUE )
				m_Pos.x = 1;
			else if ( m_Pos.x < 0 )
				m_Pos.x = 0;
			break;

		case VK_DOWN :
			m_Pos.y ++;
			if ( yMax <= m_Pos.y )
				m_Pos.y = yMax - 1;
			break;

		case VK_UP :
			m_Pos.y --;
			if ( m_Pos.y < 0 )
				m_Pos.y = 0;
			break;

		case VK_SPACE :
			GetSubItemRect(m_Pos.y, m_Pos.x, LVIR_BOUNDS, rect);
			ClientToScreen(&rect);
			CreatePopup(m_Pos.x, rect);
			break;

		default :
			if ( UseInEditKey(nChar) )
			{
				GetSubItemRect(m_Pos.y, m_Pos.x, LVIR_BOUNDS, rect);
				ClientToScreen(&rect);
				CreatePopup(m_Pos.x, rect);
			}
			else
			{
				isDefOperation = FALSE;
			}
			break;
		}
	}

	if ( isDefOperation == TRUE )
	{
		GetSubItemRect(m_Pos.y, 0, LVIR_BOUNDS, rect) ;
		InvalidateRect(&rect);
		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}


BOOL CMyListCtrl::UseInEditKey(UINT nChar)
{
	return FALSE;
}


void CMyListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 文字入力で検索機能が働いてしまうので強制的にキー入力を無効にする
	//CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}


void CMyListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	LV_HITTESTINFO hittest;
	hittest.pt = point;

	if ( SubItemHitTest(&hittest) != (-1) )
	{
		CRect rect;
		int subitem = 0;

		m_SelItem = hittest.iItem;
		m_SelSubItem = hittest.iSubItem;

		if ( m_SideHeader == TRUE && hittest.iSubItem == 0 )
			subitem = 1;
		else
			subitem = hittest.iSubItem;

		m_Pos = CPoint(subitem, hittest.iItem);
		GetItemRect(hittest.iItem, &rect, LVIR_BOUNDS);

		SetFocus() ;
		SetItemState(hittest.iItem, LVIS_FOCUSED | LVIS_SELECTED , LVIS_FOCUSED | LVIS_SELECTED );

		InvalidateRect(&rect) ;
	}
	else
	{
		CListCtrl::OnLButtonDown(nFlags, point);
	}
}


void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	LV_HITTESTINFO hittest;
	hittest.pt = point;

	if ( SubItemHitTest(&hittest) != (-1) )
	{
		if ( hittest.iItem == m_SelItem && hittest.iSubItem == m_SelSubItem &&
			 hittest.iItem == m_CurItem && hittest.iSubItem == m_CurSubItem )
		{	// 2回目のクリックは「編集」
			CellEdit(hittest);
		}
		else
		{	// 1回目のクリックは「選択」
			m_CurItem = m_SelItem;
			m_CurSubItem = m_SelSubItem;
		}
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}

void CMyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LV_HITTESTINFO hittest;
	hittest.pt = point;

	if ( SubItemHitTest(&hittest) != (-1) )
	{
		CellEdit(hittest);
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}


void CMyListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// サイドヘッダを選択したときに不正なセルが選択状態になってしまうので。。
	if ( m_SideHeader )
	{
		return ;
	}

	CListCtrl::OnRButtonDown(nFlags, point);
}


void CMyListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// 再描画するItemの座標を取得
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int iItem = lpDrawItemStruct->itemID;
	TCHAR szBuff[MAX_PATH];
	LVITEM lvItem;
	int col = 0;
	CRect rect;

	//フォーカスがあるかどうか
	BOOL isForcus = GetItemState(iItem, LVIS_FOCUSED) == LVIS_FOCUSED;

	CHeaderCtrl *Head = GetHeaderCtrl();
	int numitem = Head->GetItemCount();

	while( numitem > col )
	{
		// アイテム取得のための設定
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = iItem;
		lvItem.iSubItem = col;
		lvItem.pszText = szBuff;
		lvItem.cchTextMax = sizeof(szBuff);
		lvItem.stateMask = LVIS_ACTIVATING | LVIS_FOCUSED | LVIS_SELECTED;

		if ( GetItem(&lvItem) == TRUE )
		{
			if ( col == 0 )
			{
				CRect itemrect;
				// アイテムのときにはGetSubItemだと全体を取ってきてしまうので特別扱いをする
				GetItemRect(iItem, &rect, LVIR_BOUNDS | LVIR_LABEL);
				GetItemRect(iItem, &itemrect, LVIR_BOUNDS);
				rect.top = itemrect.top;
				rect.left = itemrect.left;
			}
			else
			{
				GetSubItemRect(iItem, col, LVIR_BOUNDS, rect);
			}

			// 選択しているか否か
			DWORD txtColor = ::GetSysColor(COLOR_WINDOWTEXT);
			BOOL isSelect = FALSE;

			if ( GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED && col == m_Pos.x )
			{
				// 選択状態を表示する
				if ( col == 0 && m_SideHeader == TRUE )
				{
					pDC->FillRect(rect, &CBrush(::GetSysColor(COLOR_3DFACE)));
					pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT) );
					// pDC->DrawFrameControl(&rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_FLAT );

					txtColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
					isSelect = TRUE;
				}
				else
				{
					pDC->FillRect(rect, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
					txtColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
					isSelect = TRUE;
				}
			}
			else
			{
				if ( col == 0 && m_SideHeader == TRUE )
				{
					pDC->FillRect(rect, &CBrush(::GetSysColor(COLOR_3DFACE)));
					pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW) );
					// pDC->DrawFrameControl(&rect, DFC_BUTTON, DFCS_BUTTONPUSH);
					isSelect = TRUE;
				}
				else
				{
					// 選択されていない状態を表示する
					pDC->FillRect(rect, &CBrush(::GetSysColor(COLOR_WINDOW)));
				}
			}

			if ( col != 0 || m_SideHeader == FALSE )
			{
				CBrush border(m_LineColor);
				CRect framerect = rect;
				framerect.top -= 1;
				framerect.left -= 1;

				pDC->FrameRect(&framerect, &border);

				border.DeleteObject();
			}

			pDC->SetTextColor(txtColor);

			DWORD uFlag;
			if ( col == 0 && m_SideHeader == TRUE )
				uFlag = DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_CENTER;
			else
				uFlag = DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_LEFT;

			CRect strRect;
			strRect = rect;
			strRect.left += 2; // 文字位置微調整

			CString outText = lvItem.pszText;
			pDC->DrawText(outText, strRect, uFlag);
		}

		col++;
	}
}


void CMyListCtrl::CreatePopup(LONG col, CRect rect)
{
	return;
}


void CMyListCtrl::CreatePopupEditBox(CRect &rect)
{
	CRect framerect = rect;

	framerect.right -= 1;
	framerect.bottom -= 1;

	if ( m_MyEdit != NULL )
	{
		delete m_MyEdit;
		m_MyEdit = NULL;
	}

	m_MyEdit = new CMyEdit(this, m_Pos);
	m_MyEdit->Create(m_EditKind, m_MaxLength, GetItemText(m_Pos.y, m_Pos.x) ,framerect, this);
}


void CMyListCtrl::CreatePopupListBox(CRect &rect)
{
	CRect framerect = rect;

	framerect.top -= 1;
	framerect.left -= 1;

	if ( m_MyList != NULL )
	{
		delete m_MyList;
		m_MyList = NULL;
	}

	m_MyList = new CMyList(this);

	// リストの矩形はサブアイテムの矩形の高さを項目数分に高くしたもの
	framerect.bottom = framerect.top + framerect.Height() * this->m_NumElements;

	// ドロップダウンリストを作成
	// リストの項目、リストの項目数(5個)、リストの矩形、自分のウィンドウクラスポインタ
	m_MyList->Create(this->m_ListElements, this->m_NumElements, framerect, this);
}


void CMyListCtrl::CellEdit(LV_HITTESTINFO hittest)
{
	CRect rect;
	int subitem;

	m_Pos = CPoint(hittest.iSubItem, hittest.iItem);

	if ( m_SideHeader == TRUE && hittest.iSubItem == 0 )
		subitem = 1;
	else
		subitem = hittest.iSubItem;

	GetSubItemRect(hittest.iItem, hittest.iSubItem, LVIR_BOUNDS, rect);
	ClientToScreen(&rect);
	CreatePopup(hittest.iSubItem, rect);
}


LRESULT CMyListCtrl::OnMyEditKillFocus(WPARAM wParam,LPARAM lParam)
{
	if ( m_MyEdit )
	{
		if ( wParam == CMyEdit::STATE_OK )
			SetItemText(m_MyEdit->m_Pos.y, m_MyEdit->m_Pos.x, m_MyEdit->m_Value);

		delete m_MyEdit;
		m_MyEdit = NULL;
	}

	return TRUE;
}


LRESULT CMyListCtrl::OnMyListKillFocus(WPARAM wParam,LPARAM lParam)
{
	if ( m_MyList )
	{
		if ( m_MyList->m_Index != (-1) )
			SetItemText(m_Pos.y, m_Pos.x, m_ListElements[m_MyList->m_Index]);

		delete m_MyList;
		m_MyList = NULL;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CIconComboBox class
IMPLEMENT_DYNAMIC(CIconComboBox, CComboBox)

CIconComboBox::CIconComboBox()
{
	m_sizeIcon.cx = GetSystemMetrics(SM_CXICON); //32
	m_sizeIcon.cy = GetSystemMetrics(SM_CYICON); //32
	m_iHeight = m_sizeIcon.cy + 4; //2(margin) + 32(icon) + 2(margin)

	m_pItemList = NULL;
	m_uItemList = 0;
}


CIconComboBox::~CIconComboBox()
{
}


BEGIN_MESSAGE_MAP(CIconComboBox, CComboBox)
	//{{AFX_MSG_MAP(CIconComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CIconComboBox::SetItemList(ICONCOMBOBOXITEM *pItemList, UINT uItemList, UINT idxIcon, DWORD dwConstraint)
{
	m_pItemList = pItemList;
	m_uItemList = uItemList;
	m_idxIcon = idxIcon;

	ResetContent();
	SetItemHeight(-1, m_iHeight);

	for( UINT i=0, idx=0; i < m_uItemList; i++ )
	{
		pItemList[i].idxItem = -1;	//初期化

		//dwConstraint は使えない全てのビットが立っている
		if ( pItemList[i].dwConstraint & dwConstraint )
			continue;	//表示制限有り

		idx = InsertString(idx, "");
		if ( idx != CB_ERR )
		{
			SetItemData(idx, pItemList[i].value);
			SetItemHeight(i, m_iHeight);
			pItemList[i].idxItem = idx;	//追加したアイテムに対しidxを付与する
			idx++;
		}
	}
}


void CIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC		dc;

	if ( ! lpDrawItemStruct )
		return;

	if ( lpDrawItemStruct->itemID != -1 && lpDrawItemStruct->itemID < m_uItemList )
	{
		if ( dc.Attach(lpDrawItemStruct->hDC) )
		{
			POINT	pt;
			UINT	uFlags;
			HICON	hIcon;
			CString	strText;
			UINT	tempID = lpDrawItemStruct->itemID;

			//表示制限があった場合、テーブルのidx値を設定してあげる必要がある
			if ( lpDrawItemStruct->itemID != m_pItemList[lpDrawItemStruct->itemID].idxItem )
			{
				for( UINT i=0; i < m_uItemList; i++ )
				{
					if ( lpDrawItemStruct->itemID == m_pItemList[i].idxItem )
					{
						tempID = i;
						break;
					}
				}
			}

			pt.x = lpDrawItemStruct->rcItem.left + 2; //margin
			pt.y = lpDrawItemStruct->rcItem.top  + 2; //margin

			if ( lpDrawItemStruct->itemState & ODS_SELECTED )
			{
				dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
				dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
				uFlags = DSS_NORMAL;
			}
			else if ( lpDrawItemStruct->itemState & ODS_DISABLED )
			{
				dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_MENU));
				uFlags = DSS_DISABLED;
			}
			else
			{
				dc.SetTextColor(GetSysColor(COLOR_MENUTEXT));
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
				uFlags = DSS_NORMAL;
			}

			hIcon = AfxGetApp()->LoadIcon(m_pItemList[tempID].idIcon[m_idxIcon]);
			if ( hIcon ) dc.DrawState(pt, m_sizeIcon, hIcon, uFlags, (HBRUSH)NULL);

			strText.LoadString(m_pItemList[tempID].idText);
			lpDrawItemStruct->rcItem.left += m_iHeight;
			dc.DrawText(strText, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);

			dc.Detach();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMyEvent キーボード or マウスの入力をエミューレートする
CMyEvent::CMyEvent()
{
}


void CMyEvent::MouseMove(CPoint pt)
{
	#define MI_H(x) (x * 65535 / (::GetSystemMetrics(SM_CXSCREEN) - 1))
	#define MI_V(y) (y * 65535 / (::GetSystemMetrics(SM_CYSCREEN) - 1))
	INPUT input;

	input.type				= INPUT_MOUSE;
    input.mi.dx				= MI_H(pt.x);
    input.mi.dy				= MI_V(pt.y);
    input.mi.mouseData		= 0;
    input.mi.dwFlags		= MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    input.mi.time			= 0;
    input.mi.dwExtraInfo	= 0;

	::SendInput(1, &input, sizeof(INPUT));
}


void CMyEvent::MouseLButtonDown()
{
	INPUT input;

	input.type				= INPUT_MOUSE;
    input.mi.dx				= 0;
    input.mi.dy				= 0;
    input.mi.mouseData		= 0;
    input.mi.dwFlags		= MOUSEEVENTF_LEFTDOWN;
    input.mi.time			= 0;
    input.mi.dwExtraInfo	= 0;

	::SendInput(1, &input, sizeof(INPUT));
}


void CMyEvent::MouseLButtonUp()
{
	INPUT input;

	input.type				= INPUT_MOUSE;
    input.mi.dx				= 0;
    input.mi.dy				= 0;
    input.mi.mouseData		= 0;
    input.mi.dwFlags		= MOUSEEVENTF_LEFTUP;
    input.mi.time			= 0;
    input.mi.dwExtraInfo	= 0;

    ::SendInput(1, &input, sizeof(INPUT));
}


void CMyEvent::MouseLButtonClick()
{
	MouseLButtonDown();
	MouseLButtonUp();
}


void CMyEvent::MouseRButtonDown()
{
	INPUT input;

	input.type				= INPUT_MOUSE;
    input.mi.dx				= 0;
    input.mi.dy				= 0;
    input.mi.mouseData		= 0;
    input.mi.dwFlags		= MOUSEEVENTF_RIGHTDOWN;
    input.mi.time			= 0;
    input.mi.dwExtraInfo	= 0;
 
	::SendInput(1, &input, sizeof(INPUT));
}


void CMyEvent::MouseRButtonUp()
{
	INPUT input;

	input.type				= INPUT_MOUSE;
    input.mi.dx				= 0;
    input.mi.dy				= 0;
    input.mi.mouseData		= 0;
    input.mi.dwFlags		= MOUSEEVENTF_RIGHTUP;
    input.mi.time			= 0;
    input.mi.dwExtraInfo	= 0;

    ::SendInput(1, &input, sizeof(INPUT));
}


void CMyEvent::MouseRButtonClick()
{
	MouseRButtonDown();
	MouseRButtonUp();
}


void CMyEvent::KeyAction(WORD VirtualKey, BOOL bHold)
{
	if ( 0x61 <= VirtualKey && VirtualKey <= 0x7a )
		VirtualKey -= 0x20;

	SendInputKey(VirtualKey, bHold);
}


void CMyEvent::FunctionKeyAction(BYTE VirtualKey, BOOL bHold)
{
	if ( VirtualKey != VK_NONE )
	{
		SendInputKey(VirtualKey, bHold);
	}
}


void CMyEvent::SendInputKey(WORD VirtualKey, BOOL bHold)
{
	/*
	 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	 * 0x40 : unassigned
	 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	#define VK_SHIFT          0x10
	#define VK_CONTROL        0x11
	#define VK_MENU           0x12
	#define VK_ESCAPE         0x1B
	#define VK_F1             0x70
	*/
	/*
	#define VK_LEFT           0x25
	#define VK_UP             0x26
	#define VK_RIGHT          0x27
	#define VK_DOWN           0x28
	*/

	INPUT input;
	input.type				= INPUT_KEYBOARD;
	input.ki.wVk			= VirtualKey;
	input.ki.wScan			= MapVirtualKey(input.ki.wVk, 0);
	input.ki.dwFlags		= KEYEVENTF_EXTENDEDKEY;
	input.ki.time			= 0;
	input.ki.dwExtraInfo	= ::GetMessageExtraInfo();

	::SendInput(1, &input, sizeof(INPUT));

	//押し続け(Shift,Ctrl,Alt等)
	if ( ! bHold )
    {
        input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
        ::SendInput(1, &input, sizeof(INPUT));
     }
}

/////////////////////////////////////////////////////////////////////////////
// CCustomCtrl 自前の表を作成する
CCustomCtrl::CCustomCtrl()
{
	m_nMaxColumn = 1;
	m_nMaxRow = 1;
}


void CCustomCtrl::SetLabel(int nIdx, int nWidth, CString name)
{
	LV_COLUMN	lvCol;

	// Column共通設定
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	lvCol.fmt  = LVCFMT_LEFT;

	// ラベル設定
	lvCol.iSubItem = nIdx;
	lvCol.cx   = nWidth;
	lvCol.pszText  = (LPSTR)name.GetString();
	InsertColumn(lvCol.iSubItem, &lvCol);
}


void CCustomCtrl::SetMaxColumnNum(int num)
{
	m_nMaxColumn = num;
}


void CCustomCtrl::SetMaxRowNum(int num)
{
	m_nMaxRow = num;
}


void CCustomCtrl::SelectIdx(int nIdx)
{
	LV_ITEM	lvItem;

	lvItem.mask      = LVIF_TEXT | LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state     = 0;
	lvItem.iSubItem = nIdx;

	SetItemState(nIdx, &lvItem);
	SetSelectionMark(nIdx);
}


void CCustomCtrl::FillRect(int nIdx, CString name)
{
	LV_ITEM	lvItem;

	lvItem.mask      = LVIF_TEXT | LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state     = 0;
	lvItem.iSubItem = nIdx;

	for( int idx=0; idx < m_nMaxColumn; idx++ )
	{
		lvItem.iItem = idx;

		if ( nIdx == 0 )
		{
			CString tmp;
			tmp.Format("%d",idx+1);
			lvItem.pszText   = (LPSTR)tmp.GetString();
			InsertItem(&lvItem);
		}
		else
		{
			lvItem.pszText   = (LPSTR)name.GetString();
			SetItem(&lvItem);
		}
	}
}


void CCustomCtrl::EnableSelectRowAll()
{
	//行で選択するように指定
	SetExtendedStyle(LVS_EX_FULLROWSELECT);
	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}


/////////////////////////////////////////////////////////////////////////////
// CMyCompareFile 2つのファイルを比較する
CMyCompareFile::CMyCompareFile()
{
}


CMyCompareFile::CMyCompareFile(CString file1, CString file2)
{
	m_file1 = file1;
	m_file2 = file2;
}


CMyCompareFile::~CMyCompareFile()
{
}


BOOL CMyCompareFile::CompareBinary()
{
	BOOL bReturn = Open();
	
	if ( bReturn )
	{
		bReturn = IsSameFile();
	}
	
	Close();

	return bReturn;
}


BOOL CMyCompareFile::CompareBinary(CString strFile1, CString strFile2)
{
	m_file1 = strFile1;
	m_file2 = strFile2;

	return CompareBinary();
}


BOOL CMyCompareFile::Open()
{
	BOOL bReturn = TRUE;

	m_err = NO_ERROR;
	if ( ! m_cfile1.Open(m_file1, CFile::typeBinary) )
	{
		m_err = FILE1_OPEN_ERROR;
		bReturn = FALSE;
	}

	if ( ! m_cfile2.Open(m_file2, CFile::typeBinary) )
	{
		m_cfile1.Close();
		m_err = FILE2_OPEN_ERROR;
		bReturn = FALSE;
	}

	return bReturn;
}


void CMyCompareFile::Close()
{
	if ( m_cfile1.m_pStream != NULL )
	{
		m_cfile1.Close();
	}

	if ( m_cfile2.m_pStream != NULL )
	{
		m_cfile2.Close();
	}
}


BOOL CMyCompareFile::IsSameFile()
{
	BOOL bReturn = FALSE;

	char *pbuf1;
	char *pbuf2;
	size_t szFile1;
	size_t szFile2;
	
	// openしたまま何度か参照する場合、ファイルポインタが後ろに行ってるのでSeekToBeginで初期値に戻す
	//m_cfile1.SeekToBegin();
	//m_cfile2.SeekToBegin();
	szFile1 = (size_t)m_cfile1.GetLength();
	szFile2 = (size_t)m_cfile2.GetLength();
	
	// ファイルサイズが同じ
	if ( szFile1 == szFile2 )
	{
		pbuf1 = (char *)calloc(szFile1, sizeof(char));
		pbuf2 = (char *)calloc(szFile2, sizeof(char));

		m_cfile1.Read(pbuf1, szFile1);
		m_cfile2.Read(pbuf2, szFile2);

		// バイナリデータも同じ
		if ( memcmp(pbuf1, pbuf2, szFile1) == 0 )
		{
			bReturn = TRUE;
		}
		free(pbuf1);
		free(pbuf2);
	}

	return bReturn;
}