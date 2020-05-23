#include <nonEuclideanEngine/WorldExample.h>

using namespace nonEuc::WorldExample;

/////////////////////////////////    超球面空间    /////////////////////////////////////////
void HyperSphere::regularize_ref(vecf3& u, int i, int j, int k)
{
	u[0] = fmod(fmod(u[0] , 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>;
	u[1] = fmod(fmod(u[1], PI<float>) + PI<float>, PI<float>);
	u[2] = fmod(fmod(u[2], PI<float>) + PI<float>, PI<float>);
}

vecf3 HyperSphere::regularize(const vecf3& u, int i, int j, int k)
{
	vecf3 ret;
	ret[0] = fmod(fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>;
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
void nonEuc::WorldExample::Hyperbolic1::regularize_ref(vecf3& u, int i, int j, int k)
{
	u[0] = std::fmod(std::fmod(u[0], PI<float>) + PI<float>, PI<float>);
	u[1] = std::fmod(std::fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>);
	u[2] = abs(u[2] - 1) + 1;
}

vecf3 nonEuc::WorldExample::Hyperbolic1::regularize(const vecf3& u, int i, int j, int k)
{
	return { std::fmod(std::fmod(u[0], PI<float>) + PI<float>, PI<float>),
		std::fmod(std::fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>), abs(u[2] - 1) + 1 };
}

matf3 nonEuc::WorldExample::Hyperbolic1::metric(const vecf3& u)
{
	float  arc = 0.5*acos(1 / (u[2]*u[2]));
	float sinarc2 = sin(arc)*sin(arc);
	return matf3{
		sinarc2,	0.0f,	0.0f,
		0.0f,	sin(u[0])*sin(u[0])*sinarc2,	0.0f,
		0.0f,	0.0f,		1/(sinarc2*(2+2*u[2]*u[2])),
	}*u[2] * u[2];
}

tensorf333 nonEuc::WorldExample::Hyperbolic1::gamma(const vecf3& u)
{
	float a = 0.5* acos(1/(u[2]*u[2]));
	float sina = sin(a);
	float cosa = cos(a);
	float b = sqrt(1-1/pow(u[2], 4));
	float m = 1 / u[2] + b * u[2] * cosa / sina / (-1 + pow(u[2], 4));
	float t = -(1 / (b * pow(u[2], 3))) * 2 * (1 + u[2] * u[2]) * pow(sina, 3) * (cosa + b * u[2] * u[2] * sina);
	return tensorf333{
		0.0f, 0.0f, m,
		0.0f, -cos(u[0])*sin(u[0]), 0.0f,
		m, 0.0f, 0.0f,

		0.0f, cos(u[0])/sin(u[0]), 0.0f,
		cos(u[0]) / sin(u[0]), 0.0f, m,
		0.0f, m, 0.0f,

		t, 0.0f, 0.0f,
		0.0f, t*sin(u[0])*sin(u[0]), 0.0f,
		0.0f, 0.0f, -1/(sina*sina*(pow(u[2],3)+pow(u[2],5))),
	};
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

/////////////////////////////////////////////////////////////////////////////
void nonEuc::WorldExample::Hyperbolic3::regularize_ref(vecf3& u, int i, int j, int k)
{
	u[1] = std::fmod(std::fmod(u[1], PI<float>) + PI<float>, PI<float>);
	u[0] = std::fmod(std::fmod(u[0], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + 2 * i * PI<float>;
}

vecf3 nonEuc::WorldExample::Hyperbolic3::regularize(const vecf3& u, int i, int j, int k)
{
	return u;
}

matf3 nonEuc::WorldExample::Hyperbolic3::metric(const vecf3& u)
{
	float coshu2 = cosh(u[2]);
	float cosh2u2 = cosh(2*u[2]);
	float sinu1 = sin(u[1]);
	return matf3{
		coshu2*coshu2*sinu1*sinu1, 0, 0,
		0, coshu2*coshu2, 0,
		0, 0, cosh2u2,
	};
}

tensorf333 nonEuc::WorldExample::Hyperbolic3::gamma(const vecf3& u)
{
	float sinu1 = sin(u[1]);
	float cosu1 = cos(u[1]);
	float cotu1 = cosu1/sinu1;
	float tanhu2 = tanh(u[2]);
	float tanh2u2 = tanh(2*u[2]);
	return tensorf333{
		0.0f, cotu1, tanhu2,
		cotu1, 0.0f, 0.0f,
		tanhu2, 0.0f, 0.0f,

		-cosu1*sinu1, 0.0f, 0.0f,
		0.0f, 0.0f, tanhu2,
		0.0f, tanhu2, 0.0f,

		-0.5f*sinu1*sinu1*tanh2u2, 0.0f, 0.0f,
		0.0f, -0.5f*tanh2u2, 0.0f,
		0.0f, 0.0f, tanh2u2,
	};
}

void nonEuc::WorldExample::Schwarzschild::regularize_ref(vecf3& u, int i, int j, int k)
{
	u[1] = fmod(fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float>;
}

vecf3 nonEuc::WorldExample::Schwarzschild::regularize(const vecf3& u, int i, int j, int k)
{
	return vecf3({u[0], fmod(fmod(u[1], 2 * PI<float>) + 2 * PI<float>, 2 * PI<float>) + i * 2 * PI<float> , u[2]});
}

matf3 nonEuc::WorldExample::Schwarzschild::metric(const vecf3& u)
{
	float sinthe = sin(u[2]);
	float rsqr = u[0] * u[0];
	float A = 0.2f;
	return matf3{
		1.f/(1.f- A/rsqr), 0.f, 0.f,
		0.f, rsqr*sinthe*sinthe, 0.f,
		0.f, 0.f, rsqr
	};
}

tensorf333 nonEuc::WorldExample::Schwarzschild::gamma(const vecf3& u)
{
	float sinthe = sin(u[2]);
	float invr = 1.f / u[0];
	float rsqr = u[0] * u[0];
	float cotth = 1.f / tan(u[2]);
	cotth = isnan(cotth) ? 0.f : cotth;
	float A = 0.2f;
	return tensorf333({
		invr * (A / (A - rsqr)), 0.f, 0.f,
		0.f, (A - rsqr) * sinthe * sinthe * invr, 0.f,
		0.f, 0.f, A * invr - u[0],
		0.f, invr, 0.f,
		invr, 0.f, cotth,
		0.f, cotth, 0.f,
		0.f, 0.f, invr,
		0.f, -cos(u[2]) * sinthe, 0.f,
		invr, 0.f, 0.f
	});
}

void nonEuc::WorldExample::Schwarzschild2::regularize_ref(vecf3& u, int i, int j, int k)
{
}

vecf3 nonEuc::WorldExample::Schwarzschild2::regularize(const vecf3& u, int i, int j, int k)
{
	return u;
}

matf3 nonEuc::WorldExample::Schwarzschild2::metric(const vecf3& u)
{
	matf3 ret(0.f);
	float r = u.norm();
	float rsqr = r * r, rcb = rsqr*r;
	float a = 0.2f;
	float fract = rsqr * (r - a);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j)
			{
				ret(i, i) = (rcb - a * (rsqr - u[i] * u[i])) / fract;
			}
			else
			{
				ret(i, j) = 2 * a * u[i] * u[j] / fract;
			}
		}
	}
	return ret;
}

tensorf333 nonEuc::WorldExample::Schwarzschild2::gamma(const vecf3& u)
{
	float a = 0.2f;
	float r = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
	float pw[3][10];
	for (int i = 0; i < 3; i++)
	{
		pw[i][0] = 1.f;
		for (int j = 1; j < 10; j++)
			pw[i][j] = pw[i][j - 1] * u[i];
	}
	float pr[10], pa[10];
	pr[0] = 1.f;
	pa[0] = 1.f;
	for (int j = 1; j < 10; j++)
	{
		pr[j] = pr[j - 1] * r;
		pa[j] = pa[j - 1] * a;
	}
	tensorf333 ret({
		-(pr[8] * pw[0][3]) + 2 * a * pr[5] * pw[0][5] - pa[2] *  pw[0][9] + 2 * pr[8] * u[0] * pw[1][2] + 5 * a * pr[5] * pw[0][3] * pw[1][2] +
	  2 * pa[3] * r * pw[0][5] * pw[1][2] - 10 * pa[2] * pw[0][7] * pw[1][2] - 12 * a * pr[5] * u[0] * pw[1][4] - 6 * pa[3] * r * pw[0][3] * pw[1][4] +
	  pa[2] * pw[0][5] * pw[1][4] - 8 * pa[3] * r * u[0] * pw[1][6] + 28 * pa[2] * pw[0][3] * pw[1][6] + 18 * pa[2] * u[0] * pw[1][8] + 2 * pr[8] * u[0] * pw[2][2] +
	  5 * a * pr[5] * pw[0][3] * pw[2][2] + 2 * pa[3] * r * pw[0][5] * pw[2][2] - 10 * pa[2] * pw[0][7] * pw[2][2] - 24 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] -
	  4 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] - 15 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] - 34 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] +
	  77 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] + 82 * pa[2] * u[0] * pw[1][6] * pw[2][2] - 12 * a * pr[5] * u[0] * pw[2][4] - 6 * pa[3] * r * pw[0][3] * pw[2][4] +
	  pa[2] * pw[0][5] * pw[2][4] - 34 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] + 77 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] +
	  128 * pa[2] * u[0] * pw[1][4] * pw[2][4] - 8 * pa[3] * r * u[0] * pw[2][6] + 28 * pa[2] * pw[0][3] * pw[2][6] + 82 * pa[2] * u[0] * pw[1][2] * pw[2][6] +
	  18 * pa[2] * u[0] * pw[2][8],-3 * pr[8] * pw[0][2] * u[1] + 8 * a * pr[5] * pw[0][4] * u[1] + 2 * pa[3] * r * pw[0][6] * u[1] - 7 * pa[2] * pw[0][8] * u[1] +
	  11 * a * pr[5] * pw[0][2] * pw[1][3] + 6 * pa[3] * r * pw[0][4] * pw[1][3] - 26 * pa[2] * pw[0][6] * pw[1][3] + 4 * pa[3] * r * pw[0][2] * pw[1][5] -
	  31 * pa[2] * pw[0][4] * pw[1][5] - 12 * pa[2] * pw[0][2] * pw[1][7] + 17 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] + 10 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] -
	  36 * pa[2] * pw[0][6] * u[1] * pw[2][2] + 18 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] - 91 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] -
	  55 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] + 8 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] - 51 * pa[2] * pw[0][4] * u[1] * pw[2][4] -
	  65 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] - 22 * pa[2] * pw[0][2] * u[1] * pw[2][6],
	 -3 * pr[8] * pw[0][2] * u[2] + 8 * a * pr[5] * pw[0][4] * u[2] + 2 * pa[3] * r * pw[0][6] * u[2] - 7 * pa[2] * pw[0][8] * u[2] + 17 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] +
	  10 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] - 36 * pa[2] * pw[0][6] * pw[1][2] * u[2] + 8 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] - 51 * pa[2] * pw[0][4] * pw[1][4] * u[2] -
	  22 * pa[2] * pw[0][2] * pw[1][6] * u[2] + 11 * a * pr[5] * pw[0][2] * pw[2][3] + 6 * pa[3] * r * pw[0][4] * pw[2][3] - 26 * pa[2] * pw[0][6] * pw[2][3] +
	  18 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] - 91 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] - 65 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] +
	  4 * pa[3] * r * pw[0][2] * pw[2][5] - 31 * pa[2] * pw[0][4] * pw[2][5] - 55 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] - 12 * pa[2] * pw[0][2] * pw[2][7],
	  -3 * pr[8] * pw[0][2] * u[1] + 8 * a * pr[5] * pw[0][4] * u[1] + 2 * pa[3] * r * pw[0][6] * u[1] - 7 * pa[2] * pw[0][8] * u[1] + 11 * a * pr[5] * pw[0][2] * pw[1][3] +
	  6 * pa[3] * r * pw[0][4] * pw[1][3] - 26 * pa[2] * pw[0][6] * pw[1][3] + 4 * pa[3] * r * pw[0][2] * pw[1][5] - 31 * pa[2] * pw[0][4] * pw[1][5] -
	  12 * pa[2] * pw[0][2] * pw[1][7] + 17 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] + 10 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] - 36 * pa[2] * pw[0][6] * u[1] * pw[2][2] +
	  18 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] - 91 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] - 55 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] +
	  8 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] - 51 * pa[2] * pw[0][4] * u[1] * pw[2][4] - 65 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] -
	  22 * pa[2] * pw[0][2] * u[1] * pw[2][6],4 * pr[8] * pw[0][3] - 12 * a * pr[5] * pw[0][5] - 4 * pa[3] * r * pw[0][7] + 12 * pa[2] *  pw[0][9] -
	  5 * pr[8] * u[0] * pw[1][2] - 6 * pa[3] * r * pw[0][5] * pw[1][2] + 27 * pa[2] * pw[0][7] * pw[1][2] + 9 * a * pr[5] * u[0] * pw[1][4] -
	  2 * pa[3] * r * pw[0][3] * pw[1][4] + 14 * pa[2] * pw[0][5] * pw[1][4] - 5 * pa[2] * pw[0][3] * pw[1][6] - 4 * pa[2] * u[0] * pw[1][8] +
	  4 * pr[8] * u[0] * pw[2][2] - 28 * a * pr[5] * pw[0][3] * pw[2][2] - 16 * pa[3] * r * pw[0][5] * pw[2][2] + 56 * pa[2] * pw[0][7] * pw[2][2] +
	  5 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] - 14 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] + 90 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] -
	  2 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] + 25 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] - 9 * pa[2] * u[0] * pw[1][6] * pw[2][2] - 16 * a * pr[5] * u[0] * pw[2][4] -
	  20 * pa[3] * r * pw[0][3] * pw[2][4] + 96 * pa[2] * pw[0][5] * pw[2][4] - 8 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] +
	  99 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] + 11 * pa[2] * u[0] * pw[1][4] * pw[2][4] - 8 * pa[3] * r * u[0] * pw[2][6] + 72 * pa[2] * pw[0][3] * pw[2][6] +
	  36 * pa[2] * u[0] * pw[1][2] * pw[2][6] + 20 * pa[2] * u[0] * pw[2][8],
	 16 * a * pr[7] * u[0] * u[1] * u[2] - 6 * pr[8] * u[0] * u[1] * u[2] + 4 * pa[3] * r * pw[0][5] * u[1] * u[2] - 14 * pa[2] * pw[0][7] * u[1] * u[2] + 8 * pa[3] * r * pw[0][3] * pw[1][3] * u[2] -
	  42 * pa[2] * pw[0][5] * pw[1][3] * u[2] + 4 * pa[3] * r * u[0] * pw[1][5] * u[2] - 42 * pa[2] * pw[0][3] * pw[1][5] * u[2] - 14 * pa[2] * u[0] * pw[1][7] * u[2] +
	  8 * pa[3] * r * pw[0][3] * u[1] * pw[2][3] - 42 * pa[2] * pw[0][5] * u[1] * pw[2][3] + 4 * pa[3] * r * u[0] * pw[1][3] * pw[2][3] -
	  78 * pa[2] * pw[0][3] * pw[1][3] * pw[2][3] - 36 * pa[2] * u[0] * pw[1][5] * pw[2][3] + 4 * pa[3] * r * u[0] * u[1] * pw[2][5] - 42 * pa[2] * pw[0][3] * u[1] * pw[2][5] -
	  36 * pa[2] * u[0] * pw[1][3] * pw[2][5] - 14 * pa[2] * u[0] * u[1] * pw[2][7],
	  -3 * pr[8] * pw[0][2] * u[2] + 8 * a * pr[5] * pw[0][4] * u[2] + 2 * pa[3] * r * pw[0][6] * u[2] - 7 * pa[2] * pw[0][8] * u[2] + 17 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] +
	  10 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] - 36 * pa[2] * pw[0][6] * pw[1][2] * u[2] + 8 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] - 51 * pa[2] * pw[0][4] * pw[1][4] * u[2] -
	  22 * pa[2] * pw[0][2] * pw[1][6] * u[2] + 11 * a * pr[5] * pw[0][2] * pw[2][3] + 6 * pa[3] * r * pw[0][4] * pw[2][3] - 26 * pa[2] * pw[0][6] * pw[2][3] +
	  18 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] - 91 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] - 65 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] +
	  4 * pa[3] * r * pw[0][2] * pw[2][5] - 31 * pa[2] * pw[0][4] * pw[2][5] - 55 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] - 12 * pa[2] * pw[0][2] * pw[2][7],
	 16 * a * pr[7] * u[0] * u[1] * u[2] - 6 * pr[8] * u[0] * u[1] * u[2] + 4 * pa[3] * r * pw[0][5] * u[1] * u[2] - 14 * pa[2] * pw[0][7] * u[1] * u[2] + 8 * pa[3] * r * pw[0][3] * pw[1][3] * u[2] -
	  42 * pa[2] * pw[0][5] * pw[1][3] * u[2] + 4 * pa[3] * r * u[0] * pw[1][5] * u[2] - 42 * pa[2] * pw[0][3] * pw[1][5] * u[2] - 14 * pa[2] * u[0] * pw[1][7] * u[2] +
	  8 * pa[3] * r * pw[0][3] * u[1] * pw[2][3] - 42 * pa[2] * pw[0][5] * u[1] * pw[2][3] + 4 * pa[3] * r * u[0] * pw[1][3] * pw[2][3] -
	  78 * pa[2] * pw[0][3] * pw[1][3] * pw[2][3] - 36 * pa[2] * u[0] * pw[1][5] * pw[2][3] + 4 * pa[3] * r * u[0] * u[1] * pw[2][5] - 42 * pa[2] * pw[0][3] * u[1] * pw[2][5] -
	  36 * pa[2] * u[0] * pw[1][3] * pw[2][5] - 14 * pa[2] * u[0] * u[1] * pw[2][7],
	 4 * pr[8] * pw[0][3] - 12 * a * pr[5] * pw[0][5] - 4 * pa[3] * r * pw[0][7] + 12 * pa[2] *  pw[0][9] + 4 * pr[8] * u[0] * pw[1][2] -
	  28 * a * pr[5] * pw[0][3] * pw[1][2] - 16 * pa[3] * r * pw[0][5] * pw[1][2] + 56 * pa[2] * pw[0][7] * pw[1][2] - 16 * a * pr[5] * u[0] * pw[1][4] -
	  20 * pa[3] * r * pw[0][3] * pw[1][4] + 96 * pa[2] * pw[0][5] * pw[1][4] - 8 * pa[3] * r * u[0] * pw[1][6] + 72 * pa[2] * pw[0][3] * pw[1][6] +
	  20 * pa[2] * u[0] * pw[1][8] - 5 * pr[8] * u[0] * pw[2][2] - 6 * pa[3] * r * pw[0][5] * pw[2][2] + 27 * pa[2] * pw[0][7] * pw[2][2] +
	  5 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] - 14 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] + 90 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] -
	  8 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] + 99 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] + 36 * pa[2] * u[0] * pw[1][6] * pw[2][2] + 9 * a * pr[5] * u[0] * pw[2][4] -
	  2 * pa[3] * r * pw[0][3] * pw[2][4] + 14 * pa[2] * pw[0][5] * pw[2][4] - 2 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] +
	  25 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] + 11 * pa[2] * u[0] * pw[1][4] * pw[2][4] - 5 * pa[2] * pw[0][3] * pw[2][6] -
	  9 * pa[2] * u[0] * pw[1][2] * pw[2][6] - 4 * pa[2] * u[0] * pw[2][8],
      -5 * pr[8] * pw[0][2] * u[1] + 9 * a * pr[5] * pw[0][4] * u[1] - 4 * pa[2] * pw[0][8] * u[1] + 4 * pr[8] * pw[1][3] - 2 * pa[3] * r * pw[0][4] * pw[1][3] -
	  5 * pa[2] * pw[0][6] * pw[1][3] - 12 * a * pr[5] * pw[1][5] - 6 * pa[3] * r * pw[0][2] * pw[1][5] + 14 * pa[2] * pw[0][4] * pw[1][5] -
	  4 * pa[3] * r * pw[1][7] + 27 * pa[2] * pw[0][2] * pw[1][7] + 12 * pa[2] * pw[1][9] + 4 * pr[8] * u[1] * pw[2][2] + 5 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] -
	  2 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] - 9 * pa[2] * pw[0][6] * u[1] * pw[2][2] - 28 * a * pr[5] * pw[1][3] * pw[2][2] -
	  14 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] + 25 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] - 16 * pa[3] * r * pw[1][5] * pw[2][2] +
	  90 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] + 56 * pa[2] * pw[1][7] * pw[2][2] - 16 * a * pr[5] * u[1] * pw[2][4] - 8 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] +
	  11 * pa[2] * pw[0][4] * u[1] * pw[2][4] - 20 * pa[3] * r * pw[1][3] * pw[2][4] + 99 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] +
	  96 * pa[2] * pw[1][5] * pw[2][4] - 8 * pa[3] * r * u[1] * pw[2][6] + 36 * pa[2] * pw[0][2] * u[1] * pw[2][6] + 72 * pa[2] * pw[1][3] * pw[2][6] +
	  20 * pa[2] * u[1] * pw[2][8],-3 * pr[8] * u[0] * pw[1][2] + 11 * a * pr[5] * pw[0][3] * pw[1][2] + 4 * pa[3] * r * pw[0][5] * pw[1][2] -
	  12 * pa[2] * pw[0][7] * pw[1][2] + 8 * a * pr[5] * u[0] * pw[1][4] + 6 * pa[3] * r * pw[0][3] * pw[1][4] - 31 * pa[2] * pw[0][5] * pw[1][4] +
	  2 * pa[3] * r * u[0] * pw[1][6] - 26 * pa[2] * pw[0][3] * pw[1][6] - 7 * pa[2] * u[0] * pw[1][8] + 17 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] +
	  18 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] - 55 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] + 10 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] -
	  91 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] - 36 * pa[2] * u[0] * pw[1][6] * pw[2][2] + 8 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] -
	  65 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] - 51 * pa[2] * u[0] * pw[1][4] * pw[2][4] - 22 * pa[2] * u[0] * pw[1][2] * pw[2][6],
	 16 * a * pr[7] * u[0] * u[1] * u[2] - 6 * pr[8] * u[0] * u[1] * u[2] + 4 * pa[3] * r * pw[0][5] * u[1] * u[2] - 14 * pa[2] * pw[0][7] * u[1] * u[2] + 8 * pa[3] * r * pw[0][3] * pw[1][3] * u[2] -
	  42 * pa[2] * pw[0][5] * pw[1][3] * u[2] + 4 * pa[3] * r * u[0] * pw[1][5] * u[2] - 42 * pa[2] * pw[0][3] * pw[1][5] * u[2] - 14 * pa[2] * u[0] * pw[1][7] * u[2] +
	  4 * pa[3] * r * pw[0][3] * u[1] * pw[2][3] - 36 * pa[2] * pw[0][5] * u[1] * pw[2][3] + 8 * pa[3] * r * u[0] * pw[1][3] * pw[2][3] -
	  78 * pa[2] * pw[0][3] * pw[1][3] * pw[2][3] - 42 * pa[2] * u[0] * pw[1][5] * pw[2][3] + 4 * pa[3] * r * u[0] * u[1] * pw[2][5] - 36 * pa[2] * pw[0][3] * u[1] * pw[2][5] -
	  42 * pa[2] * u[0] * pw[1][3] * pw[2][5] - 14 * pa[2] * u[0] * u[1] * pw[2][7],
      -3 * pr[8] * u[0] * pw[1][2] + 11 * a * pr[5] * pw[0][3] * pw[1][2] + 4 * pa[3] * r * pw[0][5] * pw[1][2] - 12 * pa[2] * pw[0][7] * pw[1][2] +
	  8 * a * pr[5] * u[0] * pw[1][4] + 6 * pa[3] * r * pw[0][3] * pw[1][4] - 31 * pa[2] * pw[0][5] * pw[1][4] + 2 * pa[3] * r * u[0] * pw[1][6] -
	  26 * pa[2] * pw[0][3] * pw[1][6] - 7 * pa[2] * u[0] * pw[1][8] + 17 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] + 18 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] -
	  55 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] + 10 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] - 91 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] -
	  36 * pa[2] * u[0] * pw[1][6] * pw[2][2] + 8 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] - 65 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] -
	  51 * pa[2] * u[0] * pw[1][4] * pw[2][4] - 22 * pa[2] * u[0] * pw[1][2] * pw[2][6],
	 2 * pr[8] * pw[0][2] * u[1] - 12 * a * pr[5] * pw[0][4] * u[1] - 8 * pa[3] * r * pw[0][6] * u[1] + 18 * pa[2] * pw[0][8] * u[1] - pr[8] * pw[1][3] +
	  5 * a * pr[5] * pw[0][2] * pw[1][3] - 6 * pa[3] * r * pw[0][4] * pw[1][3] + 28 * pa[2] * pw[0][6] * pw[1][3] + 2 * a * pr[5] * pw[1][5] +
	  2 * pa[3] * r * pw[0][2] * pw[1][5] + pa[2] * pw[0][4] * pw[1][5] - 10 * pa[2] * pw[0][2] * pw[1][7] - pa[2] * pw[1][9] +
	  2 * pr[8] * u[1] * pw[2][2] - 24 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] - 34 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] + 82 * pa[2] * pw[0][6] * u[1] * pw[2][2] +
	  5 * a * pr[5] * pw[1][3] * pw[2][2] - 4 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] + 77 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] +
	  2 * pa[3] * r * pw[1][5] * pw[2][2] - 15 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] - 10 * pa[2] * pw[1][7] * pw[2][2] - 12 * a * pr[5] * u[1] * pw[2][4] -
	  34 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] + 128 * pa[2] * pw[0][4] * u[1] * pw[2][4] - 6 * pa[3] * r * pw[1][3] * pw[2][4] +
	  77 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] + pa[2] * pw[1][5] * pw[2][4] - 8 * pa[3] * r * u[1] * pw[2][6] + 82 * pa[2] * pw[0][2] * u[1] * pw[2][6] +
	  28 * pa[2] * pw[1][3] * pw[2][6] + 18 * pa[2] * u[1] * pw[2][8],
	 -3 * pr[8] * pw[1][2] * u[2] + 17 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] + 8 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] - 22 * pa[2] * pw[0][6] * pw[1][2] * u[2] +
	  8 * a * pr[5] * pw[1][4] * u[2] + 10 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] - 51 * pa[2] * pw[0][4] * pw[1][4] * u[2] + 2 * pa[3] * r * pw[1][6] * u[2] -
	  36 * pa[2] * pw[0][2] * pw[1][6] * u[2] - 7 * pa[2] * pw[1][8] * u[2] + 11 * a * pr[5] * pw[1][2] * pw[2][3] + 18 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] -
	  65 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] + 6 * pa[3] * r * pw[1][4] * pw[2][3] - 91 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] -
	  26 * pa[2] * pw[1][6] * pw[2][3] + 4 * pa[3] * r * pw[1][2] * pw[2][5] - 55 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] -
	  31 * pa[2] * pw[1][4] * pw[2][5] - 12 * pa[2] * pw[1][2] * pw[2][7],
	  16 * a * pr[7] * u[0] * u[1] * u[2] - 6 * pr[8] * u[0] * u[1] * u[2] + 4 * pa[3] * r * pw[0][5] * u[1] * u[2] - 14 * pa[2] * pw[0][7] * u[1] * u[2] + 8 * pa[3] * r * pw[0][3] * pw[1][3] * u[2] -
	  42 * pa[2] * pw[0][5] * pw[1][3] * u[2] + 4 * pa[3] * r * u[0] * pw[1][5] * u[2] - 42 * pa[2] * pw[0][3] * pw[1][5] * u[2] - 14 * pa[2] * u[0] * pw[1][7] * u[2] +
	  4 * pa[3] * r * pw[0][3] * u[1] * pw[2][3] - 36 * pa[2] * pw[0][5] * u[1] * pw[2][3] + 8 * pa[3] * r * u[0] * pw[1][3] * pw[2][3] -
	  78 * pa[2] * pw[0][3] * pw[1][3] * pw[2][3] - 42 * pa[2] * u[0] * pw[1][5] * pw[2][3] + 4 * pa[3] * r * u[0] * u[1] * pw[2][5] - 36 * pa[2] * pw[0][3] * u[1] * pw[2][5] -
	  42 * pa[2] * u[0] * pw[1][3] * pw[2][5] - 14 * pa[2] * u[0] * u[1] * pw[2][7],
	 -3 * pr[8] * pw[1][2] * u[2] + 17 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] + 8 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] - 22 * pa[2] * pw[0][6] * pw[1][2] * u[2] +
	  8 * a * pr[5] * pw[1][4] * u[2] + 10 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] - 51 * pa[2] * pw[0][4] * pw[1][4] * u[2] + 2 * pa[3] * r * pw[1][6] * u[2] -
	  36 * pa[2] * pw[0][2] * pw[1][6] * u[2] - 7 * pa[2] * pw[1][8] * u[2] + 11 * a * pr[5] * pw[1][2] * pw[2][3] + 18 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] -
	  65 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] + 6 * pa[3] * r * pw[1][4] * pw[2][3] - 91 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] -
	  26 * pa[2] * pw[1][6] * pw[2][3] + 4 * pa[3] * r * pw[1][2] * pw[2][5] - 55 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] -
	  31 * pa[2] * pw[1][4] * pw[2][5] - 12 * pa[2] * pw[1][2] * pw[2][7],
	 4 * pr[8] * pw[0][2] * u[1] - 16 * a * pr[5] * pw[0][4] * u[1] - 8 * pa[3] * r * pw[0][6] * u[1] + 20 * pa[2] * pw[0][8] * u[1] + 4 * pr[8] * pw[1][3] -
	  28 * a * pr[5] * pw[0][2] * pw[1][3] - 20 * pa[3] * r * pw[0][4] * pw[1][3] + 72 * pa[2] * pw[0][6] * pw[1][3] - 12 * a * pr[5] * pw[1][5] -
	  16 * pa[3] * r * pw[0][2] * pw[1][5] + 96 * pa[2] * pw[0][4] * pw[1][5] - 4 * pa[3] * r * pw[1][7] + 56 * pa[2] * pw[0][2] * pw[1][7] +
	  12 * pa[2] * pw[1][9] - 5 * pr[8] * u[1] * pw[2][2] + 5 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] - 8 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] +
	  36 * pa[2] * pw[0][6] * u[1] * pw[2][2] - 14 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] + 99 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] -
	  6 * pa[3] * r * pw[1][5] * pw[2][2] + 90 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] + 27 * pa[2] * pw[1][7] * pw[2][2] + 9 * a * pr[5] * u[1] * pw[2][4] -
	  2 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] + 11 * pa[2] * pw[0][4] * u[1] * pw[2][4] - 2 * pa[3] * r * pw[1][3] * pw[2][4] +
	  25 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] + 14 * pa[2] * pw[1][5] * pw[2][4] - 9 * pa[2] * pw[0][2] * u[1] * pw[2][6] - 5 * pa[2] * pw[1][3] * pw[2][6] -
	  4 * pa[2] * u[1] * pw[2][8],
      -5 * pr[8] * pw[0][2] * u[2] + 9 * a * pr[5] * pw[0][4] * u[2] - 4 * pa[2] * pw[0][8] * u[2] + 4 * pr[8] * pw[1][2] * u[2] +
	  5 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] - 2 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] - 9 * pa[2] * pw[0][6] * pw[1][2] * u[2] - 16 * a * pr[5] * pw[1][4] * u[2] -
	  8 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] + 11 * pa[2] * pw[0][4] * pw[1][4] * u[2] - 8 * pa[3] * r * pw[1][6] * u[2] + 36 * pa[2] * pw[0][2] * pw[1][6] * u[2] +
	  20 * pa[2] * pw[1][8] * u[2] + 4 * pr[8] * pw[2][3] - 2 * pa[3] * r * pw[0][4] * pw[2][3] - 5 * pa[2] * pw[0][6] * pw[2][3] -
	  28 * a * pr[5] * pw[1][2] * pw[2][3] - 14 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] + 25 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] -
	  20 * pa[3] * r * pw[1][4] * pw[2][3] + 99 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] + 72 * pa[2] * pw[1][6] * pw[2][3] - 12 * a * pr[5] * pw[2][5] -
	  6 * pa[3] * r * pw[0][2] * pw[2][5] + 14 * pa[2] * pw[0][4] * pw[2][5] - 16 * pa[3] * r * pw[1][2] * pw[2][5] +
	  90 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] + 96 * pa[2] * pw[1][4] * pw[2][5] - 4 * pa[3] * r * pw[2][7] + 27 * pa[2] * pw[0][2] * pw[2][7] +
	  56 * pa[2] * pw[1][2] * pw[2][7] + 12 * pa[2] * pw[2][9],
	 16 * a * pr[7] * u[0] * u[1] * u[2] - 6 * pr[8] * u[0] * u[1] * u[2] + 4 * pa[3] * r * pw[0][5] * u[1] * u[2] - 14 * pa[2] * pw[0][7] * u[1] * u[2] + 4 * pa[3] * r * pw[0][3] * pw[1][3] * u[2] -
	  36 * pa[2] * pw[0][5] * pw[1][3] * u[2] + 4 * pa[3] * r * u[0] * pw[1][5] * u[2] - 36 * pa[2] * pw[0][3] * pw[1][5] * u[2] - 14 * pa[2] * u[0] * pw[1][7] * u[2] +
	  8 * pa[3] * r * pw[0][3] * u[1] * pw[2][3] - 42 * pa[2] * pw[0][5] * u[1] * pw[2][3] + 8 * pa[3] * r * u[0] * pw[1][3] * pw[2][3] -
	  78 * pa[2] * pw[0][3] * pw[1][3] * pw[2][3] - 42 * pa[2] * u[0] * pw[1][5] * pw[2][3] + 4 * pa[3] * r * u[0] * u[1] * pw[2][5] - 42 * pa[2] * pw[0][3] * u[1] * pw[2][5] -
	  42 * pa[2] * u[0] * pw[1][3] * pw[2][5] - 14 * pa[2] * u[0] * u[1] * pw[2][7],
	 -3 * pr[8] * u[0] * pw[2][2] + 11 * a * pr[5] * pw[0][3] * pw[2][2] + 4 * pa[3] * r * pw[0][5] * pw[2][2] - 12 * pa[2] * pw[0][7] * pw[2][2] +
	  17 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] + 18 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] - 55 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] +
	  8 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] - 65 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] - 22 * pa[2] * u[0] * pw[1][6] * pw[2][2] + 8 * a * pr[5] * u[0] * pw[2][4] +
	  6 * pa[3] * r * pw[0][3] * pw[2][4] - 31 * pa[2] * pw[0][5] * pw[2][4] + 10 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] -
	  91 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] - 51 * pa[2] * u[0] * pw[1][4] * pw[2][4] + 2 * pa[3] * r * u[0] * pw[2][6] - 26 * pa[2] * pw[0][3] * pw[2][6] -
	  36 * pa[2] * u[0] * pw[1][2] * pw[2][6] - 7 * pa[2] * u[0] * pw[2][8],
	  16 * a * pr[7] * u[0] * u[1] * u[2] - 6 * pr[8] * u[0] * u[1] * u[2] + 4 * pa[3] * r * pw[0][5] * u[1] * u[2] - 14 * pa[2] * pw[0][7] * u[1] * u[2] + 4 * pa[3] * r * pw[0][3] * pw[1][3] * u[2] -
	  36 * pa[2] * pw[0][5] * pw[1][3] * u[2] + 4 * pa[3] * r * u[0] * pw[1][5] * u[2] - 36 * pa[2] * pw[0][3] * pw[1][5] * u[2] - 14 * pa[2] * u[0] * pw[1][7] * u[2] +
	  8 * pa[3] * r * pw[0][3] * u[1] * pw[2][3] - 42 * pa[2] * pw[0][5] * u[1] * pw[2][3] + 8 * pa[3] * r * u[0] * pw[1][3] * pw[2][3] -
	  78 * pa[2] * pw[0][3] * pw[1][3] * pw[2][3] - 42 * pa[2] * u[0] * pw[1][5] * pw[2][3] + 4 * pa[3] * r * u[0] * u[1] * pw[2][5] - 42 * pa[2] * pw[0][3] * u[1] * pw[2][5] -
	  42 * pa[2] * u[0] * pw[1][3] * pw[2][5] - 14 * pa[2] * u[0] * u[1] * pw[2][7],
	 4 * pr[8] * pw[0][2] * u[2] - 16 * a * pr[5] * pw[0][4] * u[2] - 8 * pa[3] * r * pw[0][6] * u[2] + 20 * pa[2] * pw[0][8] * u[2] - 5 * pr[8] * pw[1][2] * u[2] +
	  5 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] - 8 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] + 36 * pa[2] * pw[0][6] * pw[1][2] * u[2] + 9 * a * pr[5] * pw[1][4] * u[2] -
	  2 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] + 11 * pa[2] * pw[0][4] * pw[1][4] * u[2] - 9 * pa[2] * pw[0][2] * pw[1][6] * u[2] - 4 * pa[2] * pw[1][8] * u[2] +
	  4 * pr[8] * pw[2][3] - 28 * a * pr[5] * pw[0][2] * pw[2][3] - 20 * pa[3] * r * pw[0][4] * pw[2][3] + 72 * pa[2] * pw[0][6] * pw[2][3] -
	  14 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] + 99 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] - 2 * pa[3] * r * pw[1][4] * pw[2][3] +
	  25 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] - 5 * pa[2] * pw[1][6] * pw[2][3] - 12 * a * pr[5] * pw[2][5] - 16 * pa[3] * r * pw[0][2] * pw[2][5] +
	  96 * pa[2] * pw[0][4] * pw[2][5] - 6 * pa[3] * r * pw[1][2] * pw[2][5] + 90 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] +
	  14 * pa[2] * pw[1][4] * pw[2][5] - 4 * pa[3] * r * pw[2][7] + 56 * pa[2] * pw[0][2] * pw[2][7] + 27 * pa[2] * pw[1][2] * pw[2][7] +
	  12 * pa[2] * pw[2][9],-3 * pr[8] * u[1] * pw[2][2] + 17 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] + 8 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] -
	  22 * pa[2] * pw[0][6] * u[1] * pw[2][2] + 11 * a * pr[5] * pw[1][3] * pw[2][2] + 18 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] -
	  65 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] + 4 * pa[3] * r * pw[1][5] * pw[2][2] - 55 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] -
	  12 * pa[2] * pw[1][7] * pw[2][2] + 8 * a * pr[5] * u[1] * pw[2][4] + 10 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] - 51 * pa[2] * pw[0][4] * u[1] * pw[2][4] +
	  6 * pa[3] * r * pw[1][3] * pw[2][4] - 91 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] - 31 * pa[2] * pw[1][5] * pw[2][4] + 2 * pa[3] * r * u[1] * pw[2][6] -
	  36 * pa[2] * pw[0][2] * u[1] * pw[2][6] - 26 * pa[2] * pw[1][3] * pw[2][6] - 7 * pa[2] * u[1] * pw[2][8],
	  -3 * pr[8] * u[0] * pw[2][2] + 11 * a * pr[5] * pw[0][3] * pw[2][2] + 4 * pa[3] * r * pw[0][5] * pw[2][2] - 12 * pa[2] * pw[0][7] * pw[2][2] +
	  17 * a * pr[5] * u[0] * pw[1][2] * pw[2][2] + 18 * pa[3] * r * pw[0][3] * pw[1][2] * pw[2][2] - 55 * pa[2] * pw[0][5] * pw[1][2] * pw[2][2] +
	  8 * pa[3] * r * u[0] * pw[1][4] * pw[2][2] - 65 * pa[2] * pw[0][3] * pw[1][4] * pw[2][2] - 22 * pa[2] * u[0] * pw[1][6] * pw[2][2] + 8 * a * pr[5] * u[0] * pw[2][4] +
	  6 * pa[3] * r * pw[0][3] * pw[2][4] - 31 * pa[2] * pw[0][5] * pw[2][4] + 10 * pa[3] * r * u[0] * pw[1][2] * pw[2][4] -
	  91 * pa[2] * pw[0][3] * pw[1][2] * pw[2][4] - 51 * pa[2] * u[0] * pw[1][4] * pw[2][4] + 2 * pa[3] * r * u[0] * pw[2][6] - 26 * pa[2] * pw[0][3] * pw[2][6] -
	  36 * pa[2] * u[0] * pw[1][2] * pw[2][6] - 7 * pa[2] * u[0] * pw[2][8],
	 -3 * pr[8] * u[1] * pw[2][2] + 17 * a * pr[5] * pw[0][2] * u[1] * pw[2][2] + 8 * pa[3] * r * pw[0][4] * u[1] * pw[2][2] - 22 * pa[2] * pw[0][6] * u[1] * pw[2][2] +
	  11 * a * pr[5] * pw[1][3] * pw[2][2] + 18 * pa[3] * r * pw[0][2] * pw[1][3] * pw[2][2] - 65 * pa[2] * pw[0][4] * pw[1][3] * pw[2][2] +
	  4 * pa[3] * r * pw[1][5] * pw[2][2] - 55 * pa[2] * pw[0][2] * pw[1][5] * pw[2][2] - 12 * pa[2] * pw[1][7] * pw[2][2] + 8 * a * pr[5] * u[1] * pw[2][4] +
	  10 * pa[3] * r * pw[0][2] * u[1] * pw[2][4] - 51 * pa[2] * pw[0][4] * u[1] * pw[2][4] + 6 * pa[3] * r * pw[1][3] * pw[2][4] -
	  91 * pa[2] * pw[0][2] * pw[1][3] * pw[2][4] - 31 * pa[2] * pw[1][5] * pw[2][4] + 2 * pa[3] * r * u[1] * pw[2][6] - 36 * pa[2] * pw[0][2] * u[1] * pw[2][6] -
	  26 * pa[2] * pw[1][3] * pw[2][6] - 7 * pa[2] * u[1] * pw[2][8],
	 2 * pr[8] * pw[0][2] * u[2] - 12 * a * pr[5] * pw[0][4] * u[2] - 8 * pa[3] * r * pw[0][6] * u[2] + 18 * pa[2] * pw[0][8] * u[2] + 2 * pr[8] * pw[1][2] * u[2] -
	  24 * a * pr[5] * pw[0][2] * pw[1][2] * u[2] - 34 * pa[3] * r * pw[0][4] * pw[1][2] * u[2] + 82 * pa[2] * pw[0][6] * pw[1][2] * u[2] - 12 * a * pr[5] * pw[1][4] * u[2] -
	  34 * pa[3] * r * pw[0][2] * pw[1][4] * u[2] + 128 * pa[2] * pw[0][4] * pw[1][4] * u[2] - 8 * pa[3] * r * pw[1][6] * u[2] + 82 * pa[2] * pw[0][2] * pw[1][6] * u[2] +
	  18 * pa[2] * pw[1][8] * u[2] - pr[8] * pw[2][3] + 5 * a * pr[5] * pw[0][2] * pw[2][3] - 6 * pa[3] * r * pw[0][4] * pw[2][3] +
	  28 * pa[2] * pw[0][6] * pw[2][3] + 5 * a * pr[5] * pw[1][2] * pw[2][3] - 4 * pa[3] * r * pw[0][2] * pw[1][2] * pw[2][3] +
	  77 * pa[2] * pw[0][4] * pw[1][2] * pw[2][3] - 6 * pa[3] * r * pw[1][4] * pw[2][3] + 77 * pa[2] * pw[0][2] * pw[1][4] * pw[2][3] +
	  28 * pa[2] * pw[1][6] * pw[2][3] + 2 * a * pr[5] * pw[2][5] + 2 * pa[3] * r * pw[0][2] * pw[2][5] + pa[2] * pw[0][4] * pw[2][5] +
	  2 * pa[3] * r * pw[1][2] * pw[2][5] - 15 * pa[2] * pw[0][2] * pw[1][2] * pw[2][5] + pa[2] * pw[1][4] * pw[2][5] - 10 * pa[2] * pw[0][2] * pw[2][7] -
	  10 * pa[2] * pw[1][2] * pw[2][7] - pa[2] * pw[2][9]
	});
	float fract = 1.f / a * 2 * pr[3] * (-a + r) * (-2 * a * pr[8] + pr[9] + pa[2] * r * (pw[0][6] + pw[1][6] + pw[2][6] - 3 * pw[0][2] * pw[1][2] * pw[2][2]) +
		pa[3] * (3 * pw[0][4] * (pw[1][2] + pw[2][2]) + 3 * pw[1][2] * pw[2][2] * (pw[1][2] + pw[2][2]) +
			pw[0][2] * (3 * pw[1][4] + 14 * pw[1][2] * pw[2][2] + 3 * pw[2][4])));
	for (int i = 0; i < ret.size; i++)
		ret.data[i] /= fract;
	return ret;
}
