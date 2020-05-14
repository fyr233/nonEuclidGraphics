
#include <nonEuclideanEngine/GlobalRenderer/RayTracer.h>
#include <core/geometry.h>
#include <FastBVH.h>

using namespace cgcore;

void nonEuc::RayTracer::BuildBVH()
{
	//triangles = world.GetTriangles();			//TODO

	FastBVH::DefaultBuilder<float> builder;

	auto boxConverter = [](const Triangle& t) {
		FastBVH::Vector3<float> minbound, maxbound;
		minbound[0] = std::min(std::min(t.pos[0][0], t.pos[1][0]), t.pos[2][0]);
		minbound[1] = std::min(std::min(t.pos[0][1], t.pos[1][1]), t.pos[2][1]);
		minbound[2] = std::min(std::min(t.pos[0][2], t.pos[1][2]), t.pos[2][2]);

		minbound[0] = std::max(std::max(t.pos[0][0], t.pos[1][0]), t.pos[2][0]);
		minbound[1] = std::max(std::max(t.pos[0][1], t.pos[1][1]), t.pos[2][1]);
		minbound[2] = std::max(std::max(t.pos[0][2], t.pos[1][2]), t.pos[2][2]);

		return FastBVH::BBox<float>(minbound, maxbound);
	};
	bvh = builder(triangles, boxConverter);
}

rgbf nonEuc::RayTracer::shader(FastBVH::Ray<float> ray, float distance, int times)
{
	Intersector intersector;
	FastBVH::Traverser<float, Triangle, decltype(intersector)> traverser(bvh, intersector);
	
	return { 0.f,0.f,0.f };
}
