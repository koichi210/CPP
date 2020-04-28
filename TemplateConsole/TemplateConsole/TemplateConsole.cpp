// TemplateConsole.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <string>
 
using namespace std;
 
//  テンプレート関数
template <typename T>
T FuncAdd(T x, T y)
{
    return x + y;
}

template<typename T> class CCalc
{
public:
    T m_n1;
    T m_n2;

    T add() const{
        return m_n1 + m_n2;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    cout << FuncAdd<string>("ABC", "def") << endl;	// stringを明示的に指定
    cout << FuncAdd<int>(12, 34) << endl;			// intを明示的に指定
    cout << FuncAdd(5, 6) << endl;					// intの場合、指定省略可能

    CCalc<int> calc1;
	calc1.m_n1 = 7;
	calc1.m_n2 = 8;
	cout << calc1.add() << endl;

    CCalc<string> calc2;
	calc2.m_n1 = "GHI";
	calc2.m_n2 = "jkl";
	cout << calc2.add() << endl;

	return 0;
}

