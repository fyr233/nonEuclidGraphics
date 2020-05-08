

#include <iostream>

#include <nonEuclideanEngine/Engine.h>

#include <core/vec.h>
#include <core/mat.h>

#include <core/transform.h>

using namespace cgcore;

int main()
{
    vecf3 v1 = { 0.1f, 2.f, 0.3f };
    vecf3 v2 = { 0.5, 0.3f, 0.4f };
    vec<float, 6> v3(v1, v2);
    printf("%f\n",vecf3::dot(v1, v2));

    mat<float, 3, 3> mat1 = { 4.f, 2.f, 3.f, 4.f, 1.f, 6.f, 7.f, 8.f, 9.f};

    std::cout << mat1.transpose() << std::endl;
    std::cout << vecf3::cross(v1, v2) << std::endl;
    std::cout << mat1.inverse() << std::endl;
    std::cout << matf3::dot(mat1.inverse(), mat1) << std::endl;
    std::cout << "Hello World!\n";
    matf3 g = matf3::dot(mat1.transpose(), mat1);
    std::cout << g << std::endl;
    std::cout << SchmidtOrthogonalize(g) << std::endl;

    nonEuc::Engine engine;      // 这个最好放在最开始
    nonEuc::World world;

    // 添加网格
    world.AddObj(std::make_shared<Mesh>("../data/balls.obj"), {0.f, 0.f, 0.f}, matf3::Identity());

    engine.SetWorld(&world);
    engine.Loop();
    engine.Clear();


    return 0;
}
