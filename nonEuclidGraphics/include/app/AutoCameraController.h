#pragma once

#include <iostream>
#include <vector>
#include <core/mat.h>
#include <core/vec.h>
#include <nonEuclideanEngine/world.h>
#include <nonEuclideanEngine/GlobalRenderer/RayTracer.h>

#include <string>
#include <fstream>
#include <sstream>

class AutoCameraController
{
	enum OperationType
	{
		UP,DOWN,LEFT,RIGHT,FRONT,BACK,
		ROTATE_UP,ROTATE_DOWN,ROTATE_LEFT,ROTATE_RIGHT
	};

	typedef struct Operation {
		OperationType type;
		float distance;
	}Operation;

public:
	AutoCameraController() {};
	~AutoCameraController() {};

	bool Init(std::string path, std::shared_ptr<nonEuc::World> _pWorld);
	bool Run();

private:
	int steps_per_frame = 100;
	float step_size = 0.01f;

	float distanceLimit = 5.0f;
	float decay = 3.0f;
	rgbf backgroundColor = rgbf{ 0.12f, 0.13f, 0.19f };
	float dt = 0.01f;
	int width = 128;
	int height = 128;

	std::shared_ptr<nonEuc::World> pWorld = nullptr;
	std::vector<Operation> operationQueue;
};