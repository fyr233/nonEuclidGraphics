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
	
	vecf3 front = vecf3{ cos(y) * cos(p), sin(p), sin(y) * cos(p) };
	vecf3 right = vecf3{ -sin(y), 0.0f, cos(y) };

	Front = S.dot(front);
	Right = S.dot(right);
	Up = S.dot(vecf3::cross(right, front).normalize());
}

void Camera::UpdatePosition(const tensorf333& gamma, vecf3 du, matf3 G)
{
	// 根据位置变化调整相应的三个坐标轴向量
	paraPos = paraPos + du;
	Front = Translate(gamma, du, Front);
	Right = Translate(gamma, du, Right);
	Up = Translate(gamma, du, Up);

	Front = Front / sqrt(G.dot_s(Front, Front));
	Right = Right / sqrt(G.dot_s(Right, Right));
	Up = Up / sqrt(G.dot_s(Up, Up));
}

