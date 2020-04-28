// vector.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
 
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// push_back()	�v�f�̒ǉ�
	// clear()		�v�f�̃N���A
	// size()		�z��̑傫���𓾂�֐�
	// capacity()	���I�z��ɒǉ��ł���v�f�̋��e��
	// empty()		�v�f���󂩂ǂ����𒲂ׂ�

	vector<int> v1;
    vector<string> v2;

	v1.push_back(12);
    v1.push_back(34);
    v1.push_back(56);

	v2.push_back("ABC");
    v2.push_back("def");

	for (unsigned int i = 0; i < v1.size(); i++)
    {
        cout << "v1[" << i << "]=" << v1[i] << endl;
    }

	for (unsigned int i = 0; i < v2.size(); i++)
    {
        cout << "v2[" << i << "]=" << v2[i] << endl;
    }

	return 0;
}
