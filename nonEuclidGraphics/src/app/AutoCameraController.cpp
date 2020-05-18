#include <app/AutoCameraController.h>

bool AutoCameraController::Init(std::string path, std::shared_ptr<nonEuc::World> _pWorld)
{
	pWorld = _pWorld;

	std::ifstream objfile(path);
	if (!objfile) {
		std::cout << "ERROR::AutoCameraController::Init:" << std::endl
			<< "\t" << "open file (" << path << ") fail" << std::endl;
		return false;
	}

	std::string line;
	std::getline(objfile, line);
	std::stringstream ss(line);
	ss >> steps_per_frame >> step_size >> dt >> width >> height;

	while (!objfile.eof())
	{
		std::string line;
		std::getline(objfile, line);
		if (line.empty() || line == "\n")
			continue;
		Operation newOp;

		std::stringstream ss(line);
		std::string type;
		ss >> type;
		ss >> newOp.distance;
		switch (type[0])
		{
		case 'U': 
			newOp.type = UP;
			break;
		case 'D':
			newOp.type = DOWN;
			break;
		case 'F':
			newOp.type = FRONT;
			break;
		case 'B':
			newOp.type = BACK;
			break;
		case 'L':
			newOp.type = LEFT;
			break;
		case 'R':
			if (type.size() > 1 && type[1] == 'I')
				newOp.type = RIGHT;
			else switch (type[1])
			{
			case 'L':
				newOp.type = ROTATE_LEFT;
				break;
			case 'R':
				newOp.type = ROTATE_RIGHT;
				break;
			case 'D':
				newOp.type = ROTATE_DOWN;
				break;
			case 'U':
				newOp.type = ROTATE_UP;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		operationQueue.push_back(newOp);
	}

	objfile.close();
	return true;
}

bool AutoCameraController::Run()
{
	std::cout << steps_per_frame << " " << step_size << " " << dt << " " << width << " " << height << std::endl;
	for (int i = 0; i < operationQueue.size(); i++)
		std::cout << operationQueue[i].type << " " << operationQueue[i].distance << std::endl;
	
	float fov = width / (float)height;

	nonEuc::Camera& camera = pWorld->camera;

	nonEuc::RayTracer rayTracer(&(*pWorld));
	rayTracer.SetParameter(distanceLimit, decay, backgroundColor, dt);    //初始渲染参数
	rayTracer.BuildBVH();                                               //生成BVH

	int num = 0;
	int count = 0;
	for (auto op : operationQueue)
	{
		float real_distance = 0.0f;
		while (real_distance < op.distance)
		{
			if (count % steps_per_frame == 0)
			{
				std::ostringstream ss;
				ss << "../data/Result/rst" << num++ << ".jpg";

				cv::Mat image = rayTracer.RenderTracing(PI<float> / 4, fov, width);

				if (cv::imwrite(ss.str(), image))
					std::cout << "Done" << std::endl;
				else
					std::cout << "FileError" << std::endl;
			}
			count++;
			vecf3 Front = vecf3{ camera.T(2, 0), camera.T(2, 1), camera.T(2, 2) };
			vecf3 Right = vecf3{ camera.T(0, 0), camera.T(0, 1), camera.T(0, 2) };
			vecf3 Up = vecf3{ camera.T(1, 0), camera.T(1, 1), camera.T(1, 2) };
			vecf3 du = vecf3(0.0f);
			float dyaw = 0.0f;
			float dpitch = 0.0f;
			switch (op.type)
			{
			case UP:
				du = Up * step_size;
				break;
			case DOWN:
				du = Up * -step_size;
				break;
			case LEFT:
				du = Right * -step_size;
				break;
			case RIGHT:
				du = Right * step_size;
				break;
			case FRONT:
				du = Front * -step_size;
				break;
			case BACK:
				du = Front * step_size;
				break;
			case ROTATE_DOWN:
				dpitch = 10 * step_size;
				break;
			case ROTATE_LEFT:
				dyaw = 10 * step_size;
				break;
			case ROTATE_RIGHT:
				dyaw = -10 * step_size;
				break;
			case ROTATE_UP:
				dpitch = -10 * step_size;
				break;
			default:
				break;
			}
			camera.UpdateDirection(dyaw, dpitch);
			camera.UpdatePosition(du);
			real_distance += du.norm() + abs(dyaw) + abs(dpitch);
		}
	}
	return true;
}