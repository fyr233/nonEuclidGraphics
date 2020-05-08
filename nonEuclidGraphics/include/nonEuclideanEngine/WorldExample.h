#pragma once

/*
	在 WorldExpamle.h 中提供了一些参数方程描述的世界;
	直接用这里的 Example 会方便很多。

	TODO
*/

#include <core/mat.h>
#include <core/func.h>
#include <core/geometry.h>

using namespace cgcore;

namespace nonEuc
{
	class WorldExample
	{
	public:
		virtual void regularize_ref(vecf3& v) = 0;
		virtual vecf3 regularize(const vecf3& v) = 0;

		virtual vecf4 coord(const vecf3& v) = 0;
		virtual matf43 jacobi(const vecf3& v) = 0;
		virtual	matf3 metric(const vecf3& v) = 0;
		virtual tensorf333 gamma(const vecf3& v) = 0;
	};


	/*
	3次超球面空间（由于参数表示问题：存在若干奇点）
	*/
	class HyperSphere : WorldExample
	{
	public:
		void regularize_ref(vecf3& u);

		vecf3 regularize(const vecf3& u);

		vecf4 coord(const vecf3& u);

		matf43 jacobi(const vecf3& u);

		matf3 metric(const vecf3& u);

		tensorf333 gamma(const vecf3& u);
	};

}