// othello.h : OTHELLO �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_OTHELLO_H__FC6185DF_5A6F_428B_969A_22A691764BF7__INCLUDED_)
#define AFX_OTHELLO_H__FC6185DF_5A6F_428B_969A_22A691764BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// COthelloApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� othello.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class COthelloApp : public CWinApp
{
public:
	COthelloApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(COthelloApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(COthelloApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OTHELLO_H__FC6185DF_5A6F_428B_969A_22A691764BF7__INCLUDED_)
