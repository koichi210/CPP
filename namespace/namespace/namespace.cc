// namespace.cc : 名前空間の使い方を確認するコンソールアプリ

#include "stdafx.h"
#include <iostream>

// クラス・変数・関数を名前空間に入れ、Greeting:: で修飾して使う
namespace Greeting
{
	class Portugues {};

	const char* Spanish = "Hola\n";

	void English()
	{
		std::cout << "Hello World\n";
	}
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	Greeting::Portugues portugues;
	(void)portugues;

	std::cout << Greeting::Spanish;
	Greeting::English();

	return 0;
}
