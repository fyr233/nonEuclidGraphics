#pragma once

#include <core/mat.h>
#include <core/tensor.h>

namespace cgcore
{
	/*
	这里提供一些变换的函数
	1. 施密特正交化(模板)
	template<typename T, size_t NRow, size_t NRow> 
		mat<T, NRow, NRow> SchmidtOrthogonalize(const mat<T, NRow, NRow>& metric);

	2. 计算Perspective矩阵
	template<typename T>
		mat<T, 4, 4> Perspective(T fovY, T aspect, T zNear, T zFar)

	3. 计算Model->ParaCoord变换
	template<typename T>
		mat<T, 4, 4> MakeTransform(const mat<T, 3, 3>& S, const mat<T, 3, 3>& R, const mat<T, 3, 3>& scale, vec<T, 3>& center)

	4. 平移向量
		vecf3 Translate(const matf3& g, const tensorf333& gamma, vecf3 du, vecf3 v)

	5. 平移的旋转矩阵 TODO
		void Translate(const matf3& g, const tensorf333& gamma, vecf3 du, matf3 R)

	*/



	//施密特正交化 (metric : 度量矩阵) // UNTESTED (这是个模板函数)
	template<typename T, size_t NRow>
	mat<T, NRow, NRow> SchmidtOrthogonalize(const mat<T, NRow, NRow>& metric)
	{
		mat<T, NRow, NRow> S;
		S.fill(static_cast<T>(0));
		vec<T, NRow> beta;
		for (size_t i = 0; i < NRow; i++)
		{
			//计算 beta_i = S(1, i)r_1 + ... + S(m, i)r_m
			beta.fill(static_cast<T>(0));
			beta[i] = static_cast<T>(1);
			for (size_t l = 0; l < NRow; l++)
				for (size_t j = 0; j < i; j++)
					for (size_t k = 0; k < NRow; k++)
						beta[l] += S(k, j) * metric(i, k) * S(l, j);
			T beta_norm = sqrt(metric.dot_s(beta, beta));
			for (size_t l = 0; l < NRow; l++)
				S(l, i) = beta[l] / beta_norm;
		}
		return S;
	}
	
	//计算Perspective矩阵
	template<typename T>
	mat<T, 4, 4> Perspective(T fovY, T aspect, T zNear, T zFar) {
		assert(fovY > 0 && aspect > 0 && zNear >= 0 && zFar > zNear);

		T tanHalfFovY = std::tan(fovY / static_cast<T>(2));
		float cotHalfFovY = 1 / tanHalfFovY;

		T m00 = cotHalfFovY / aspect;
		T m11 = cotHalfFovY;
		T m22 = (zFar + zNear) / (zNear - zFar);
		T m23 = (2 * zFar * zNear) / (zNear - zFar);

		return mat<T, 4, 4>{
				m00, 0, 0, 0,
				0, m11, 0, 0,
				0, 0, m22, m23,
				0, 0, -1, 0,
		};
	}

	template<typename T>
	mat<T, 4, 4> MakeTransform(const mat<T, 3, 3>& S, const mat<T, 3, 3>& R, const mat<T, 3, 3>& scale, vec<T, 3>& center)
	{
		mat<T, 4, 4> ret;
		mat<T, 3, 3> model2para = mat<T, 3, 3>::dot(mat<T, 3, 3>::dot(S, R), scale);
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				ret(i, j) = model2para(i, j);
		for (size_t i = 0; i < 3; i++)
			ret(3, i) = static_cast<T>(0);
		for (size_t i = 0; i < 3; i++)
			ret(i, 3) = center[i];
		ret(3, 3) = static_cast<T>(1);
		return ret;
	}

	//计算向量平移(返回v+dv)
	vecf3 Translate(const tensorf333& gamma, vecf3 du, vecf3 v);
	
	//计算旋转矩阵平移 TODO
	matf3 Translate(const matf3& S_u1, const matf3& S_u2, const tensorf333& gamma, vecf3 du, const matf3& R);

}