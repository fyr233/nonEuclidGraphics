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
		const size_t size = NRow * NCol;

		mat(){}
		mat(const mat<T, NRow, NCol>& m)
		{
			for (size_t i = 0; i < size; i++)
				data[i] = m.data[i];
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

		T& operator()(const size_t irow, const size_t jcol) const 
		{
			assert(irow < NRow && jcol < NCol && "mat:: Index out of range");
			return data[irow * NCol + jcol];
		}
		T& operator[](const size_t idx) const 
		{
			assert(idx < size && "mat:: Index out of range");
			return data[idx];
		}

		void fill(T val);

		template<size_t L>
		static const mat<T, NRow, L> dot(const mat<T, NRow, NCol>& m1, const mat<T, NCol, L>& m2);

		const vec<T, NRow> dot(const vec<T, NRow> v) const
		{
			vec<T, NRow> ret;
			for (size_t i = 0; i < NRow; i++)
			{
				ret.data[i] = static_cast<T>(0);
				for (size_t j = 0; j < NCol; j++)
					ret.data[i] += data[i * NCol + j] * v.data[i];
			}
			return ret;
		}

		const mat<T, NRow, NCol> operator*(const T t) const
		{
			mat<T, NRow, NCol> ret;
			for (size_t i = 0; i < size; i++)
				ret.data[i] = data[i] * t;
			return ret;
		}

		const mat<T, NCol, NRow> transpose() const;

		mat<T, NCol, NRow> inverse() const;
		
		void SR(size_t i, size_t j);
		void TR(size_t i, size_t j, T val);
		void DR(size_t i, T val);
		
	};
	template<typename T, size_t NRow, size_t NCol>
	std::ostream& operator << (std::ostream& ostrm, const mat<T, NRow, NCol>& m);


	typedef mat<float, 3, 3> matf3;
}

#include <core/mat.inl>