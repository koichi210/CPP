
// BinaryEdit_Mfc.h : BinaryEdit_Mfc �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CBinaryEdit_MfcApp:
// ���̃N���X�̎����ɂ��ẮABinaryEdit_Mfc.cpp ���Q�Ƃ��Ă��������B
//

class CBinaryEdit_MfcApp : public CWinAppEx
{
public:
	CBinaryEdit_MfcApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBinaryEdit_MfcApp theApp;
