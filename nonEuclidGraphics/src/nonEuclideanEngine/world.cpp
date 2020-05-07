
#include <nonEuclideanEngine/world.h>

using namespace nonEuc;

void World::AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, matf3 rotation)
{
	// TODO:在这里要更改newMesh的参数坐标
	objectPtrs.push_back(std::make_shared<Obj>(newMesh, center, rotation));
}