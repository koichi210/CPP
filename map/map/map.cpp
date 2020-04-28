// map.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// map のデータ構造
    map <string, int> price;

	// キー名
	string names[] = { "Potate","Carrot","GreenPepper" };

	// 値
	price[names[0]] = 18;
    price[names[1]] = 38;
    price[names[2]] = 42;

	for(unsigned int i = 0; i < 3; i++)
	{
		cout << names[i] << ":" << price[names[i]]  << endl;
    }

	return 0;
}

