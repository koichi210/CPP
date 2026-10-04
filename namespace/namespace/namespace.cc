// namespace.cc : 名前空間の使い方を確認するコンソールアプリ

#include "stdafx.h"
#include <iostream>

// クラス・変数・関数を名前空間に入れ、greeting:: で修飾して使う
namespace greeting
{
	class Portuguese {};

	const char* spanish = "Hola\n";

	void English()
	{
		std::cout << "Hello World\n";
	}
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	greeting::Portuguese portuguese;
	(void)portuguese;

	std::cout << greeting::spanish;
	greeting::English();

	return 0;
}
