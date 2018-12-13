// �v���W�F�N�g�Ŏg�p����萔��`

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// language
#define ENGLISH			(0)

//�f�o�b�O��`
#define TOOLBAR_USE		(1)
#define ALLSTEAL_USE	(0)

// ��{�萔
#define STR_BUFF			(256)	// ������ő�o�b�t�@
#define START_STONE			(4)		// �X�^�[�g���̐΂̐�
#define WIN_MIN_SIZE		(250)	// �E�B���h�E�\���̂̍ŏ��l
#define TEXT_OFFSET			(100)	// ���Ԃ�\�����Ă���e�L�X�g�̉���
#define FRAME_OFFSET		(25)	// ��ʒ[����g�܂ł̋���
#define NUM_OFFSET			(5)		// ��ʒ[����g�ԍ��܂ł̋���
#define FRAME2TEXT_OFFSET	(20)	// ��՘g�ƃe�L�X�g�g�Ƃ̋���
#define TEXT2TEXT_OFFSET	(40)	// ���Ɣ��̋���
#define SCORE_OFFSET		(60)	// �X�R�A�\���̋���
#define TIME2TEXT_OFFSET	(20)	// �F���Ǝ��ԂƂ̋���
#define DEFAULT_TIME_POS_X	(181)	// ���ԕ\���iX���W�j��DEFAULT�l
#define DEFAULT_TIME_POS_Y	(60)	// ���ԕ\���iY���W�j��DEFAULT�l
#define TIME_OUT			(1)		// �^�C���A�E�g

// ���ʎq
#define EMY					(1)		// ����
#define ENEMY				(2)		// �G

// �^�C�}ID
#define COM_TIMER_ID		(1)		// COM �̓����^�C�}ID
#define BLACK_TIMER_ID		(2)		// �� �̓����^�C�}ID
#define WHITE_TIMER_ID		(3)		// �� �̓����^�C�}ID

// �^�C�}
#define VS_MODE_INTERVAL	(10)	// PLAYER VS COM ���̃C���^�[�o��(10ms���炢���ȁH)
#define COM_MODE_INTERVAL	(10)	// COM VS COM ���̃C���^�[�o��
#define COUNT_DOWN_CYC		(100)	// �J�E���g�_�E������

// �F
#define UNKNOWN				(0)		// �s��
#define BLACK				(1)		// ��
#define WHITE				(2)		// ��
#define BLACK_NAME			"��"	// ���̖��O
#define WHITE_NAME			"��"	// ���̖��O

// �}�X
#define LINE_MIN		(1)						// �P�ӂ̃}�X�̍ŏ��l
#define LINE_MAX		(8)						// �P�ӂ̃}�X�̍ő�l
#define	MAS_MAX			(LINE_MAX * LINE_MAX)	// �}�X�̍ő�l
#define TURN_MAX		(MAS_MAX-START_STONE)	// �^�[���̍ő吔

// ����
#define ORIENT_MAX		(8)	// �����ő吔
#define LEFT			(1)	// �i�ޕ����i���j
#define RIGHT			(2)	// �i�ޕ����i�E�j
#define UP				(3)	// �i�ޕ����i��j
#define DOWN			(4)	// �i�ޕ����i���j
#define LEFT_UP			(5)	// �i�ޕ����i����j
#define LEFT_DOWN		(6)	// �i�ޕ����i�����j
#define RIGHT_UP		(7)	// �i�ޕ����i�E��j
#define RIGHT_DOWN		(8)	// �i�ޕ����i�E���j

// �����̑���߂�
#define VERS			(1)	// ��著��
#define REVERS			(2)	// ���߂�

// �^�[��
#define	PLAYER			(1)	// PLAYER
#define	COM				(2)	// COMPUTER

// VS
#define	PLAYER_PLAYER	(1)	// �l VS �l
#define	PLAYER_COM		(2)	// �l VS COM
#define	COM_PLAYER		(3)	// COM VS �l
#define	COM_COM			(4)	// COM VS COM

// �Q�[���X�e�[�^�X
#define GAME_INIT		(1)	// �Q�[���J�n�O
#define GAME_PLAY		(2)	// �Q�[����
#define GAME_STOP		(3)	// �Q�[����~
#define GAME_END		(4)	// �Q�[���I��

// manager
#define	PUT_ERROR		(-1)	// PutCheck ERROR
#define	PUT_NORMAL		(0)		// PutCheck NORMAL
#define	NORMAL_RETURN	(0)		// ���탊�^�[��
#define ERROR_RETURN	(-1)	// �ُ탊�^�[��
#define CHK_ON			(1)		// �`�F�b�N�̂݁iPutCheck)
#define PUT_ON			(2)		// �u����ꏊ���T�[�`�iPutCheck)

// ����
#define KIHU_DIR		"�I�Z������\\"
#define KIHU_NAME		"DATE"
#define SHOW			(1)	// ����������
#define SAVE			(2)	// ������ۑ�

// COM
#define	STARTER			(1)	// ����(COM)
#define	CENTER			(2)	// ����(COM)
#define	FINISHER		(3)	// �I��(COM)

// �ݒ�l�̏��
#define ON				(1)
#define OFF				(0)

// ���j���[�o�[�ݒ�l
#define	ENABLE			(1)
#define	CHECK			(2)

// COM�p �Ԃ�l
#define COM_ERROR		(-1)	// COM�ُ�I��
#define COM_NORMAL		(0)		// COM����I��
#define STEAL			(3)		// �����
#define NOT_STEAL		(4)		// ����Ȃ�
#define STAR			(5)		// �u���Ƃ���͐�
#define	NOT_STAR		(6)		// ���ł͂Ȃ�
#define NSTAR			(7)		// �u���Ƃ���͐��̗�
#define	NOT_NSTAR		(8)		// ���̋߂��ł͂Ȃ�

#define STR_WEIGHT		(15)	// ������
#define STR_HEIGHT		(15)	// ��������

// �e�p�����[�^�̏����l
#define DEFAULT_PLAY_MODE	PLAYER_COM
#define DEFAULT_TIME		(-1)	// �������ԁi�b�j
#define DEFAULT_COM_LEVEL	(3)
#define DEFAULT_PUT_NOTICE	ON
