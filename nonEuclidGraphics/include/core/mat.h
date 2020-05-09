#pragma once

#include <core/vec.h>
#include <string.h>

namespace cgcore
{
	// 小型稠密矩阵的模板
	template<typename T, size_t NRow, size_t NCol>
	struct mat
	{
		T* const data = new T[NRow*NCol];
		static const size_t size = NRow * NCol;

		mat(){}
		mat(const mat<T, NRow, NCol>& m)
		{
			memcpy(data, m.data, sizeof(T) * size);
		}
		mat(const T val)
		{
			fill(val);
		}
		
		mat(std::initializer_list<T> elem_list)
		{
			assert(elem_list.size() <= size && "Initializer list is too long");
			size_t i = 0;
			for (auto it : elem_list)
				data[i++] = it;
		}
		~mat() { delete[] data; }

		//通过行、列访问. (irow, joc) 相当于 [irow*Ncol+jcol]
		T& operator()(const size_t irow, const size_t jcol) const 
		{
			assert(irow < NRow && jcol < NCol && "mat:: Index out of range");
			return data[irow * NCol + jcol];
		}

		//直接通过索引访问
		T& operator[](const size_t idx) const 
		{
			assert(idx < size && "mat:: Index out of range");
			return data[idx];
		}

		//赋值运算符
		const mat<T, NRow, NCol>& operator =(const mat<T, NRow, NCol>& m)
		{
			for (size_t i = 0; i < size; i++)
				data[i] = m.data[i];
			return (*this);
		}

		void fill(T val);

		template<size_t L>
		static const mat<T, NRow, L> dot(const mat<T, NRow, NCol>& m1, const mat<T, NCol, L>& m2);

		const vec<T, NRow> dot(const vec<T, NCol>& v) const;

		const mat<T, NRow, NCol> operator*(const T t) const;

		const mat<T, NCol, NRow> transpose() const;
		
		//求矩阵的逆。（ ! 除了3x3矩阵以外，求逆运算是否正确还没有验证）
		mat<T, NCol, NRow> inverse() const;
		
		//交换行i, j
		void SR(size_t i, size_t j);
		//把行 j 的 val 倍加到行 i 上
		void TR(size_t i, size_t j, T val);
		//把行 i 乘以 val 倍
		void DR(size_t i, T val);

		//连续点乘：v1^T.Self.v2
		const T dot_s(const vec<T, NRow>& v1, const vec<T, NCol>& v2) const;

		static mat<T, NRow, NCol> Identity();
		static mat<T, NRow, NRow> Diag(const vec<T, NRow> diag);
	};
	template<typename T, size_t NRow, size_t NCol>
	std::ostream& operator << (std::ostream& ostrm, const mat<T, NRow, NCol>& m);


	typedef mat<float, 3, 3> matf3;
	typedef mat<float, 4, 4> matf4;
	typedef mat<float, 4, 3> matf43;
}

#include <core/mat.inl>