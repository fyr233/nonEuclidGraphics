
#include <nonEuclideanEngine/GlobalRenderer/RayTracer.h>
#include <core/geometry.h>
#include <FastBVH.h>

using namespace cgcore;

using Vec3 = FastBVH::Vector3<float>;

float dot(const Vec3& v1, const matf3& mat, const Vec3& v2)
{
	float ret = 0.f;
	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			ret += v1[i] * mat(i, j) * v2[j];
	return ret;
}

nonEuc::RayTracer::RayTracer(World* _world) :world{_world}
{
	
}

void nonEuc::RayTracer::SetWorld(World* _world)
{
	world = _world;
}

void nonEuc::RayTracer::SetParameter(float _distance_limit, float _decay_distance, rgbf _background_color, float _dt)
{
	distance_limit = _distance_limit;
	decay_distance = _decay_distance;
	background_color = _background_color;
	dt = _dt;
}

void nonEuc::RayTracer::BuildBVH()
{
	triangles = world->GetTriangles();			//TODO

	FastBVH::DefaultBuilder<float> builder;

	auto boxConverter = [](const Triangle& t) {
		FastBVH::Vector3<float> minbound, maxbound;
		minbound[0] = t.pos[0][0] + std::min(std::min(0.f, t.pos[1][0]), t.pos[2][0]);
		minbound[1] = t.pos[0][1] + std::min(std::min(0.f, t.pos[1][1]), t.pos[2][1]);
		minbound[2] = t.pos[0][2] + std::min(std::min(0.f, t.pos[1][2]), t.pos[2][2]);

		maxbound[0] = t.pos[0][0] + std::max(std::max(0.f, t.pos[1][0]), t.pos[2][0]);
		maxbound[1] = t.pos[0][1] + std::max(std::max(0.f, t.pos[1][1]), t.pos[2][1]);
		maxbound[2] = t.pos[0][2] + std::max(std::max(0.f, t.pos[1][2]), t.pos[2][2]);

		return FastBVH::BBox<float>(minbound, maxbound);
	};
	delete bvh;
	bvh = new FastBVH::BVH(builder(triangles, boxConverter));
}

cv::Mat nonEuc::RayTracer::RenderTracing(float fov, float aspect, int width)
{

	vecf3 o = world->camera.paraPos;		//渲染的光线出发点
	vecf3 camerax = { world->camera.T(0, 0) ,world->camera.T(0, 1), world->camera.T(0, 2) };
	vecf3 cameray = { world->camera.T(1, 0) ,world->camera.T(1, 1), world->camera.T(1, 2) };
	vecf3 cameraz = { world->camera.T(2, 0) ,world->camera.T(2, 1), world->camera.T(2, 2) };

	int height = (int)(width / aspect);
	float vertical = tan(fov / 2.f);
	int midwid = width / 2, midhgh = height / 2;
	float delta = vertical / (height / 2);

	rgbf pixel;

	cv::Mat img(height, width, CV_32FC3);

	Intersector intersector;
	FastBVH::Traverser<float, Triangle, Intersector> traverser(*bvh, intersector);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			vecf3 d = cameraz*(-1.f) + camerax * (delta * (i - midwid)) + cameray*(delta*(midhgh - j));
			d = d / sqrt(world->metric(o).dot_s(d, d));			//Normalize
			pixel = tracer(FastBVH::Ray<float>(Vec3{o[0],o[1],o[2]}, Vec3{ d[0], d[1], d[2]}), traverser);
			img.at<cv::Vec3f>(j, i) = cv::Vec3f((float*)&(pixel*256.f));
			//std::cout << pixel.r << pixel.g << pixel.b<< std::endl;
		}
		std::cout << float(i) / width << std::endl;
	}
	// 暂时保存为图片查看。接下来最好做成显示在 GUI 上。
	cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
	if (cv::imwrite("../data/rst.jpg", img))
		std::cout << "Done" << std::endl;
	else
		std::cout << "FileError" << std::endl;
	return img;
}

rgbf nonEuc::RayTracer::shader(FastBVH::Ray<float> ray, float distance, int times)
{
	//Intersector intersector;
	//FastBVH::Traverser<float, Triangle, decltype(intersector)> traverser(bvh, intersector);
	//TODO
	return { 0.f,0.f,0.f };
}

rgbf nonEuc::RayTracer::tracer(FastBVH::Ray<float> ray, FastBVH::Traverser<float, Triangle, Intersector>& traverser)
{
	float distance = 0.f;
	float* rayo = (float*)&ray.o;

	FastBVH::Intersection<float, Triangle> isect;
	while (true)
	{
		float nrm = sqrt(dot(ray.d, world->metric(vecf3(rayo)), ray.d));
		ray.d = ray.d * (dt / nrm);

		tensorf333 gamma = world->gamma(vecf3(rayo));
		Vec3 dv;
		for (size_t i = 0; i < 3; i++)
		{
			dv[i] = 0.f;
			for (size_t k = 0; k < 3; k++)
				for (size_t l = 0; l < 3; l++)
					dv[i] -= gamma(i, k, l) * ray.d[k] * ray.d[l];
		}
		ray.d = ray.d + dv;

		isect = traverser.traverse(ray);
		if (isect)
		{

			Obj* isectobj = &(*world->objectPtrs[isect.object->obj_id]);
			if (isectobj->obj_type == Obj::ObjType::_AreaLight)
				return ((AreaLight*)isectobj)->RadianceFactor();
			else
				return ((Object*)isectobj)->mesh->AlbedoTexture->Sample({ isect.uv[0], isect.uv[1] });
		}

		ray.o = ray.o + ray.d;
		distance += dt;

		if (distance > distance_limit)
			return background_color;
		
	}
}
