// zodiac.h : ZODIAC �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_ZODIAC_H__3FFB9082_4534_4A0D_911F_45F6F34D33F4__INCLUDED_)
#define AFX_ZODIAC_H__3FFB9082_4534_4A0D_911F_45F6F34D33F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CZodiacApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� zodiac.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CZodiacApp : public CWinApp
{
public:
	CZodiacApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CZodiacApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CZodiacApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ZODIAC_H__3FFB9082_4534_4A0D_911F_45F6F34D33F4__INCLUDED_)
