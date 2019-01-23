#ifndef _UTILS_
#define _UTILS_

// マクロ定義
#define STR_MIN_SIZE (10)
#define splitpath_s _splitpath_s
//#define splitpath() _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)

#define issjiskanji(c)	((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) \
		|| (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))


// 構造体定義
typedef struct
{
	int		iCtrlId;			//コントロールID
	int		iStrId;				//文字列リソースID (Caption)
} TABLE;



// プロトタイプ宣言
void MergePath(CString *path, CString dir, CString file, CString ext);
void SplitPath(CString path, CString *dir, CString *file, CString *ext);
void AppendPath(CString *path, CString more);
void AppendExt(CString *path, CString dist);
void ReplaceString(CString oname, CString *nname, LPSTR srch, LPSTR rep, BOOL bDiff);
BOOL Browse(HWND m_hWnd, CString title, CString *path);
BOOL SetDlgItemTextAll(HWND hDlg, const TABLE *pTbl, int iMax);
int han2zen(char *str);
int zen2han(char *str);



#endif