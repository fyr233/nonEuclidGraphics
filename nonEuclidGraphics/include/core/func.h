#pragma once

#include <functional>
#include <core/mat.h>
#include <core/tensor.h>

namespace cgcore
{

	//简化常用函数类型的名称。这是为了避免太长的类型名。

	
	typedef std::function<matf3(const vecf3&)> Func3to33;
	typedef std::function<mat<float, 4, 3>(const vecf3&)> Func3to43;
	typedef std::function<tensorf333(const vecf3&)> Func3to333;
	typedef std::function<vecf4(const vecf3&)> Func3to4;

	//下面的定义使得通过Lambda表达式能直接在函数层面上做运算
	
	//函数相加
	template<typename ret_Type, typename para_Type>
	std::function<ret_Type(para_Type)>operator +(std::function<ret_Type(para_Type)> f1, std::function<ret_Type(para_Type)> f2)
	{
		return [f1, f2](para_Type para) {
			return f1(para) + f2(para);
		};
	}
	
	//复合函数
	template<typename final_Type, typename mid_Type, typename para_Type>
	std::function<final_Type(para_Type)> composite(std::function<final_Type(mid_Type)> f, std::function<mid_Type(para_Type)> g)
	{
		return [f, g](para_Type para) {
			return f(g(para));
		};
	}
	
	//复合函数
	template<typename final_Type, typename mid_Type, typename para_Type>
	std::function<final_Type(para_Type)> composite(std::function<final_Type(mid_Type, mid_Type)> f, std::function<mid_Type(para_Type)> g1, std::function<mid_Type(para_Type)> g2)
	{
		return [f, g1, g2](para_Type para) {
			return f(g1(para), g2(para));
		};
	}
}
