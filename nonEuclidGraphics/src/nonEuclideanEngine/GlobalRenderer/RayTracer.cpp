
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

	rgbf pixel;



	int blockwidth = width / blocksize;			//划分为block 的宽度、高度
	int blockheight = height / blocksize;

	width = blockwidth * blocksize;
	height = blockheight * blocksize;


	int midwid = width / 2, midhgh = height / 2;
	float delta = vertical / (height / 2);

	cv::Mat img(height, width, CV_32FC3);

	Intersector intersector;
	FastBVH::Traverser<float, Triangle, Intersector> traverser(*bvh, intersector);

	for (int i = 0; i < width; i+=blocksize)
	{
#pragma omp parallel for
		for (int j = 0; j < height; j+=blocksize)
		{
			//vecf3 d = cameraz*(-1.f) + camerax * (delta * (i - midwid)) + cameray*(delta*(midhgh - j));
			//d = d / sqrt(world->metric(o).dot_s(d, d));			//Normalize
			//调用Bloack Tracer
			blocktracer(traverser, o, cameraz, camerax, cameray, delta, i, i + blocksize, j, j + blocksize, midwid, midhgh, img);
		}
		std::cout << float(i) / width << std::endl;
	}
	// 暂时保存为图片查看
	cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
	//if (cv::imwrite("../data/rst.jpg", img))
	//	std::cout << "Done" << std::endl;
	//else
	//	std::cout << "FileError" << std::endl;
	return img;
}

rgbf nonEuc::RayTracer::shader(FastBVH::Ray<float> ray, float distance, int times)
{
	//Intersector intersector;
	//FastBVH::Traverser<float, Triangle, decltype(intersector)> traverser(bvh, intersector);
	//TODO
	return { 0.f,0.f,0.f };
}

rgbf nonEuc::RayTracer::tracer(FastBVH::Ray<float> ray, FastBVH::Traverser<float, Triangle, Intersector>& traverser, float distance)
{
	float* rayo = (float*)&ray.o;

	FastBVH::Intersection<float, Triangle> isect;
	while (true)
	{
		vecf3 vrayo((float*)&ray.o);
		world->regularize_ref(vrayo, 0, 0, 0);
		memcpy(&(ray.o), vrayo.data, sizeof(float) * 3);

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
		ray.inv_d = Vec3{ 1.f / ray.d[0],1.f / ray.d[1], 1.f / ray.d[2] };
		isect = traverser.traverse(ray);
		if (isect)
		{
			Obj* isectobj = &(*world->objectPtrs[isect.object->obj_id]);
			if (isectobj->obj_type == Obj::ObjType::_Object)
			{
				float decay = exp(-pow(distance / decay_distance, 1.5f));
				return ((Object*)isectobj)->mesh->AlbedoTexture->Sample({ isect.uv[0], isect.uv[1] }) * decay + background_color * (1 - decay);
			}
				
		}

		ray.o = ray.o + ray.d;
		distance += dt;

		if (distance > distance_limit)
			return background_color;
		
	}
}

void nonEuc::RayTracer::blocktracer(FastBVH::Traverser<float, Triangle, Intersector>& traverser,
	const vecf3& o, const vecf3& cameraz, const vecf3& camerax, const vecf3& cameray,
	float delta,
	int ibegin, int iend, int jbegin, int jend,
	int midwid, int midhgh,
	cv::Mat& img)
{
	//计算四条初始光线原点和方向
	Vec3 ro{o[0], o[1], o[2]};
	vecf3 rd[2][2] = {
		{cameraz * (-1.f) + camerax * (delta * (ibegin - midwid)) + cameray * (delta * (midhgh - jbegin)),
		cameraz * (-1.f) + camerax * (delta * (ibegin - midwid)) + cameray * (delta * (midhgh - jend))},
		{cameraz * (-1.f) + camerax * (delta * (iend - midwid)) + cameray * (delta * (midhgh - jbegin))
		,cameraz * (-1.f) + camerax * (delta * (iend - midwid)) + cameray * (delta * (midhgh - jend)) }
	};

	//四条光线
	FastBVH::Ray<float> rays[2][2]{
		{FastBVH::Ray<float>{ro, Vec3{rd[0][0][0],rd[0][0][1], rd[0][0][2]}},
		FastBVH::Ray<float>{ro, Vec3{rd[0][1][0],rd[0][1][1], rd[0][1][2]}}},
		{FastBVH::Ray<float>{ro, Vec3{rd[1][0][0],rd[1][0][1], rd[1][0][2]}},
		FastBVH::Ray<float>{ro, Vec3{rd[1][1][0],rd[1][1][1], rd[1][1][2]}}}
	};


	float distance = 0.f;
	float* rayos[2][2] = {
		{(float*)&rays[0][0].o, (float*)&rays[0][1].o},
		{(float*)&rays[1][0].o, (float*)&rays[1][1].o}
	};

	FastBVH::Intersection<float, Triangle> isect;

	bool hitted = false;
	while (!hitted)
	{
		vecf3 vrayos[2][2]{ {rayos[0][0] , rayos[0][1]}, {rayos[1][0], rayos[1][1]} };
		for (int a = 0; a < 2; a++)
		{
			for (int b = 0; b < 2; b++)
			{

				vecf3 vrayo(rayos[a][b]);
				FastBVH::Ray<float>& ray = rays[a][b];
				float* rayo = rayos[a][b];

				world->regularize_ref(vrayo, 0, 0, 0);		//规范化光的起点
				memcpy(&(ray.o), vrayo.data, sizeof(float) * 3);

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
				ray.inv_d = Vec3{ 1.f / ray.d[0],1.f / ray.d[1], 1.f / ray.d[2] };
				isect = traverser.traverse(FastBVH::Ray(ray.o, ray.d * 4.f));
				if (isect)
				{
					Obj* isectobj = &(*world->objectPtrs[isect.object->obj_id]);
					if (isectobj->obj_type == Obj::ObjType::_Object)
					{
						hitted = true;
					}
				}

				ray.o = ray.o + ray.d;

				if (distance > distance_limit)		// 超过距离极限，返回背景色
				{
					for (int i = ibegin; i < iend; i++)
						for (int j = jbegin; j < jend; j++)
							img.at<cv::Vec3f>(j, i) = cv::Vec3f((float*)&(background_color * 256.f));
					return;
				}
			}
		}
		distance += dt;
	}
	
	if (hitted)		// 试探光线打到物体，进行双线性插值出中间光线，并追踪
	{
		rgbf pixel;
		for (int i = ibegin; i < iend; i++)
		{
			for (int j = jbegin; j < jend; j++)
			{
				float fract = 1.f / (float)((iend - ibegin) * (jend - jbegin));
				float w[2][2] = {
					{(iend - i) * (jend - j) * fract, (iend - i) * (j - jbegin) * fract},
					{(i - ibegin) * (jend - j) * fract,(i - ibegin) * (j - jbegin) * fract}
				};
				FastBVH::Ray<float> ray{
					rays[0][0].o * w[0][0] + rays[0][1].o * w[0][1] + rays[1][0].o * w[1][0] + rays[1][1].o * w[1][1],
					rays[0][0].d * w[0][0] + rays[0][1].d * w[0][1] + rays[1][0].d * w[1][0] + rays[1][1].d * w[1][1]
				};
				pixel = tracer(ray, traverser, distance);
				img.at<cv::Vec3f>(j, i) = cv::Vec3f((float*)&(pixel * 256.f));
			}
		}
	}
}
