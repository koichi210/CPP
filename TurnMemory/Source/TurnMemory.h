// TurnMemory.h : TURNMEMORY �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_TURNMEMORY_H__6E461B87_B055_43F8_84C0_9B42194B7434__INCLUDED_)
#define AFX_TURNMEMORY_H__6E461B87_B055_43F8_84C0_9B42194B7434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CTurnMemoryApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� TurnMemory.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CTurnMemoryApp : public CWinApp
{
public:
	CTurnMemoryApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTurnMemoryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CTurnMemoryApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TURNMEMORY_H__6E461B87_B055_43F8_84C0_9B42194B7434__INCLUDED_)
