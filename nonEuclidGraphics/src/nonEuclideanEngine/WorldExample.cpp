#include <nonEuclideanEngine/WorldExample.h>

using namespace nonEuc::WorldExample;

/////////////////////////////////    超球面空间    /////////////////////////////////////////
void HyperSphere::regularize_ref(vecf3& u, int i, int j, int k)
{
	u[0] = fmod(fmod(u[0] + i * 2 * PI<float>, 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>);
	u[1] = fmod(fmod(u[1], PI<float>) + PI<float>, PI<float>);
	u[2] = fmod(fmod(u[2], PI<float>) + PI<float>, PI<float>);
}

vecf3 HyperSphere::regularize(const vecf3& u, int i, int j, int k)
{
	vecf3 ret;
	ret[0] = fmod(fmod(u[0] + i * 2 * PI<float>, 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>);
	ret[1] = fmod(fmod(u[1], PI<float>) + PI<float>, PI<float>);
	ret[2] = fmod(fmod(u[2], PI<float>) + PI<float>, PI<float>);
	return ret;
}

vecf4 HyperSphere::coord(const vecf3& u)
{
	float sinu1 = sin(u[1]), sinu2 = sin(u[2]);
	return { sin(u[0]) * sinu1 * sinu2, cos(u[0]) * sinu1 * sinu2, cos(u[1]) * sinu2, cos(u[2]) };
}

matf43 HyperSphere::jacobi(const vecf3& u)
{
	float cosu0 = cos(u[0]), sinu0 = sin(u[0]), cosu1 = cos(u[1]), sinu1 = sin(u[1]), cosu2 = cos(u[2]), sinu2 = sin(u[2]);
	return { cosu0 * sinu1 * sinu2, cosu1 * sinu0 * sinu2, cosu2 * sinu0 * sinu1,
		-sinu0 * sinu1 * sinu2, cosu0 * cosu1 * sinu2, cosu0 * cosu2 * sinu1,
		0.f, -sinu1 * sinu2, cosu1 * cosu2,
		0.f, 0.f, -sinu2 };
}

matf3 HyperSphere::metric(const vecf3& u)
{
	/*
	{{Sin[u2]^2 Sin[u3]^2, 0, 0},
	{0,	Sin[u3]^2, 0},
	{0, 0, 2 Cos[u3]^2}}
	*/
	matf3 ret(0.f);
	float sinu1 = sin(u[1]), sinu2 = sin(u[2]);
	ret(0, 0) = sinu1 * sinu1 * sinu2 * sinu2;
	ret(1, 1) = sinu2 * sinu2;
	ret(2, 2) = 1.f;
	return ret;
}

tensorf333 HyperSphere::gamma(const vecf3& u)
{
	/*
	{{{0, Cot[u2], Cot[u3]}, {Cot[u2], 0, 0}, {Cot[u3], 0,0}},
	{{-Cos[u2] Sin[u2], 0, 0}, {0, 0, Cot[u3]}, {0, Cot[u3],0}},
	{{-Cos[u3] Sin[u2]^2 Sin[u3], 0, 0}, {0, -Cos[u3] Sin[u3],0}, {0, 0, 0}}}
	*/
	float sinu1 = sin(u[1]), cosu1 = cos(u[1]), sinu2 = sin(u[2]), cosu2 = cos(u[2]);
	float cotu1 = cosu1 / sinu1;
	float cotu2 = cosu2 / sinu2;

	tensorf333 ret = {
		0.f, cotu1, cotu2,
		cotu1, 0.f, 0.f,
		cotu2, 0.f, 0.f,

		-cosu1 * sinu1, 0.f, 0.f,
		0.f, 0.f, cotu2,
		0.f, cotu2, 0.f,

		-cosu2 * sinu1 * sinu1 * sinu2, 0.f, 0.f,
		0.f, -cosu2 * sinu2, 0.f,
		0.f, 0.f, 0.f
	};
	return ret;
}
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////    欧式空间    /////////////////////////////////////////

void Euclidean::regularize_ref(vecf3& u, int i, int j, int k){}

vecf3 Euclidean::regularize(const vecf3& u, int i, int j, int k)
{
	return u;
}

vecf4 Euclidean::coord(const vecf3& u)
{
	return vecf4(u, 0.f);
}

matf43 Euclidean::jacobi(const vecf3& u)
{
	matf43 ret(0.f);
	ret(0, 0) = 1.f;
	ret(1, 1) = 1.f;
	ret(2, 2) = 1.f;
	return ret;
}

matf3 Euclidean::metric(const vecf3& u)
{
	return matf3::Identity();
}

tensorf333 Euclidean::gamma(const vecf3& u)
{
	return tensorf333(0.f);
}

//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

void nonEuc::WorldExample::Gaussian::regularize_ref(vecf3& u, int i, int j, int k)
{
}

vecf3 nonEuc::WorldExample::Gaussian::regularize(const vecf3& u, int i, int j, int k)
{
	return u;
}

vecf4 nonEuc::WorldExample::Gaussian::coord(const vecf3& u)
{
	return {u[0], u[1], u[2], exp(-u.norm2())};
}

matf43 nonEuc::WorldExample::Gaussian::jacobi(const vecf3& u)
{
	matf43 ret(0.f);
	ret(0, 0) = 1.f;
	ret(1, 1) = 1.f;
	ret(2, 2) = 1.f;
	float e2unrm = -2 * exp(-u.norm2());
	for (size_t i = 0; i < 3; i++)
		ret(3, i) = e2unrm * u[i];
	return ret;
}

matf3 nonEuc::WorldExample::Gaussian::metric(const vecf3& u)
{
	float e2unrm = exp(-2 * u.norm2());
	matf3 ret;
	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			ret(i, j) = 4 * e2unrm * u[i] * u[j] + (i == j ? 1 : 0);
	return ret;
}

tensorf333 nonEuc::WorldExample::Gaussian::gamma(const vecf3& u)
{
	float usqr[3] = {u[0]*u[0], u[1]*u[1], u[2]*u[2]};
	float nrm = u.norm2();
	float fract = exp(2 * nrm) + 4 * nrm;
	tensorf333 ret;
	for (size_t k = 0; k < 3; k++)
		for (size_t i = 0; i < 3; i++)
			for (size_t l = 0; l < 3; l++)
				if (i == l)
					ret(k, i, l) = u[k] * (4 - 8 * usqr[i]) / fract;
				else
					ret(k, i, l) = -8 * u[i] * u[l] * u[k] / fract;
	return ret;
}

void nonEuc::WorldExample::OneRecursive::regularize_ref(vecf3& u, int i, int j, int k)
{
	u[0] = std::fmod(std::fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>;
}

vecf3 nonEuc::WorldExample::OneRecursive::regularize(const vecf3& u, int i, int j, int k)
{
	return { std::fmod(std::fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>, u[1], u[2] };
}

matf3 nonEuc::WorldExample::OneRecursive::metric(const vecf3& u)
{
	return matf3::Identity();
}

tensorf333 nonEuc::WorldExample::OneRecursive::gamma(const vecf3& u)
{
	return tensorf333(0.f);
}


/////////////////////////////////////////////////////////////////////////////
void nonEuc::WorldExample::Helicoid::regularize_ref(vecf3& u, int i, int j, int k)
{
	//u[1] = std::fmod(std::fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>;
}

vecf3 nonEuc::WorldExample::Helicoid::regularize(const vecf3& u, int i, int j, int k)
{
	//return { u[0], std::fmod(std::fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>, u[2] };
	return u;
}

matf3 nonEuc::WorldExample::Helicoid::metric(const vecf3& u)
{
	return matf3{
		1.0f,	0.0f,	0.0f,
		0.0f,	1+u[0]*u[0],	0.0f,
		0.0f,	0.0f,		1.0f,
	};
}

tensorf333 nonEuc::WorldExample::Helicoid::gamma(const vecf3& u)
{
	auto tmp = 1 + u[0] * u[0];
	return tensorf333{
		0.0f, 0.0f, 0.0f,
		0.0f, -u[0], 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, u[0]/tmp, 0.0f,
		u[0]/tmp, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};
}

/////////////////////////////////////////////////////////////////////////////
void nonEuc::WorldExample::Hyperbolic::regularize_ref(vecf3& u, int i, int j, int k)
{
	//u[0] = std::fmod(std::fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>;
	//u[1] = std::fmod(std::fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + j * 2 * PI<float>;
	//u[1] = std::fmod(u[1], 2 * PI<float>);
}

vecf3 nonEuc::WorldExample::Hyperbolic::regularize(const vecf3& u, int i, int j, int k)
{
	//return { std::fmod(std::fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>, 
	//	std::fmod(std::fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + j * 2 * PI<float>, u[2] };
		//std::fmod(u[1], 2 * PI<float>), u[2] };
	return u;
}

matf3 nonEuc::WorldExample::Hyperbolic::metric(const vecf3& u)
{
	float a = 0.1;
	return matf3{
		a*a*u[2]*u[2]*sin(u[1])*sin(u[1]),	0.0f,	0.0f,
		0.0f,	a*a*u[2] * u[2],	0.0f,
		0.0f,	0.0f,		1.0f+a*a,
	};
}

tensorf333 nonEuc::WorldExample::Hyperbolic::gamma(const vecf3& u)
{
	float a = 0.1;
	return tensorf333{
		0.0f, cos(u[1])/sin(u[1]), 1/u[2],
		cos(u[1]) / sin(u[1]), -u[0], 0.0f,
		1 / u[2], 0.0f, 0.0f,

		-cos(u[1])*sin(u[1]),0.0f, 0.0f,
		0.0f, 0.0f, 1/u[2],
		0.0f, 1/u[2], 0.0f,

		-a*a/(1+a*a) * u[2]*sin(u[1])*sin(u[1]), 0.0f, 0.0f,
		0.0f, -a*a/(1+a*a) * u[2], 0.0f,
		0.0f, 0.0f, 0.0f,
	};
}

/////////////////////////////////////////////////////////////////////////////
void nonEuc::WorldExample::Hyperbolic1::regularize_ref(vecf3& u, int i, int j, int k)
{
	
}

vecf3 nonEuc::WorldExample::Hyperbolic1::regularize(const vecf3& u, int i, int j, int k)
{
	return u;
}

matf3 nonEuc::WorldExample::Hyperbolic1::metric(const vecf3& u)
{
	float r2 = u.norm2();
	matf3 rst;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			rst(i, j) = u[i] * u[j] / (r2 * A);
			if (i == j) rst(i, j) += 1.0f;
		}
	return rst;
}

tensorf333 nonEuc::WorldExample::Hyperbolic1::gamma(const vecf3& u)
{
	float r2 = u.norm2();
	tensorf333 rst;
	for (int i = 0; i < 3; i ++)
		for (int k = 0; k < 3; k++)
			for (int l = 0; l < 3; l++)
			{
				rst(i, k, l) = k == l ? (r2-u[k]*u[k]) : (-u[k]*u[l]);
				rst(i, k, l) *= u[i]/((1+A)*(r2*r2));
			}
	return rst;
}

/////////////////////////////////////////////////////////////////////////////
void nonEuc::WorldExample::Hyperbolic2::regularize_ref(vecf3& u, int i, int j, int k)
{

}

vecf3 nonEuc::WorldExample::Hyperbolic2::regularize(const vecf3& u, int i, int j, int k)
{
	return u;
}

matf3 nonEuc::WorldExample::Hyperbolic2::metric(const vecf3& u)
{
	float r2 = u.norm2();
	matf3 rst;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			rst(i, j) = u[i] * u[j] / ((r2+1) * A*A);
			if (i == j) rst(i, j) += 1.0f;
		}
	return rst;
}

tensorf333 nonEuc::WorldExample::Hyperbolic2::gamma(const vecf3& u)
{
	float r2 = u.norm2();
	float t = (1+r2)*(r2+A*A*(1+r2));
	tensorf333 rst;
	for (int i = 0; i < 3; i++)
		for (int k = 0; k < 3; k++)
			for (int l = 0; l < 3; l++)
			{
				rst(i, k, l) = k == l ? (1 + r2 - u[k] * u[k]) : (-u[k] * u[l]);
				rst(i, k, l) *= u[i] / t;
			}
	return rst;
}
