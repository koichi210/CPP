// vector.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
 
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// push_back()	要素の追加
	// clear()		要素のクリア
	// size()		配列の大きさを得る関数
	// capacity()	動的配列に追加できる要素の許容量
	// empty()		要素が空かどうかを調べる

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
