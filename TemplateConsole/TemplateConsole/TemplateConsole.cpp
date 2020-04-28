// TemplateConsole.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <string>
 
using namespace std;
 
//  テンプレート関数
template <typename T>
T add(T x, T y){
    return x + y;
}

int _tmain(int argc, _TCHAR* argv[])
{
    cout << add<string>("ABC", "def") << endl;	// stringを明示的に指定
    cout << add<int>(12, 34) << endl;			// intを明示的に指定
    cout << add(5, 6) << endl;					// intの場合、指定省略可能

	return 0;
}

