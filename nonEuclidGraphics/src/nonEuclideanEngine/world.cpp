
#include <nonEuclideanEngine/world.h>

using namespace nonEuc;

World::World()
{
	
}

void World::AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, vecf3 scale, matf3 rotation)
{
	objectPtrs.push_back(std::make_shared<Object>(this, newMesh, center, scale, rotation));
}