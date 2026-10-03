// template_console.cc : 関数テンプレート・クラステンプレートの確認用コンソールアプリ

#include "stdafx.h"
#include <iostream>
#include <string>

// 関数テンプレート
template <typename T>
T FuncAdd(T x, T y)
{
	return x + y;
}

// 関数テンプレート（型引数が複数）
template <typename T, typename S>
S FuncMul(T x, S y)
{
	return x * y;
}

// クラステンプレート
template <typename T>
class Calc
{
public:
	T n1_;
	T n2_;

	T Add() const
	{
		return n1_ + n2_;
	}
};

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	std::cout << FuncAdd<std::string>("ABC", "def") << std::endl;	// string を明示的に指定
	std::cout << FuncAdd<int>(12, 34) << std::endl;				// int を明示的に指定
	std::cout << FuncAdd(5, 6) << std::endl;						// 引数から推論できるので省略可能

	std::cout << FuncMul<int, double>(20, 1.5) << std::endl;		// 型引数を複数指定

	Calc<int> calc1;
	calc1.n1_ = 7;
	calc1.n2_ = 8;
	std::cout << calc1.Add() << std::endl;

	Calc<std::string> calc2;
	calc2.n1_ = "GHI";
	calc2.n2_ = "jkl";
	std::cout << calc2.Add() << std::endl;

	return 0;
}
