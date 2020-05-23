

#include <iostream>

#include <nonEuclideanEngine/Engine.h>

#include <core/vec.h>
#include <core/mat.h>
#include <core/rgb.h>

#include <core/transform.h>
#include <nonEuclideanEngine/WorldExample.h>

#include <nonEuclideanEngine/GlobalRenderer/RayTracer.h>

using namespace cgcore;

int main()
{

    nonEuc::Engine engine;      // 这个最好放在最开始
    //std::shared_ptr<nonEuc::World> pworld = std::make_shared<nonEuc::World>();
    std::shared_ptr<nonEuc::World> pworld = std::make_shared<nonEuc::World>(0);

    engine.SetWorld(pworld);
    engine.Loop();
    engine.Clear();


    return 0;
}
