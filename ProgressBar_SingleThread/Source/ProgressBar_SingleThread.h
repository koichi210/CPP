
// ProgressBar_SingleThread.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CProgressBar_SingleThreadApp:
// ���̃N���X�̎����ɂ��ẮAProgressBar_SingleThread.cpp ���Q�Ƃ��Ă��������B
//

class CProgressBar_SingleThreadApp : public CWinApp
{
public:
	CProgressBar_SingleThreadApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CProgressBar_SingleThreadApp theApp;