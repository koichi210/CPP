
// FileCompareDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "FileCompare.h"
#include "FileCompareDlg.h"
#include "afxdialogex.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileCompareDlg �_�C�A���O
CFileCompareDlg::CFileCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileCompareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bProc = FALSE;
}


void CFileCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDPR_COMPARE, mdx_progress);
}


BEGIN_MESSAGE_MAP(CFileCompareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBT_EXECUTE, OnBnClickedExecute)
END_MESSAGE_MAP()


// CFileCompareDlg ���b�Z�[�W �n���h���[
BOOL CFileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	GetDlgItem(IDBT_EXECUTE)->SetWindowText(STR_START);
	GetDlgItem(IDET_PATH)->SetWindowText(STR_DEFALUT_PATH);
	GetDlgItem(IDET_PATH)->SetFocus();
	((CEdit*)GetDlgItem(IDET_PATH))->SetSel(0,-1);
	return FALSE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}


// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B
void CFileCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CFileCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileCompareDlg::OnBnClickedExecute()
{
	if ( m_bProc )
	{
		StopProc();
		return;
	}

	InitProc();
	GetFolder();
	GetFile();
	Compare();
}


void CFileCompareDlg::StopProc()
{
	GetDlgItem(IDBT_EXECUTE)->SetWindowText(STR_START);
	m_bProc = FALSE;
}

void CFileCompareDlg::InitProc()
{
	m_SameGroup = 1;
	m_FileNum = 0;
	m_bProc = TRUE;

	for ( int i=0; i<MAX_FILE_NUM; i++)
	{
		m_FileList[i].nFindSame = UNKNOWN_SAME;
		m_FileList[i].strFileName = "";
	}

	for ( int i=0; i<MAX_FOLDER_NUM; i++)
	{
		m_FolderList[i] = "";
	}

	GetDlgItem(IDBT_EXECUTE)->SetWindowText(STR_STOP);
	OutputResult(TRUE);
}


void CFileCompareDlg::GetFolder()
{
	CString temp;
	int		nCnt;

	GetDlgItem(IDET_PATH)->GetWindowText(temp);
	if ( temp.IsEmpty() )
	{
		return ;
	}
	
	for(int i=0; i<MAX_FOLDER_NUM; i++)
	{
		nCnt = temp.Find(";");
		if ( nCnt != -1 )
		{
			m_FolderList[i] = temp.Left(nCnt);
			temp.Delete(0, nCnt+1);	//�ړI�̕����� + ��؂�i;�j������
		}
		else
		{
			m_FolderList[i] = temp;
			break;
		}
	}
}


void CFileCompareDlg::GetFile()
{
	CFileFind cFind;
	TCHAR pathName[MAX_PATH];

	for ( int i=0; i<MAX_FOLDER_NUM; i++)
	{
		if ( m_FolderList[i].IsEmpty() )
		{
			break;
		}

		// �����Ώۃt�H���_�̃p�X
		sprintf(pathName, "%s\\*", m_FolderList[i]);

		BOOL bContinue = cFind.FindFile(pathName);
		while( bContinue )
		{
			bContinue = cFind.FindNextFile();
			
			if ( cFind.IsDots() )	// "." or ".."
			{
				continue;
			}

			if( cFind.IsDirectory() )
			{
				// �t�H���_�̓l�X�g����H
			}
			else // �t�@�C���Ƃ����F���ŗǂ��H
			{
				m_FileList[m_FileNum].strFileName = cFind.GetFilePath();
				if ( m_FileNum < MAX_FILE_NUM )
				{
					m_FileNum++;
				}
				else
				{
					break;
				}
			}
		}
	}
}

void CFileCompareDlg::Compare()
{
	//�v���O���X�o�[�̕����Z�b�g�B��������Ŕ�r���邽�ߎ���[x * (x-1) /2 = 1�`x-1 �܂ł̘a]
	mdx_progress.SetRange32(0, GetProgressBarEnd());

	// �ʒu�\���̃��x�������Z�b�g
	UpdateProgressBar(POS_INIT);
	AfxBeginThread(ProcThread, this);
}

void CFileCompareDlg::OutputResult(BOOL bClear)
{
	CString str;

	if ( bClear )
	{
		str = "";
	}
	else
	{
		CString temp;
		for ( int i=1; i<m_SameGroup; i++)
		{
			temp.Format("[Group%d]%s", i, END_OF_LINE);
			str += temp;
			for ( int j=0; j<MAX_FILE_NUM; j++)
			{
				if ( m_FileList[j].nFindSame == i )
				{
					str += m_FileList[j].strFileName;
					str += END_OF_LINE;
				}
			}
			str += END_OF_LINE;
		}
	}
	GetDlgItem(IDET_RESULT)->SetWindowText(str);
}

void CFileCompareDlg::Result()
{
	m_bProc = FALSE;
	OutputResult();
}


int CFileCompareDlg::GetProgressBarEnd()
{
	//return m_FileNum*(m_FileNum-1)/2;
	return m_FileNum*(m_FileNum-1);
}


void CFileCompareDlg::UpdateProgressBar(int nPos)
{
	// Start�ʒu�AEnd�ʒu��ݒ�
	int start,end;
	switch(nPos)
	{
	case POS_INIT :
		start = 0;
		end = 0;
		break;

	case POS_END :
		start = GetProgressBarEnd();
		end = GetProgressBarEnd();
		break;

	default :
		start = nPos;
		end = GetProgressBarEnd();
		break;
	}

	// �v���O���X�o�[�̌��݈ʒu�ݒ�
	mdx_progress.SetPos(start);

	// ���x���\��
	CString str;
	str.Format("(%d / %d)", start, end);
	GetDlgItem(IDST_COMPARE)->SetWindowText(str);
}


UINT ProcThread(LPVOID pParam)
{
	CFileCompareDlg* pDlg=(CFileCompareDlg*)pParam;
	CMyCompareFile cCompFile;
	CString str;
	BOOL bIsSame;

	for( int i=0; i < pDlg->m_FileNum && pDlg->m_bProc; i++ )	// �t�@�C�����X�g�������ς��ɂȂ�܂Ń��[�v
	{
		pDlg->UpdateProgressBar(i * pDlg->m_FileNum);	// �O�̍s * 1��MAX + j
		if ( pDlg->m_FileList[i].nFindSame != UNKNOWN_SAME )
		{
			// ���łɃ_�u��������Ă���
			continue;
		}

		for( int j=i+1; j < pDlg->m_FileNum && pDlg->m_bProc; j++ )
		{
			pDlg->UpdateProgressBar(i * pDlg->m_FileNum + j);	// �O�̍s * 1��MAX + j
			if ( pDlg->m_FileList[j].nFindSame != UNKNOWN_SAME )
			{
				// ���łɃ_�u��������Ă���
				continue;
			}
			bIsSame = cCompFile.CompareBinary(pDlg->m_FileList[i].strFileName, pDlg->m_FileList[j].strFileName);
			if ( bIsSame )
			{
				pDlg->m_FileList[i].nFindSame = pDlg->m_SameGroup;
				pDlg->m_FileList[j].nFindSame = pDlg->m_SameGroup;
			}
		}

		if ( pDlg->m_FileList[i].nFindSame != UNKNOWN_SAME )
		{
			// ����V�������������
			pDlg->m_SameGroup++;
		}
	}

	//���ʕ\��
	pDlg->StopProc();
	pDlg->OutputResult();
	return TRUE;
}

