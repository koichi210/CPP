// map.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// map �̃f�[�^�\��
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

