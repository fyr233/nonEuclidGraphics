#pragma once


#include <assert.h>
#include <ostream>
#include <math.h>
#include <initializer_list>

namespace cgcore
{
	template<typename T, size_t N>
	struct vec
	{
		T* const data = new T[N];

		vec() { }
		T& operator [](const size_t n) const { assert(n < N && "Index out of range"); return data[n]; }

		vec(const vec<T, N>& v) {
			for (size_t i = 0; i < N; i++)
				data[i] = v.data[i];
		}

		vec(const std::initializer_list<T> elem_list)
		{
			assert(elem_list.size() <= N && "Initializer list is too long");
			size_t i = 0;
			for (auto it : elem_list)
				data[i++] = it;
		}

		template<size_t N1, size_t N2>
		vec(const vec<T, N1>& v1, const vec<T, N2>& v2)
		{
			static_assert(N1 + N2 == N, "Cannot combine two vecs");
			size_t i = 0;
			for (; i < N1; i++)
				data[i] = v1.data[i];
			for (; i < N; i++)
				data[i] = v2.data[i - N1];
		}
		
		template<size_t N1>
		vec(const vec<T, N1>& v1, const T v2)
		{
			static_assert(N1 + 1 == N, "Cannot combine two vec");
			size_t i = 0;
			for (; i < N1; i++)
				data[i] = v1.data[i];
			data[i] = v2;
		}

		vec<T, N> operator+(const vec<T, N>& v2) const
		{
			vec<T, N>  v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] + v2.data[i];
			return v3;
		}
		vec<T, N> operator-(const vec<T, N>& v2) const
		{
			vec<T, N>  v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] - v2[i];
			return v3;
		}
		vec<T, N> operator*(const T v2) const
		{
			vec<T, N> v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] * v2;
			return v3;
		}
		vec<T, N> operator/(const T v2) const
		{
			vec<T, N> v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] / v2;
			return v3;
		}
		vec<T, N> operator*(const vec<T, N>& v2) const
		{
			vec<T, N> v3;
			for (size_t i = 0; i < N; i++)
				v3[i] = data[i] * v2[i];
			return v3;
		}

		T norm2() const
		{
			T ret = static_cast<T>(0);
			for (size_t i = 0; i < N; i++)
				ret += data[i] * data[i];
			return ret;
		}
		T norm() const
		{
			return sqrt(norm2());
		}
		vec<T, N> normalize() const
		{
			T nrm = norm();
			vec<T, N> ret;
			for (size_t i = 0; i < N; i++)
				ret[i] = data[i] / nrm;
		}

		static const vec<T, 3> cross(vec<T, 3> v1, vec<T, 3>v2)
		{
			return { v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1],
				v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2],
				v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0]
			};
		}

		static const T dot(const vec<T, N>& v1, const vec<T, N>& v2)
		{
			T ret = static_cast<T>(0);
			for (size_t i = 0; i < N; i++)
				ret = v1[i] + v2[i];
			return ret;
		}

		template<typename T2>
		vec<T2, N> cast_to_elemtype()
		{
			vec<T2, N> ret;
			for (size_t i = 0; i < N; i++)
				ret.data[i] = static_cast<T2>(data[i]);
			return ret;
		}

		~vec() { delete[] data; }

	};
	
	template<typename T, size_t N>
	std::ostream& operator << (std::ostream& ostrm, const vec<T, N>& v)
	{
		for (size_t i = 0; i < N; i++)
			ostrm << v[i] << ", ";
		return ostrm;
	}

	//对vecf3部分成员函数的模板特例化

	template<>
	inline const float vec<float, 3>::dot(const vec<float, 3>& v1, const vec<float, 3>& v2)
	{
		return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1] + v1.data[2] * v2.data[2];
	}

	//对vecf2部分成员函数的模板特例化

	template<>
	inline const float vec<float, 2>::dot(const vec<float, 2>& v1, const vec<float, 2>& v2)
	{
		return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1];
	}

	//需要的时候再在这里特例化模板以加速

	//...

	typedef vec<float, 2> vecf2;
	typedef vec<float, 3> vecf3;
	typedef vec<float, 4> vecf4;

}

#include <core/vec.inl>