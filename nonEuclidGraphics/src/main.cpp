

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
    std::shared_ptr<nonEuc::World> pworld = std::make_shared<nonEuc::World>();

    pworld->SetWorldExample<nonEuc::WorldExample::OneRecursive>();
    std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
    tex->Load("../data/test.png");
    tex->SetTextureImage();
    // 添加网格

    //pworld->AddObj(std::make_shared<Mesh>("../data/ball.obj", tex), {0.f,0.f,0.f }, { 0.05f, 0.05f, 0.05f }, matf3::Identity());
    for(int i = -3; i <=3; i+=2)
        for (int j = -3; j <= 3; j += 2)
            for (int k = -3; k <= 3; k += 2)
                pworld->AddObj(std::make_shared<Mesh>("../data/ball.obj", tex, Material::MaterialType::DEFAULT), { (float)i / 2.f, (float)j / 2.f, (float)k / 2.f + 10.f}, { 0.05f, 0.05f, 0.05f }, vecf3{0.f, 0.f, 0.f});
    pworld->AddAreaLight({1.f, 1.f, 10.f}, {0.3f, 0.f, 0.f}, {0.f, 0.f, 0.3f}, 30.f, { 1.f,1.f,1.f });
    
    //pworld->AddObj(std::make_shared<Mesh>("../data/ball.obj", tex, Material::BRASS), { 0.5f,0.f,0.f }, { 0.1f, 0.1f, 0.1f }, matf3::Identity());
    engine.SetWorld(pworld);
    engine.Loop();
    engine.Clear();


    return 0;
}
