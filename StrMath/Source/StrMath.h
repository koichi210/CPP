// StrMath.h : STRMATH �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_STRMATH_H__28139293_0EB6_4B5C_A315_4C6821FBD7E5__INCLUDED_)
#define AFX_STRMATH_H__28139293_0EB6_4B5C_A315_4C6821FBD7E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CStrMathApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� StrMath.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CStrMathApp : public CWinApp
{
public:
	CStrMathApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CStrMathApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CStrMathApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STRMATH_H__28139293_0EB6_4B5C_A315_4C6821FBD7E5__INCLUDED_)
