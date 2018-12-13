/////////////////////////////////////////////////////////////////////////////
// ���ʂ̃��W���[���\�[�X
/////////////////////////////////////////////////////////////////////////////

#include "tchar.h"
#include "stdafx.h"
#include "StandardTemplate.h"

#if USE_IMM_H
#include <imm.h> //ImmAssociateContext(),imm32.lib
#endif


/////////////////////////////////////////////////////////////////////////////
// Global�֐���`
/////////////////////////////////////////////////////////////////////////////
// ���������������path�Ɋi�[
//void MyMergePath(
//				CString *cspPath	: [OUT] ���������p�X
//				CString csDrv		: [IN] ��������h���C�u��
//				CString csDir		: [IN] ��������f�B���N�g����
//				CString csFile		: [IN] ��������t�@�C����
//				CString csExt		: [IN] ��������g���q
void MyMergePath(CString *cspPath, CString csDrv, CString csDir, CString csFile, CString csExt)
{
	// �o�͐悪NULL��������I��
	if ( cspPath == NULL )
	{
		return;
	}

	// Drv�����񂪎w�肳��Ă�����Z�b�g
	if ( ! csDrv.IsEmpty() )
	{
		MyConnection(cspPath, csDrv);
	}

	// Dir�����񂪎w�肳��Ă�����Z�b�g
	if ( ! csDir.IsEmpty() )
	{
		MyConnection(cspPath, csDir);
	}

	// File�����񂪎w�肳��Ă�����Z�b�g
	if ( ! csFile.IsEmpty() )
	{
		MyConnection(cspPath, csFile);
	}

	// Ext�����񂪎w�肳��Ă�����Z�b�g
	if ( ! csExt.IsEmpty() )
	{
		AppendExt(cspPath, csExt);
	}
}


/////////////////////////////////////////////////////////////////////////////
// path�Ɋi�[����Ă���t���p�X���e������ɕ�����
//void MySplitPath(
//				CString csPath		: [IN] �������̃p�X
//				CString *cspDrv		: [OUT] ���o�����h���C�u��
//				CString *cspDir		: [OUT] ���o�����f�B���N�g����
//				CString *cspFile	: [OUT] ���o�����t�@�C����
//				CString *cspExt		: [OUT] ���o�����g���q
void MySplitPath(CString csPath, CString *cspDrv, CString *cspDir, CString *cspFile, CString *cspExt)
{
	char wk_drv[MAX_PATH];
	char wk_dir[MAX_PATH];
	char wk_file[MAX_PATH];
	char wk_ext[MAX_PATH];
	
	// �Ώە����񂪂Ȃ��̂ŏI��
	if ( csPath.IsEmpty() )
	{
		return;
	}

	splitpath_s(csPath.GetString(), wk_drv, sizeof(wk_drv), wk_dir, sizeof(wk_dir), wk_file, sizeof(wk_file), wk_ext, sizeof(wk_ext));

	// Drv�o�͐悪���� & ������� wk_drv������
	if( cspDrv && strlen(wk_drv) )
	{
		*cspDrv = wk_drv;
	}

	// Dir�o�͐悪���� & ������� wk_dir������
	if( cspDir && strlen(wk_dir) )
	{
		*cspDir = wk_dir;
	}

	// File�o�͐悪���� & ������� wk_file������
	if( cspFile && strlen(wk_file) )
	{
		*cspFile = wk_file;
	}

	// Ext�o�͐悪���� & ������� wk_ext������
	if( cspExt && strlen(wk_ext) )
	{
		*cspExt = wk_ext;
	}
}


