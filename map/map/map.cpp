// map.cpp : std::map の使い方を確認するコンソールアプリ

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	// clear()	全ての要素をクリア
	// erase()	指定要素をクリア
	// empty()	マップが空か？
	// size()	要素数を取得
	// find()	指定キーと一致する要素のイテレータを取得

	// map のデータ構造（最初の型がキー）
	std::map<std::string, int> price;

	// キー名
	std::string names[] = { "Potate", "Carrot", "GreenPepper" };

	// 値
	price[names[0]] = 18;
	price[names[1]] = 38;
	price[names[2]] = 42;

	for (unsigned int i = 0; i < 3; i++)
	{
		std::cout << names[i] << ":" << price[names[i]] << std::endl;
	}

	return 0;
}
