// ���ʂ̃��[�e�B�e�B
#include "tchar.h"
#include "stdafx.h"
#include "utils.h"

// ���������������path�Ɋi�[
void MergePath(CString *path, CString dir, CString file, CString ext)
{
	*path = dir;
	AppendPath(path,file);
	AppendExt(path,ext);
}

// path�Ɋi�[����Ă���t���p�X���e������ɕ�����
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
	UINT cp_num=0;	//�R�s���鐔
	UINT cp_start=0;//�R�s����擪�z��
	BOOL rt = FALSE;

	memset(new_fname, 0, sizeof(new_fname));
	memset(tmp, 0, sizeof(tmp));

	strcpy_s(old_fname,sizeof(old_fname),(LPSTR)oname.GetString());	// �I���W�i���t�@�C�������R�s
	for(UINT i=0; i<strlen(old_fname); i++)	// 1����������
	{
		if ( bDiff )
		{
			rt = strncmp(&old_fname[i], srch, strlen(srch));	// �w�蕶�����ŖړI�̕����������
		}
		else
		{
			rt = _strnicmp(&old_fname[i], srch, strlen(srch));	// �w�蕶�����ŖړI�̕����������
		}

		if(rt == 0)	// �w�蕶�����ŖړI�̕����������
		{
			strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//������O�܂ł̕�����
			strcat_s(new_fname, sizeof(new_fname), tmp);	//�ŏI�I�ȖړI�̕�����
			if(rep != NULL)
			{
				// �u���Ώە���������Ȃ�A�����Ŏ��s
				strcat_s(new_fname, sizeof(new_fname), rep);
			}
			i += strlen(srch) - 1; //�݂����������񕪂̌����͔�΂�
			cp_num=0;	//�L���b�V���̃J�E���g�����Z�b�g����
			cp_start = i + 1;
		}
		else
		{
			cp_num++;
		}
	}

	// �Ώە�����ȍ~�̕�����A��
	if ( cp_num )
	{
		strncpy_s(tmp, sizeof(tmp), &old_fname[cp_start], cp_num);	//������O�܂ł̕�����
		strcat_s(new_fname, sizeof(new_fname), tmp);	//�ŏI�I�ȖړI�̕�����
	}
	*nname = new_fname;
}

