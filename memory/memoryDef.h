// memoryDef.h : �w�b�_�[ �t�@�C��
//
#define FONT		1	// ������Ɩ��ȃ��c

#define GENERATE_ID	1	//�^�C�}ID
#define SHOW_WEIGHT	40	//�\��������̕�
#define SHOW_HEIGHT	40	//�\��������̍���
#define STR_BUFF	256
#define ANSER_BUFF	1024	
#define HELP_BUFF	4096	

//�ő�ŏ��l
#define	MIN_VAL		1		//�e�l�̍ŏ��l
#define PR_NUM_MAX	20		//�o�萔�̍ő�l
#define KETA_ANIKI_MAX	15	//�o�茅���̍ő�l
#define KETA_KEISAN_MAX	3	//�o�茅���̍ő�l
#define MAX_CYC		5000	// �ő����
#define MIN_CYC		10		// �ŏ�����
#define ANS_OFFSET	15		//���b�Z�[�W�\���̃w�b�_��
#define ANS_MAX		(KETA_ANIKI_MAX+ANS_OFFSET)

//�����l
#define	CYC_INIT_VAL	600		//�\������(ms)
#define	NUM_INIT_VAL	5		//�o�萔�̏����l
#define	KETA_INIT_VAL	1		//�o�茅���̏����l
#define TYPE_INIT_VAL	KEISAN	// �o���ʂ̏����l

//���
#define	INIT		0	//����
#define	ING			1	//�^���Œ�
#define	END			2	//�I��

//���[�h
#define	ANKI		1	//�ËL
#define	KEISAN		2	//�v�Z

//�^�C�v
#define NUMBER		1	//���l
#define ENG_SMALL	2	//�A���t�@�x�b�g�i���j
#define ENG_LARGE	4	//�A���t�@�x�b�g�i��j

// ���^�[���l
#define NORMAL		0	//���탊�^�[��
#define NUM_ERR		1	//��萔�G���[
#define KETA_ERR	2	//�����G���[


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
