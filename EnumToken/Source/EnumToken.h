// EnumToken.h : SECURITY �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_SECURITY_H__B92FE000_4F5D_4C82_A9C8_36DBCC1FCEB3__INCLUDED_)
#define AFX_SECURITY_H__B92FE000_4F5D_4C82_A9C8_36DBCC1FCEB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSecurityApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� EnumToken.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSecurityApp : public CWinApp
{
public:
	CSecurityApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSecurityApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CSecurityApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SECURITY_H__B92FE000_4F5D_4C82_A9C8_36DBCC1FCEB3__INCLUDED_)