/////////////////////////////////////////////////////////////////////////////
// ������ǉ�����Ƃ��A"\\"�̕t���v�ۂ��l��
//void MyConnection(
//			CString *csFile1	: [OUT] �o�͐�
//			CString csFile2		: [IN] ���͌�
void MyConnection(CString *csFile1, CString csFile2)
{
	if ( csFile1->IsEmpty() )
	{
		// ���߂ăZ�b�g����ꍇ�ɂ́A���̂܂ܑ��
		*csFile1 = csFile2;
	}
	else
	{
		// ���łɃZ�b�g����Ă����ꍇ�ɂ́A"\\"��ǉ�����K�v������̂�AppendPath���g�p
		AppendPath(csFile1, csFile2);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ������ǉ�
//void AppendPath(
//			CString *path		: [OUT] �o�͐�
//			CString string		: [IN] ���͌�
void AppendPath(CString *path, CString string)
{
	*path += "\\" + string;
}


/////////////////////////////////////////////////////////////////////////////
// �g���q��ǉ�
//void AppendExt(
//			CString *path		: [OUT] �o�͐�
//			CString string		: [IN] ���͌�
void AppendExt(CString *path, CString string)
{
	*path += "." + string;
}


/////////////////////////////////////////////////////////////////////////////
// ������u��
// void ReplaceString(
//			CString oname		: [IN] �u���O�̃I���W�i���t�@�C����
//			CString *nname		: [OUT] �u����̐V�����t�@�C����
//			LPSTR srch			: [IN] �����Ώە�����
//			LPSTR rep			: [IN] �u���Ώۂ̕�����
//			BOOL bDiffBigSmall	: [IN] �啶���������̋�ʉ�(TRUE=��ʂ��Ȃ�)
void ReplaceString(CString oname, CString *nname, LPSTR srch, LPSTR rep, BOOL bDiffBigSmall)
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
		if ( bDiffBigSmall )
		{
			rt = strncmp(&old_fname[i], srch, strlen(srch));	// �w�蕶�����ŖړI�̕���������� & �啶������������ʂ��Ȃ�
		}
		else
		{
			rt = _strnicmp(&old_fname[i], srch, strlen(srch));	// �w�蕶�����ŖړI�̕���������� & �啶������������ʂ���
		}

		if(rt == 0 )	// �w�蕶�����ŖړI�̕����������
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


/////////////////////////////////////////////////////////////////////////////
// �_�C�A���O
//BOOL Browse(
//		HWND m_hWnd		: [IN] �_�C�A���O�n���h��
//		CString title	: [IN] �_�C�A���O�̃^�C�g��
//		CString *path	: [OUT] �擾�����p�X
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


/////////////////////////////////////////////////////////////////////////////
// �_�C�A���O���̃R���g���[���ɕ���������蓖�Ă�
//BOOL SetDlgItemTextAll(
//			HWND hDlg			: [IN] �_�C�A���O�̃n���h��
//			const TABLE *pTbl	: [IN] ID�ƕ�������Z�b�g�ɂ���TABLE
//			int iMax			: [IN] TABLE�̃Z�b�g��
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
// ���p������S�p�����֕ϊ�
//int han2zen(
//		char *str	: [IN/OUT] �ϊ��Ώۂ̔��p����
void han2zen(char *str)
{
	char *buf,*p,*ptr;
	BOOL bLarge;

	buf = (char *)calloc(strlen(str)*2+1, sizeof(char));
	for(ptr=str, p=buf; *ptr!='\0'; *ptr++)
	{
		if ( issjiskanji(*ptr) )
		{
			/* SJIS����1�o�C�g�ڂ̏ꍇ */
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
			case ' ': strcpy(p,"�@"); break;
			case '!': strcpy(p,"�I"); break;
			case '"': strcpy(p,"�h"); break;
			case '#': strcpy(p,"��"); break;
			case '$': strcpy(p,"��"); break;
			case '%': strcpy(p,"��"); break;
			case '&': strcpy(p,"��"); break;
			case '\'': strcpy(p,"�f"); break;
			case '(': strcpy(p,"�i"); break;
			case ')': strcpy(p,"�j"); break;
			case '*': strcpy(p,"��"); break;
			case '+': strcpy(p,"�{"); break;
			case ',': strcpy(p,"�C"); break;
			case '-': strcpy(p,"�|"); break;
			case '.': strcpy(p,"�D"); break;
			case '/': strcpy(p,"�^"); break;
			case '0': strcpy(p,"�O"); break;
			case '1': strcpy(p,"�P"); break;
			case '2': strcpy(p,"�Q"); break;
			case '3': strcpy(p,"�R"); break;
			case '4': strcpy(p,"�S"); break;
			case '5': strcpy(p,"�T"); break;
			case '6': strcpy(p,"�U"); break;
			case '7': strcpy(p,"�V"); break;
			case '8': strcpy(p,"�W"); break;
			case '9': strcpy(p,"�X"); break;
			case ':': strcpy(p,"�F"); break;
			case ';': strcpy(p,"�G"); break;
			case '<': strcpy(p,"��"); break;
			case '=': strcpy(p,"��"); break;
			case '>': strcpy(p,"��"); break;
			case '?': strcpy(p,"�H"); break;
			case '@': strcpy(p,"��"); break;
			case 'A': strcpy(p,"�`"); break;
			case 'B': strcpy(p,"�a"); break;
			case 'C': strcpy(p,"�b"); break;
			case 'D': strcpy(p,"�c"); break;
			case 'E': strcpy(p,"�d"); break;
			case 'F': strcpy(p,"�e"); break;
			case 'G': strcpy(p,"�f"); break;
			case 'H': strcpy(p,"�g"); break;
			case 'I': strcpy(p,"�h"); break;
			case 'J': strcpy(p,"�i"); break;
			case 'K': strcpy(p,"�j"); break;
			case 'L': strcpy(p,"�k"); break;
			case 'M': strcpy(p,"�l"); break;
			case 'N': strcpy(p,"�m"); break;
			case 'O': strcpy(p,"�n"); break;
			case 'P': strcpy(p,"�o"); break;
			case 'Q': strcpy(p,"�p"); break;
			case 'R': strcpy(p,"�q"); break;
			case 'S': strcpy(p,"�r"); break;
			case 'T': strcpy(p,"�s"); break;
			case 'U': strcpy(p,"�t"); break;
			case 'V': strcpy(p,"�u"); break;
			case 'W': strcpy(p,"�v"); break;
			case 'X': strcpy(p,"�w"); break;
			case 'Y': strcpy(p,"�x"); break;
			case 'Z': strcpy(p,"�y"); break;
			case '[': strcpy(p,"�m"); break;
			case '\\': strcpy(p,"��"); break;
			case ']': strcpy(p,"�n"); break;
			case '^': strcpy(p,"�O"); break;
			case '_': strcpy(p,"�Q"); break;
			case '`': strcpy(p,"�e"); break;
			case 'a': strcpy(p,"��"); break;
			case 'b': strcpy(p,"��"); break;
			case 'c': strcpy(p,"��"); break;
			case 'd': strcpy(p,"��"); break;
			case 'e': strcpy(p,"��"); break;
			case 'f': strcpy(p,"��"); break;
			case 'g': strcpy(p,"��"); break;
			case 'h': strcpy(p,"��"); break;
			case 'i': strcpy(p,"��"); break;
			case 'j': strcpy(p,"��"); break;
			case 'k': strcpy(p,"��"); break;
			case 'l': strcpy(p,"��"); break;
			case 'm': strcpy(p,"��"); break;
			case 'n': strcpy(p,"��"); break;
			case 'o': strcpy(p,"��"); break;
			case 'p': strcpy(p,"��"); break;
			case 'q': strcpy(p,"��"); break;
			case 'r': strcpy(p,"��"); break;
			case 's': strcpy(p,"��"); break;
			case 't': strcpy(p,"��"); break;
			case 'u': strcpy(p,"��"); break;
			case 'v': strcpy(p,"��"); break;
			case 'w': strcpy(p,"��"); break;
			case 'x': strcpy(p,"��"); break;
			case 'y': strcpy(p,"��"); break;
			case    'z': strcpy(p,"��"); break;
			case    '{': strcpy(p,"�o"); break;
			case    '|': strcpy(p,"�b"); break;
			case    '}': strcpy(p,"�p"); break;
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
// �S�p�����𔼊p�����֒u��
//int zen2han(
//		char *str	: [IN/OUT] �ϊ��Ώۂ̑S�p����
void zen2han(char *str)
{
	char	*buf,*p,*ptr;
	BOOL	bLarge;

	buf=(char *)calloc(strlen(str)+1,sizeof(char));

	for(ptr=str, p=buf; *ptr!='\0'; *ptr++,p++)
	{
		bLarge = TRUE;
		if ( strncmp(ptr, "�@", 2) == 0 ){*p=' ';}
		else if ( strncmp(ptr, "�I", 2) == 0 ){*p='!';}
		else if ( strncmp(ptr, "�h", 2) == 0 ){*p='"';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='#';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='$';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='%';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='&';}
		else if ( strncmp(ptr, "�f", 2) == 0 ){*p='\'';}
		else if ( strncmp(ptr, "�i", 2) == 0 ){*p='(';}
		else if ( strncmp(ptr, "�j", 2) == 0 ){*p=')';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='*';}
		else if ( strncmp(ptr, "�{", 2) == 0 ){*p='+';}
		else if ( strncmp(ptr, "�C", 2) == 0 ){*p=',';}
		else if ( strncmp(ptr, "�|", 2) == 0 ){*p='-';}
		else if ( strncmp(ptr, "�D", 2) == 0 ){*p='.';}
		else if ( strncmp(ptr, "�^", 2) == 0 ){*p='/';}
		else if ( strncmp(ptr, "�O", 2) == 0 ){*p='0';}
		else if ( strncmp(ptr, "�P", 2) == 0 ){*p='1';}
		else if ( strncmp(ptr, "�Q", 2) == 0 ){*p='2';}
		else if ( strncmp(ptr, "�R", 2) == 0 ){*p='3';}
		else if ( strncmp(ptr, "�S", 2) == 0 ){*p='4';}
		else if ( strncmp(ptr, "�T", 2) == 0 ){*p='5';}
		else if ( strncmp(ptr, "�U", 2) == 0 ){*p='6';}
		else if ( strncmp(ptr, "�V", 2) == 0 ){*p='7';}
		else if ( strncmp(ptr, "�W", 2) == 0 ){*p='8';}
		else if ( strncmp(ptr, "�X", 2) == 0 ){*p='9';}
		else if ( strncmp(ptr, "�F", 2) == 0 ){*p=':';}
		else if ( strncmp(ptr, "�G", 2) == 0 ){*p=';';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='<';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='=';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='>';}
		else if ( strncmp(ptr, "�H", 2) == 0 ){*p='?';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='@';}
		else if ( strncmp(ptr, "�`", 2) == 0 ){*p='A';}
		else if ( strncmp(ptr, "�a", 2) == 0 ){*p='B';}
		else if ( strncmp(ptr, "�b", 2) == 0 ){*p='C';}
		else if ( strncmp(ptr, "�c", 2) == 0 ){*p='D';}
		else if ( strncmp(ptr, "�d", 2) == 0 ){*p='E';}
		else if ( strncmp(ptr, "�e", 2) == 0 ){*p='F';}
		else if ( strncmp(ptr, "�f", 2) == 0 ){*p='G';}
		else if ( strncmp(ptr, "�g", 2) == 0 ){*p='H';}
		else if ( strncmp(ptr, "�h", 2) == 0 ){*p='I';}
		else if ( strncmp(ptr, "�i", 2) == 0 ){*p='J';}
		else if ( strncmp(ptr, "�j", 2) == 0 ){*p='K';}
		else if ( strncmp(ptr, "�k", 2) == 0 ){*p='L';}
		else if ( strncmp(ptr, "�l", 2) == 0 ){*p='M';}
		else if ( strncmp(ptr, "�m", 2) == 0 ){*p='N';}
		else if ( strncmp(ptr, "�n", 2) == 0 ){*p='O';}
		else if ( strncmp(ptr, "�o", 2) == 0 ){*p='P';}
		else if ( strncmp(ptr, "�p", 2) == 0 ){*p='Q';}
		else if ( strncmp(ptr, "�q", 2) == 0 ){*p='R';}
		else if ( strncmp(ptr, "�r", 2) == 0 ){*p='S';}
		else if ( strncmp(ptr, "�s", 2) == 0 ){*p='T';}
		else if ( strncmp(ptr, "�t", 2) == 0 ){*p='U';}
		else if ( strncmp(ptr, "�u", 2) == 0 ){*p='V';}
		else if ( strncmp(ptr, "�v", 2) == 0 ){*p='W';}
		else if ( strncmp(ptr, "�w", 2) == 0 ){*p='X';}
		else if ( strncmp(ptr, "�x", 2) == 0 ){*p='Y';}
		else if ( strncmp(ptr, "�y", 2) == 0 ){*p='Z';}
		else if ( strncmp(ptr, "�m", 2) == 0 ){*p='[';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='\\';}
		else if ( strncmp(ptr, "�n", 2) == 0 ){*p=']';}
		else if ( strncmp(ptr, "�O", 2) == 0 ){*p='^';}
		else if ( strncmp(ptr, "�Q", 2) == 0 ){*p='_';}
		else if ( strncmp(ptr, "�e", 2) == 0 ){*p='`';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='a';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='b';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='c';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='d';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='e';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='f';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='g';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='h';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='i';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='j';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='k';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='l';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='m';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='n';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='o';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='p';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='q';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='r';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='s';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='t';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='u';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='v';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='w';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='x';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='y';}
		else if ( strncmp(ptr, "��", 2) == 0 ){*p='z';}
		else if ( strncmp(ptr, "�o", 2) == 0 ){*p='{';}
		else if ( strncmp(ptr, "�b", 2) == 0 ){*p='|';}
		else if ( strncmp(ptr, "�p", 2) == 0 ){*p='}';}
		else{ *p=*ptr; bLarge=FALSE; }

		if ( bLarge ) {ptr++;}
	}
	strcpy(str,buf);
	free(buf);
}


/////////////////////////////////////////////////////////////////////////////
// Class��`
/////////////////////////////////////////////////////////////////////////////
// CMyStaticImage �r�b�g�}�b�v��`��
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


// �v���W�F�N�g���ɍ쐬�����r�b�g�}�b�v��ID��n��
void CMyStaticImage::SetBitmapId(int id)
{
	m_idBitmap = id;
}


// m_idBitmap�Ŏw�肳��Ă���r�b�g�}�b�v��`�悷��
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
// CMyRistrictedSBCS SBCS�����̓��͂��R���g���[���i�����j����
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


// �����̎�ʂ�ݒ�
void CMyRistrictedSBCS::SetCharacterKind(DWORD dwRange)
{
	m_dwCharKind = dwRange;
}


// ����ΏۊO�i��O�j�Ƃ��ē���̕�����o�^����
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


// �R�s�y�𖳌��ɂ���
void CMyRistrictedSBCS::DisableCopyAndPaste(BOOL bDisableCopyAndPaste)
{
	m_bDisableCopyAndPaste = bDisableCopyAndPaste;
}


// IME�𖳌��ɂ���i�ʓr���C�u�������K�v�j
void CMyRistrictedSBCS::OnSetFocus(CWnd *pOldWnd)
{
#if USE_IMM_H
	ImmAssociateContext(this->m_hWnd, NULL); //disable IME
#endif
	CEdit::OnSetFocus(pOldWnd);
}


// �L�[���͂��������Ƃ��ɕ����̔���
void CMyRistrictedSBCS::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL	bCallOnChar = TRUE;

	if ( ! iscntrl(nChar) ) //����R�[�h cf.���z�L�[�R�[�h(VK_BACK, VK_TAB, etc, ...) ???
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

		// ��O�w�肳��Ă���������������A�����ΏۊO�Ƃ���
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

	// �R�s�y�������ݒ�̂Ƃ��A�R�s�y���ꂽ�珈���ΏۊO�Ƃ���
	if ( bCallOnChar && m_bDisableCopyAndPaste )
	{
		if ( nChar == 0x0003 || //Copy
			 nChar == 0x0016 || //Paste
			 nChar == 0x0018 )  //Cut
			bCallOnChar = FALSE;
	}

	if ( bCallOnChar ) CEdit::OnChar(nChar, nRepCnt, nFlags);
}


// �R�s�y�����ݒ�̂Ƃ��A�R���e�L�X�g���j���[�ł������ɂ���
void CMyRistrictedSBCS::OnContextMenu(CWnd *pWnd, CPoint point)
{
	if ( ! m_bDisableCopyAndPaste ) CEdit::OnContextMenu(pWnd, point);
}


/////////////////////////////////////////////////////////////////////////////
// CMyEdit �G�f�B�b�g�{�b�N�X�����
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


// �G�f�B�b�g�{�b�N�X���쐬����
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
		isDefOperation = TRUE;	// �����폜�͖������ŋ���
		isCtrlString = TRUE;	// ���䕶���̏ꍇ�A�����������Ȃ�
		break;

	case VK_SPACE :
		isDefOperation = TRUE;	// �X�y�[�X�͖������ŋ���
		break;

	default :
		if ( isdigit(nChar) && (m_PossibleKind & USESTRING_DIGIT) ||	// ���l���͂�����
			 isalpha(nChar) && (m_PossibleKind & USESTRING_ALPHA) ||	// a�`z���͂�����
			 m_PossibleKind == USESTRING_ANY )							// ���ׂĂ̕���������
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
	// �N���b�v�{�[�h�o�R�œ��͂����ƍ���̂ŁA�R���e�L�X�g���j���[��\�������Ȃ��B
	//CEdit::OnRButtonUp(nFlags, point);
}


void CMyEdit::OnKillFocus(CWnd *pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
//CMyList ���X�g�{�b�N�X�����
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


// ���X�g�{�b�N�X���쐬����
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

	// �e�̃t�H���g��ݒ�
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
// CMyListCtrl ���X�g�R���g���[�������
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

	// �����I�����Ă��Ȃ���Ԃ̂Ƃ�������̂ŁA��������I�����Ă���Ƃ��̂݃L�[������s���悤�ɂ���
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
	// �������͂Ō����@�\�������Ă��܂��̂ŋ����I�ɃL�[���͂𖳌��ɂ���
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
		{	// 2��ڂ̃N���b�N�́u�ҏW�v
			CellEdit(hittest);
		}
		else
		{	// 1��ڂ̃N���b�N�́u�I���v
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
	// �T�C�h�w�b�_��I�������Ƃ��ɕs���ȃZ�����I����ԂɂȂ��Ă��܂��̂ŁB�B
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
	// �ĕ`�悷��Item�̍��W���擾
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int iItem = lpDrawItemStruct->itemID;
	TCHAR szBuff[MAX_PATH];
	LVITEM lvItem;
	int col = 0;
	CRect rect;

	//�t�H�[�J�X�����邩�ǂ���
	BOOL isForcus = GetItemState(iItem, LVIS_FOCUSED) == LVIS_FOCUSED;

	CHeaderCtrl *Head = GetHeaderCtrl();
	int numitem = Head->GetItemCount();

	while( numitem > col )
	{
		// �A�C�e���擾�̂��߂̐ݒ�
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
				// �A�C�e���̂Ƃ��ɂ�GetSubItem���ƑS�̂�����Ă��Ă��܂��̂œ��ʈ���������
				GetItemRect(iItem, &rect, LVIR_BOUNDS | LVIR_LABEL);
				GetItemRect(iItem, &itemrect, LVIR_BOUNDS);
				rect.top = itemrect.top;
				rect.left = itemrect.left;
			}
			else
			{
				GetSubItemRect(iItem, col, LVIR_BOUNDS, rect);
			}

			// �I�����Ă��邩�ۂ�
			DWORD txtColor = ::GetSysColor(COLOR_WINDOWTEXT);
			BOOL isSelect = FALSE;

			if ( GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED && col == m_Pos.x )
			{
				// �I����Ԃ�\������
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
					// �I������Ă��Ȃ���Ԃ�\������
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
			strRect.left += 2; // �����ʒu������

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

	// ���X�g�̋�`�̓T�u�A�C�e���̋�`�̍��������ڐ����ɍ�����������
	framerect.bottom = framerect.top + framerect.Height() * this->m_NumElements;

	// �h���b�v�_�E�����X�g���쐬
	// ���X�g�̍��ځA���X�g�̍��ڐ�(5��)�A���X�g�̋�`�A�����̃E�B���h�E�N���X�|�C���^
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
		pItemList[i].idxItem = -1;	//������

		//dwConstraint �͎g���Ȃ��S�Ẵr�b�g�������Ă���
		if ( pItemList[i].dwConstraint & dwConstraint )
			continue;	//�\�������L��

		idx = InsertString(idx, "");
		if ( idx != CB_ERR )
		{
			SetItemData(idx, pItemList[i].value);
			SetItemHeight(i, m_iHeight);
			pItemList[i].idxItem = idx;	//�ǉ������A�C�e���ɑ΂�idx��t�^����
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

			//�\���������������ꍇ�A�e�[�u����idx�l��ݒ肵�Ă�����K�v������
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
// CMyEvent �L�[�{�[�h or �}�E�X�̓��͂��G�~���[���[�g����
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

	//��������(Shift,Ctrl,Alt��)
	if ( ! bHold )
    {
        input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
        ::SendInput(1, &input, sizeof(INPUT));
     }
}

/////////////////////////////////////////////////////////////////////////////
// CCustomCtrl ���O�̕\���쐬����
CCustomCtrl::CCustomCtrl()
{
	m_nMaxColumn = 1;
	m_nMaxRow = 1;
}


void CCustomCtrl::SetLabel(int nIdx, int nWidth, CString name)
{
	LV_COLUMN	lvCol;

	// Column���ʐݒ�
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	lvCol.fmt  = LVCFMT_LEFT;

	// ���x���ݒ�
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
	//�s�őI������悤�Ɏw��
	SetExtendedStyle(LVS_EX_FULLROWSELECT);
	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}


/////////////////////////////////////////////////////////////////////////////
// CMyCompareFile 2�̃t�@�C�����r����
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
	
	// open�����܂܉��x���Q�Ƃ���ꍇ�A�t�@�C���|�C���^�����ɍs���Ă�̂�SeekToBegin�ŏ����l�ɖ߂�
	//m_cfile1.SeekToBegin();
	//m_cfile2.SeekToBegin();
	szFile1 = (size_t)m_cfile1.GetLength();
	szFile2 = (size_t)m_cfile2.GetLength();
	
	// �t�@�C���T�C�Y������
	if ( szFile1 == szFile2 )
	{
		pbuf1 = (char *)calloc(szFile1, sizeof(char));
		pbuf2 = (char *)calloc(szFile2, sizeof(char));

		m_cfile1.Read(pbuf1, szFile1);
		m_cfile2.Read(pbuf2, szFile2);

		// �o�C�i���f�[�^������
		if ( memcmp(pbuf1, pbuf2, szFile1) == 0 )
		{
			bReturn = TRUE;
		}
		free(pbuf1);
		free(pbuf2);
	}

	return bReturn;
}