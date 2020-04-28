// list.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <list>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//push_front()	先頭に要素を追加
	//push_back()	末尾に要素を追加
	//pop_front()	先頭の要素を削除
	//pop_back()	末尾の要素を削除
	//insert()		要素を挿入
	//erase()		要素を削除
	//clear()		全要素を削除

	list<int> li;

	// [12]
	li.push_back(12);	// 後ろに挿入

	// [12],[34]
	li.push_back(34);	// 後ろに挿入

	// [56],[12],[34]
	li.push_front(56);	// 前に挿入

	list<int>::iterator itr;
    itr = li.begin();	// イテレータを先頭に設定

	// [56],[78],[12],[34]
    itr++;				// 一つ後ろに移動
    li.insert(itr, 78);	// 値の挿入

	// [56],[78],[34]
	li.remove(*itr);

	//  リストの中身を表示
    for (itr = li.begin(); itr != li.end(); itr++)
	{
		cout << "[" << *itr << "] ";
    }

	return 0;
}

