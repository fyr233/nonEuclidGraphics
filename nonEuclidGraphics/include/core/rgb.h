#pragma once

#include <initializer_list>
#include <assert.h>

typedef unsigned char uchar;

namespace cgcore
{
	template<typename T>
	struct rgb
	{
		T r, g, b;

		rgb()
		{
			r = static_cast<T>(0);
			g = static_cast<T>(0);
			b = static_cast<T>(0);
		}

		rgb(const rgb<T>& c)
		{
			r = c.r;
			g = c.g;
			b = c.b;
		}

		T& operator[](size_t i) {
			return *((T*)this + i);
		}

		rgb(T _r, T _g, T _b) :r{ _r }, g{ _g }, b{ _b }
		{}

		rgb(T _t) :r{ _t }, g{_t},b{_t}
		{}

		rgb(const std::initializer_list<T> elem_list)
		{
			assert(elem_list.size() == 3 && "Initializer list must be 3");
			T* p = (T*)this;
			for (auto it : elem_list)
				*p++ = it;
		}

		T illumination() const noexcept {
			return static_cast<T>(0.2126) * r + static_cast<T>(0.7152) * g + static_cast<T>(0.0722) * b;
		}

		T gray() const noexcept {
			return r * static_cast<T>(0.299) + g * static_cast<T>(0.587) + b * static_cast<T>(0.114);
		}

		rgb<T> operator+(const rgb<T>& c2) const
		{
			return rgb<T>(r + c2.r, g + c2.g, b + c2.b);
		}
		rgb<T> operator-(const rgb<T>& c2) const
		{
			return rgb<T>(r - c2.r, g - c2.g, b - c2.b);
		}
		rgb<T> operator*(const rgb <T>& c2) const
		{
			return rgb<T>(r * c2.r, g * c2.g, b * c2.b);
		}

		rgb<T> operator*(const T x) const
		{
			return rgb<T>(r * x, g * x, b * x);
		}
		const rgb<T>& operator=(const rgb<T>& c2)
		{
			r = c2.r; g = c2.g; b = c2.b;
			return *this;
		}
		const rgb<T>& operator+=(const rgb<T>& c2)
		{
			r += c2.r; g += c2.g; b += c2.b;
			return *this;
		}
		const rgb<T>& operator*=(const rgb<T>& c2)
		{
			r *= c2.r; g *= c2.g; b *= c2.b;
			return *this;
		}
		const rgb<T>& operator*=(const T x)
		{
			r *= x; g *= x; b *= x;
			return *this;
		}
		template<typename T2>
		rgb<T2> cast_to()
		{
			return rgb<T2>((T2)r, (T2)g, (T2)b);
		}
	};

	typedef rgb<float> rgbf;
	typedef rgb<uchar> rgbb;
}
