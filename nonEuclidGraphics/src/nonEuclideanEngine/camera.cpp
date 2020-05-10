#include <nonEuclideanEngine/camera.h>

using namespace nonEuc;

Camera::Camera(vecf3 position, nonEuc::World& _world) : world {_world}
{
	paraPos = position;
	Rotation = matf3::Identity();
	T = (SchmidtOrthogonalize(world.metric(paraPos)) * Rotation).transpose();
}

Camera::~Camera()
{

}

void nonEuc::Camera::ResetCamera(vecf3 position, float yaw, float pitch)
{
	paraPos = position;
	float y = toRad(yaw);
	float p = toRad(pitch);

	Rotation = matf3{
		sin(y),		-sin(p) * cos(y),	cos(y) * cos(p),
		0,			cos(p),			sin(p),
		-cos(y),	-sin(y) * sin(p),	sin(y) * cos(p),
	};
	T = (SchmidtOrthogonalize(world.metric(paraPos)) * Rotation).transpose();
}

void Camera::UpdateDirection(float dyaw, float dpitch)
{
	float y = toRad(dyaw);
	float p = toRad(dpitch);
	
	Rotation = matf3{
		cos(y),		-sin(y)*sin(p),	sin(y)*cos(p),
		0,			cos(p),			sin(p),
		-sin(y),	-cos(y)*sin(p),	cos(y)*cos(p),
	} * Rotation;
	T = (SchmidtOrthogonalize(world.metric(paraPos)) * Rotation).transpose();
}

void Camera::UpdatePosition(vecf3 du)
{
	matf3 G1 = world.metric(paraPos);
	matf3 G2 = world.metric(paraPos + du);
	matf3 S1 = SchmidtOrthogonalize(G1);
	matf3 S2 = SchmidtOrthogonalize(G2);

	Rotation = Translate(S1, S2, world.gamma(paraPos), du, Rotation);
	std::cout << "Rotation" << std::endl<< Rotation;
	paraPos = paraPos + du;
	T = (S2 * Rotation).transpose();
}

matf4 Camera::GetView()
{
	// Vp = TG(p - camera.p)
	matf3 G = world.metric(paraPos);

	matf4 m1{
		1.0f, 0.0f, 0.0f, -paraPos[0],
		0.0f, 1.0f, 0.0f, -paraPos[1],
		0.0f, 0.0f, 1.0f, -paraPos[2],
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	matf3 tmp = T * G ;
	matf4 m2{
		tmp(0, 0), tmp(0, 1), tmp(0, 2), 0.0f,
		tmp(1, 0), tmp(1, 1), tmp(1, 2), 0.0f,
		tmp(2, 0), tmp(2, 1), tmp(2, 2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return m2 * m1;
}

