#include <core/transform.h>

//计算向量平移(返回v+dv)
vecf3 Translate(const tensorf333& gamma, vecf3 du, vecf3 v)
{
	vecf3 dv;
	for (size_t i = 0; i < 3; i++)
	{
		dv[i] = 0.f;
		for (size_t k = 0; k < 3; k++)
			for (size_t l = 0; l < 3; l++)
				dv[i] += gamma(i, k, l) * du[i] * v[i];
	}
	return v + dv;
}

//计算旋转矩阵平移
void Translate(const matf3& g, const tensorf333& gamma, vecf3 du, matf3 R)
{
	//TODO
}