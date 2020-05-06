#pragma once

#include<core/mat.h>

namespace cgcore
{
	/*
	这里提供一些变换的函数
	1. 施密特正交化
	template<typename T, size_t NRow, size_t NRow> mat<T, NRow, NRow>
		SchmidtOrthogonalize(const mat<T, NRow, NRow>& metric);

	2. 线性平移+旋转 -> 4x4 Transform 矩阵

	*/



	//施密特正交化 (metric : 度量矩阵) //TODO
	template<typename T, size_t NRow, size_t NRow>
		mat<T, NRow, NRow> SchmidtOrthogonalize(const mat<T, NRow, NRow>& metric)
	{
		mat<T, NRow, NRow> ret;
		ret.fill(static_cast<T>(0));
		//TODO
	}

	
}