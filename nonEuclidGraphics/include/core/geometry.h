#pragma once


//#include <FastBVH.h>
#include <core/primitive.h>
#include <core/mat.h>

template<typename T>
static const float PI = static_cast<T>(3.1415926535897932);

// 下面定义一个三角形求交
/*
namespace cgcore
{
    class Intersector final {
    public:
        FastBVH::Intersection<float, Triangle> operator () (const Triangle& tri, const FastBVH::Ray<float>& ray) noexcept {

            matf3 E1E2d = {
                tri.pos[1][0], tri.pos[2][0], -ray.d[0],
                tri.pos[1][1], tri.pos[2][2], -ray.d[1],
                tri.pos[1][2], tri.pos[2][2], -ray.d[2]
            };
            vecf3 abt = E1E2d.inverse().dot(vecf3{ ray.o[0], ray.o[1], ray.o[2] }-tri.pos[0]);
            if(isnan(abt[2]) || abt[2]<0.f || abt[2]>1.f){
                return FastBVH::Intersection<float, Triangle> { };
            }

            float u = abt[0] * tri.uv[1][0] + abt[1] * tri.uv[2][0] + (1.f - abt[0] - abt[1]) * tri.uv[0][0];
            float v = abt[0] * tri.uv[1][1] + abt[1] * tri.uv[2][1] + (1.f - abt[0] - abt[1]) * tri.uv[0][1];

            return FastBVH::Intersection<float, Triangle> {
                abt[2],
                &tri,
                {u, v}
            };
        }
    };
}
*/