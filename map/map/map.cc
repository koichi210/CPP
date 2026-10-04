// map.cc : std::map の使い方を確認するコンソールアプリ

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
	const std::string names[] = { "Potato", "Carrot", "GreenPepper" };

	// 値
	price[names[0]] = 18;
	price[names[1]] = 38;
	price[names[2]] = 42;

	// 配列の要素数を手書きせず、範囲 for で names を順に回す
	for (const std::string& name : names)
	{
		std::cout << name << ":" << price[name] << std::endl;
	}

	return 0;
}