BOOL Browse(HWND m_hWnd, CString title, CString *path)
{
	char folderName[MAX_PATH] = { 0 };
	BROWSEINFO browseInfo = { 0 };
	LPITEMIDLIST pidlSelected = NULL;
	BOOL rt = FALSE;

	// BROWSEINFO�쐬
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
			/* SJIS����1�o�C�g�ڂ̏ꍇ */
			*p=*ptr;
			p++;
			ptr++;
			*p=*ptr;
			p++;
			*p='\0';
			continue;
		}

		switch((int)*ptr){
			case    ' ': strcpy(p,"�@");p+=2;break;
			case    '!': strcpy(p,"�I");p+=2;break;
			case    '"': strcpy(p,"�h");p+=2;break;
			case    '#': strcpy(p,"��");p+=2;break;
			case    '$': strcpy(p,"��");p+=2;break;
			case    '%': strcpy(p,"��");p+=2;break;
			case    '&': strcpy(p,"��");p+=2;break;
			case    '\'': strcpy(p,"�f");p+=2;break;
			case    '(': strcpy(p,"�i");p+=2;break;
			case    ')': strcpy(p,"�j");p+=2;break;
			case    '*': strcpy(p,"��");p+=2;break;
			case    '+': strcpy(p,"�{");p+=2;break;
			case    ',': strcpy(p,"�C");p+=2;break;
			case    '-': strcpy(p,"�|");p+=2;break;
			case    '.': strcpy(p,"�D");p+=2;break;
			case    '/': strcpy(p,"�^");p+=2;break;
			case    '0': strcpy(p,"�O");p+=2;break;
			case    '1': strcpy(p,"�P");p+=2;break;
			case    '2': strcpy(p,"�Q");p+=2;break;
			case    '3': strcpy(p,"�R");p+=2;break;
			case    '4': strcpy(p,"�S");p+=2;break;
			case    '5': strcpy(p,"�T");p+=2;break;
			case    '6': strcpy(p,"�U");p+=2;break;
			case    '7': strcpy(p,"�V");p+=2;break;
			case    '8': strcpy(p,"�W");p+=2;break;
			case    '9': strcpy(p,"�X");p+=2;break;
			case    ':': strcpy(p,"�F");p+=2;break;
			case    ';': strcpy(p,"�G");p+=2;break;
			case    '<': strcpy(p,"��");p+=2;break;
			case    '=': strcpy(p,"��");p+=2;break;
			case    '>': strcpy(p,"��");p+=2;break;
			case    '?': strcpy(p,"�H");p+=2;break;
			case    '@': strcpy(p,"��");p+=2;break;
			case    'A': strcpy(p,"�`");p+=2;break;
			case    'B': strcpy(p,"�a");p+=2;break;
			case    'C': strcpy(p,"�b");p+=2;break;
			case    'D': strcpy(p,"�c");p+=2;break;
			case    'E': strcpy(p,"�d");p+=2;break;
			case    'F': strcpy(p,"�e");p+=2;break;
			case    'G': strcpy(p,"�f");p+=2;break;
			case    'H': strcpy(p,"�g");p+=2;break;
			case    'I': strcpy(p,"�h");p+=2;break;
			case    'J': strcpy(p,"�i");p+=2;break;
			case    'K': strcpy(p,"�j");p+=2;break;
			case    'L': strcpy(p,"�k");p+=2;break;
			case    'M': strcpy(p,"�l");p+=2;break;
			case    'N': strcpy(p,"�m");p+=2;break;
			case    'O': strcpy(p,"�n");p+=2;break;
			case    'P': strcpy(p,"�o");p+=2;break;
			case    'Q': strcpy(p,"�p");p+=2;break;
			case    'R': strcpy(p,"�q");p+=2;break;
			case    'S': strcpy(p,"�r");p+=2;break;
			case    'T': strcpy(p,"�s");p+=2;break;
			case    'U': strcpy(p,"�t");p+=2;break;
			case    'V': strcpy(p,"�u");p+=2;break;
			case    'W': strcpy(p,"�v");p+=2;break;
			case    'X': strcpy(p,"�w");p+=2;break;
			case    'Y': strcpy(p,"�x");p+=2;break;
			case    'Z': strcpy(p,"�y");p+=2;break;
			case    '[': strcpy(p,"�m");p+=2;break;
			case    '\\': strcpy(p,"��");p+=2;break;
			case    ']': strcpy(p,"�n");p+=2;break;
			case    '^': strcpy(p,"�O");p+=2;break;
			case    '_': strcpy(p,"�Q");p+=2;break;
			case    '`': strcpy(p,"�e");p+=2;break;
			case    'a': strcpy(p,"��");p+=2;break;
			case    'b': strcpy(p,"��");p+=2;break;
			case    'c': strcpy(p,"��");p+=2;break;
			case    'd': strcpy(p,"��");p+=2;break;
			case    'e': strcpy(p,"��");p+=2;break;
			case    'f': strcpy(p,"��");p+=2;break;
			case    'g': strcpy(p,"��");p+=2;break;
			case    'h': strcpy(p,"��");p+=2;break;
			case    'i': strcpy(p,"��");p+=2;break;
			case    'j': strcpy(p,"��");p+=2;break;
			case    'k': strcpy(p,"��");p+=2;break;
			case    'l': strcpy(p,"��");p+=2;break;
			case    'm': strcpy(p,"��");p+=2;break;
			case    'n': strcpy(p,"��");p+=2;break;
			case    'o': strcpy(p,"��");p+=2;break;
			case    'p': strcpy(p,"��");p+=2;break;
			case    'q': strcpy(p,"��");p+=2;break;
			case    'r': strcpy(p,"��");p+=2;break;
			case    's': strcpy(p,"��");p+=2;break;
			case    't': strcpy(p,"��");p+=2;break;
			case    'u': strcpy(p,"��");p+=2;break;
			case    'v': strcpy(p,"��");p+=2;break;
			case    'w': strcpy(p,"��");p+=2;break;
			case    'x': strcpy(p,"��");p+=2;break;
			case    'y': strcpy(p,"��");p+=2;break;
			case    'z': strcpy(p,"��");p+=2;break;
			case    '{': strcpy(p,"�o");p+=2;break;
			case    '|': strcpy(p,"�b");p+=2;break;
			case    '}': strcpy(p,"�p");p+=2;break;
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
		if(strncmp(ptr,"�@",2)==0){*p=' ';p++;ptr++;}
		else if(strncmp(ptr,"�I",2)==0){*p='!';p++;ptr++;}
		else if(strncmp(ptr,"�h",2)==0){*p='"';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='#';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='$';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='%';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='&';p++;ptr++;}
		else if(strncmp(ptr,"�f",2)==0){*p='\'';p++;ptr++;}
		else if(strncmp(ptr,"�i",2)==0){*p='(';p++;ptr++;}
		else if(strncmp(ptr,"�j",2)==0){*p=')';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='*';p++;ptr++;}
		else if(strncmp(ptr,"�{",2)==0){*p='+';p++;ptr++;}
		else if(strncmp(ptr,"�C",2)==0){*p=',';p++;ptr++;}
		else if(strncmp(ptr,"�|",2)==0){*p='-';p++;ptr++;}
		else if(strncmp(ptr,"�D",2)==0){*p='.';p++;ptr++;}
		else if(strncmp(ptr,"�^",2)==0){*p='/';p++;ptr++;}
		else if(strncmp(ptr,"�O",2)==0){*p='0';p++;ptr++;}
		else if(strncmp(ptr,"�P",2)==0){*p='1';p++;ptr++;}
		else if(strncmp(ptr,"�Q",2)==0){*p='2';p++;ptr++;}
		else if(strncmp(ptr,"�R",2)==0){*p='3';p++;ptr++;}
		else if(strncmp(ptr,"�S",2)==0){*p='4';p++;ptr++;}
		else if(strncmp(ptr,"�T",2)==0){*p='5';p++;ptr++;}
		else if(strncmp(ptr,"�U",2)==0){*p='6';p++;ptr++;}
		else if(strncmp(ptr,"�V",2)==0){*p='7';p++;ptr++;}
		else if(strncmp(ptr,"�W",2)==0){*p='8';p++;ptr++;}
		else if(strncmp(ptr,"�X",2)==0){*p='9';p++;ptr++;}
		else if(strncmp(ptr,"�F",2)==0){*p=':';p++;ptr++;}
		else if(strncmp(ptr,"�G",2)==0){*p=';';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='<';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='=';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='>';p++;ptr++;}
		else if(strncmp(ptr,"�H",2)==0){*p='?';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='@';p++;ptr++;}
		else if(strncmp(ptr,"�`",2)==0){*p='A';p++;ptr++;}
		else if(strncmp(ptr,"�a",2)==0){*p='B';p++;ptr++;}
		else if(strncmp(ptr,"�b",2)==0){*p='C';p++;ptr++;}
		else if(strncmp(ptr,"�c",2)==0){*p='D';p++;ptr++;}
		else if(strncmp(ptr,"�d",2)==0){*p='E';p++;ptr++;}
		else if(strncmp(ptr,"�e",2)==0){*p='F';p++;ptr++;}
		else if(strncmp(ptr,"�f",2)==0){*p='G';p++;ptr++;}
		else if(strncmp(ptr,"�g",2)==0){*p='H';p++;ptr++;}
		else if(strncmp(ptr,"�h",2)==0){*p='I';p++;ptr++;}
		else if(strncmp(ptr,"�i",2)==0){*p='J';p++;ptr++;}
		else if(strncmp(ptr,"�j",2)==0){*p='K';p++;ptr++;}
		else if(strncmp(ptr,"�k",2)==0){*p='L';p++;ptr++;}
		else if(strncmp(ptr,"�l",2)==0){*p='M';p++;ptr++;}
		else if(strncmp(ptr,"�m",2)==0){*p='N';p++;ptr++;}
		else if(strncmp(ptr,"�n",2)==0){*p='O';p++;ptr++;}
		else if(strncmp(ptr,"�o",2)==0){*p='P';p++;ptr++;}
		else if(strncmp(ptr,"�p",2)==0){*p='Q';p++;ptr++;}
		else if(strncmp(ptr,"�q",2)==0){*p='R';p++;ptr++;}
		else if(strncmp(ptr,"�r",2)==0){*p='S';p++;ptr++;}
		else if(strncmp(ptr,"�s",2)==0){*p='T';p++;ptr++;}
		else if(strncmp(ptr,"�t",2)==0){*p='U';p++;ptr++;}
		else if(strncmp(ptr,"�u",2)==0){*p='V';p++;ptr++;}
		else if(strncmp(ptr,"�v",2)==0){*p='W';p++;ptr++;}
		else if(strncmp(ptr,"�w",2)==0){*p='X';p++;ptr++;}
		else if(strncmp(ptr,"�x",2)==0){*p='Y';p++;ptr++;}
		else if(strncmp(ptr,"�y",2)==0){*p='Z';p++;ptr++;}
		else if(strncmp(ptr,"�m",2)==0){*p='[';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='\\';p++;ptr++;}
		else if(strncmp(ptr,"�n",2)==0){*p=']';p++;ptr++;}
		else if(strncmp(ptr,"�O",2)==0){*p='^';p++;ptr++;}
		else if(strncmp(ptr,"�Q",2)==0){*p='_';p++;ptr++;}
		else if(strncmp(ptr,"�e",2)==0){*p='`';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='a';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='b';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='c';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='d';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='e';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='f';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='g';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='h';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='i';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='j';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='k';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='l';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='m';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='n';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='o';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='p';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='q';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='r';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='s';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='t';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='u';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='v';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='w';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='x';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='y';p++;ptr++;}
		else if(strncmp(ptr,"��",2)==0){*p='z';p++;ptr++;}
		else if(strncmp(ptr,"�o",2)==0){*p='{';p++;ptr++;}
		else if(strncmp(ptr,"�b",2)==0){*p='|';p++;ptr++;}
		else if(strncmp(ptr,"�p",2)==0){*p='}';p++;ptr++;}
		else{ *p=*ptr; p++; }
	}
	strcpy(str,buf);
	free(buf);

	return(0);
}
