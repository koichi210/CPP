// fname_exchange.h : FNAME_EXCHANGE �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_FNAME_EXCHANGE_H__45AC3F65_5AF9_4C42_9187_E74D82503712__INCLUDED_)
#define AFX_FNAME_EXCHANGE_H__45AC3F65_5AF9_4C42_9187_E74D82503712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CFname_exchangeApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� fname_exchange.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CFname_exchangeApp : public CWinApp
{
public:
	CFname_exchangeApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFname_exchangeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CFname_exchangeApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FNAME_EXCHANGE_H__45AC3F65_5AF9_4C42_9187_E74D82503712__INCLUDED_)
