// TemplateConsole.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>
#include <string>
 
using namespace std;
 
//  �e���v���[�g�֐�
template <typename T>
T add(T x, T y){
    return x + y;
}

int _tmain(int argc, _TCHAR* argv[])
{
    cout << add<string>("ABC", "def") << endl;	// string�𖾎��I�Ɏw��
    cout << add<int>(12, 34) << endl;			// int�𖾎��I�Ɏw��
    cout << add(5, 6) << endl;					// int�̏ꍇ�A�w��ȗ��\

	return 0;
}

