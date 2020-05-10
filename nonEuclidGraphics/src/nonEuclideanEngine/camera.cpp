#include <nonEuclideanEngine/camera.h>

Camera::Camera()
{

}

Camera::Camera(vecf3 position, matf3 S)
{
	UpdateDirection(position, S);
}

Camera::~Camera()
{

}

void Camera::UpdateDirection(vecf3 position, matf3 S)
{
	paraPos = position;
	float y = toRad(yaw);
	float p = toRad(pitch);
	
	Rotation = matf3{
		sin(y),		-sin(p)*cos(y),	cos(y)*cos(p),
		0,			cos(p),			sin(p),
		-cos(y),	-sin(y)*sin(p),	sin(y)*cos(p),
	};

	Position = S * Rotation;
}

void Camera::UpdatePosition(const tensorf333& gamma, vecf3 du, matf3& G, matf3& S1, matf3& S2)
{
	
	paraPos = paraPos + du;
	Rotation = Translate(S1, S2, gamma, du, Rotation);
	Position = S2 * Rotation;
}

matf4 Camera::GetView(matf3 G)
{
	// Vp = TG(p - camera.p)
	matf4 m1{
		1.0f, 0.0f, 0.0f, -paraPos[0],
		0.0f, 1.0f, 0.0f, -paraPos[1],
		0.0f, 0.0f, 1.0f, -paraPos[2],
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	matf3 tmp = Position * G;
	matf4 m2{
		tmp(0, 0), tmp(1, 0), tmp(2, 0), 0.0f,
		tmp(0, 1), tmp(1, 1), tmp(2, 1), 0.0f,
		tmp(0, 2), tmp(1, 2), tmp(2, 2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return m2 * m1;
}

