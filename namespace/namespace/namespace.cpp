// namespace.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include<iostream>

using namespace std;

namespace Greeting
{
	class Portugues{};

	char *Spanish = "Hola\n";

	void English()
	{
		cout << "Hello World\n";
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	Greeting::Portugues;

	cout << Greeting::Spanish;
	Greeting::English();

	return 0;
}

