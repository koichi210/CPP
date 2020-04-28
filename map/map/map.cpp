// map.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//clear()	�S�Ă̗v�f���N���A
	//erase()	�w��v�f���N���A
	//empty()	�}�b�v���J�����H
	//size()	�v�f�����擾
	//find()	�w��L�[�ƈ�v����v�f�̃C�e���[�^���擾

	// map �̃f�[�^�\���i�ŏ��̌^���L�[�j
    map <string, int> price;

	// �L�[��
	string names[] = { "Potate","Carrot","GreenPepper" };

	// �l
	price[names[0]] = 18;
    price[names[1]] = 38;
    price[names[2]] = 42;

	for(unsigned int i = 0; i < 3; i++)
	{
		cout << names[i] << ":" << price[names[i]]  << endl;
    }

	return 0;
}

