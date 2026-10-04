// vector.cc : std::vector の使い方を確認するコンソールアプリ

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	// push_back()	要素の追加
	// clear()		要素のクリア
	// size()		配列の大きさを得る関数
	// capacity()	動的配列に追加できる要素の許容量
	// empty()		要素が空かどうかを調べる

	std::vector<int> v1;
	std::vector<std::string> v2;

	v1.push_back(12);
	v1.push_back(34);
	v1.push_back(56);

	v2.push_back("ABC");
	v2.push_back("def");

	for (size_t i = 0; i < v1.size(); i++)
	{
		std::cout << "v1[" << i << "]=" << v1[i] << std::endl;
	}

	for (size_t i = 0; i < v2.size(); i++)
	{
		std::cout << "v2[" << i << "]=" << v2[i] << std::endl;
	}

	return 0;
}
