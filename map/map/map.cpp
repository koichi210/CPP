// map.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//clear()	全ての要素をクリア
	//erase()	指定要素をクリア
	//empty()	マップがカラか？
	//size()	要素数を取得
	//find()	指定キーと一致する要素のイテレータを取得

	// map のデータ構造（最初の型がキー）
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

