#pragma once

#include <core/mat.h>

using namespace cgcore;

template<typename T, size_t NRow, size_t NCol>
void mat<T, NRow, NCol>::fill(T val)
{
	T* pe = data + size;
	for (T* p = data; p < pe; p++)
		*p = val;
}

template<typename T, size_t NRow, size_t NCol>
void mat<T, NRow, NCol>::SR(size_t i, size_t j)
{
	T temp;
	T* pe = data + (i + 1) * NCol;
	assert(i < NRow& j < NRow & "mat::SR: Row index out of range");
	for (T* p1 = data + i * NCol, p2 = data + j * NCol; p1 < pe; p1++, p2++)
	{
		temp = *p1; *p1 = *p2; *p2 = temp;
	}
}

template<typename T, size_t NRow, size_t NCol>
void mat<T, NRow, NCol>::TR(size_t i, size_t j, T val)
{
	T temp;
	T* pe = data + (i + 1) * NCol;
	assert(i < NRow& j < NRow & "mat::SR: Row index out of range");
	for (T* p1 = data + i * NCol, p2 = data + j * NCol; p1 < pe; p1++, p2++)
		*p1 += *p2 * val;
}

template<typename T, size_t NRow, size_t NCol>
void mat<T, NRow, NCol>::DR(size_t i, T val)
{
	T temp;
	T* pe = data + (i + 1) * NCol;
	assert(i < NRow & "mat::SR: Row index out of range");
	for (T* p1 = data + i * NCol; p1 < pe; p1++)
		*p1 *= val;
}

template<typename T, size_t NRow, size_t NCol>
inline const T cgcore::mat<T, NRow, NCol>::dot_s(const vec<T, NRow>& v1, const vec<T, NCol>& v2) const
{
	T ret = static_cast<T>(0);
	for (size_t i = 0; i < NRow; i++)
		for (size_t j = 0; j < NCol; j++)
			ret += v1[i] * (*this)(i, j) * v2.data[j];
	return ret;
}

template<typename T, size_t NRow, size_t NCol>
inline mat<T, NRow, NCol> cgcore::mat<T, NRow, NCol>::Identity()
{
	mat<T, NRow, NCol> m;
	m.fill(static_cast<T>(0));
	for (T* p = m.data, *pe = m.data + size; p < pe; p += NCol + 1)
		*p = static_cast<T>(1);
	return m;
}


template<typename T, size_t NRow, size_t NCol>
const vec<T, NRow> mat<T, NRow, NCol>::dot(const vec<T, NCol>& v) const
{
	vec<T, NRow> ret;
	for (size_t i = 0; i < NRow; i++)
	{
		ret.data[i] = static_cast<T>(0);
		for (size_t j = 0; j < NCol; j++)
			ret.data[i] += data[i * NCol + j] * v.data[j];
	}
	return ret;
}

template<typename T, size_t NRow, size_t NCol>
inline const mat<T, NRow, NCol> cgcore::mat<T, NRow, NCol>::operator*(const T t) const
{
	mat<T, NRow, NCol> ret;
	for (size_t i = 0; i < size; i++)
		ret.data[i] = data[i] * t;
	return ret;
}

template<typename T, size_t NRow, size_t NCol>
template<size_t L>
static const mat<T, NRow, L> mat<T, NRow, NCol>::dot(const mat<T, NRow, NCol>& m1, const mat<T, NCol, L>& m2)
{
	mat<T, NRow, L> ret;
	size_t idx;
	for (size_t i = 0; i < NRow; i++)
		for (size_t j = 0; j < L; j++)
		{
			idx = i * L + j;
			ret.data[idx] = static_cast<T>(0);
			for (size_t k = 0; k < NCol; k++)
				ret.data[idx] += m1.data[i * NCol + k] * m2.data[k * L + j];
		}
	return ret;
}

template<typename T, size_t NRow, size_t NCol>
mat<T, NCol, NRow> mat<T, NRow, NCol>::inverse() const
{
	static_assert(NRow == NCol, "Inverse of non-square matrix is not supported");
	mat<T, NCol, NRow> ret;
	mat<T, NCol, NRow> ma(*this);
	size_t firstrownot0, i, j;

	for (i = 0; i < NCol; i++)
	{
		for (firstrownot0 = i; firstrownot0 < NCol && static_cast<T>(0) == mat(firstrownot0, i); firstrownot0++);
		if (firstrownot0 >= NCol) throw "No inverse for singular matrix.";
		if (firstrownot0 != i)
		{
			ma.SR(i, firstrownot0);
			ret.SR(i, firstrownot0);
		}
		for (j = i + 1; j < NCol; j++)
		{
			if (ma(j, i) != 0)
			{
				ma.TR(j, i, -mat.X[j, i] / mat.X[i, i]);
				ret.TR(j, i, -mat.X[j, i] / mat.X[i, i]);
			}
		}
	}
	for (i = NCol; i >= 0; i--)
	{
		ret.DR(i, static_cast<T>(1) / ma(i, i));
		for (j = i - 1; j >= 1; j--)
		{
			if (ma.X[j, i] != 0)
				ret.TR(j, i, -ma(j, i));
		}
	}
	return ret;
}

//对matf3求逆的特例化实现
template<>
inline mat<float, 3, 3> mat<float, 3, 3>::inverse() const
{
	float det = (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1))
		- (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) 
		+ (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
	mat<float, 3, 3> ret;
	for(size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
		{
			ret(j, i) = ((*this)((i + 1) % 3, (j + 1) % 3) * (*this)((i + 2) % 3, (j + 2) % 3) - (*this)((i + 1) % 3, (j + 2) % 3) * (*this)((i + 2) % 3, (j + 1) % 3))/det;
		}
	return ret;
}

template<typename T, size_t NRow, size_t NCol>
const mat<T, NCol, NRow> mat<T, NRow, NCol>::transpose() const
{
	mat<T, NCol, NRow> ret;
	for (size_t i = 0; i < NRow; i++)
		for (size_t j = 0; j < NCol; j++)
			ret.data[j * NRow + i] = data[i * NCol + j];
	return ret;
}

namespace cgcore
{
	//输出矩阵内容，用于调试
	template<typename T, size_t NRow, size_t NCol>
	std::ostream& operator<<(std::ostream& ostrm, const mat<T, NRow, NCol>& m)
	{
		for (size_t i = 0; i < NRow; i++)
		{
			for (size_t j = 0; j < NCol; j++)
			{
				ostrm << m(i, j) << ", ";
			}
			ostrm << std::endl;
		}
		return ostrm;
	}
}