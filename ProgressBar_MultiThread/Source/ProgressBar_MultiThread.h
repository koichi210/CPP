
// ProgressBar_MultiThread.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CProgressBarApp:
// ���̃N���X�̎����ɂ��ẮAProgressBar_MultiThread.cpp ���Q�Ƃ��Ă��������B
//

class CProgressBarApp : public CWinApp
{
public:
	CProgressBarApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CProgressBarApp theApp;