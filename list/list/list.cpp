// list.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>
#include <list>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//push_front()	�擪�ɗv�f��ǉ�
	//push_back()	�����ɗv�f��ǉ�
	//pop_front()	�擪�̗v�f���폜
	//pop_back()	�����̗v�f���폜
	//insert()		�v�f��}��
	//erase()		�v�f���폜
	//clear()		�S�v�f���폜

	list<int> li;

	// [12]
	li.push_back(12);	// ���ɑ}��

	// [12],[34]
	li.push_back(34);	// ���ɑ}��

	// [56],[12],[34]
	li.push_front(56);	// �O�ɑ}��

	list<int>::iterator itr;
    itr = li.begin();	// �C�e���[�^��擪�ɐݒ�

	// [56],[78],[12],[34]
    itr++;				// ����Ɉړ�
    li.insert(itr, 78);	// �l�̑}��

	// [56],[78],[34]
	li.remove(*itr);

	//  ���X�g�̒��g��\��
    for (itr = li.begin(); itr != li.end(); itr++)
	{
		cout << "[" << *itr << "] ";
    }

	return 0;
}

