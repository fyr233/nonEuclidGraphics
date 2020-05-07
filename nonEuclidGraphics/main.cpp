// nonEuclidGraphics.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>

#include <nonEuclideanEngine/Engine.h>

#include <core/vec.h>
#include <core/mat.h>

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

    nonEuc::Engine engine;      // 这个最好放在最开始
    nonEuc::World world;

    // 添加网格
    world.AddMesh(new Mesh("cube.obj"));
    //std::cout << sizeof(vecf3) << " " << sizeof(float) << std::endl;

    engine.SetWorld(&world);
    engine.Loop();
    engine.Clear();


    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
