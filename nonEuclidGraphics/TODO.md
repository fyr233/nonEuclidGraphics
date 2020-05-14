# 最后的任务——全局渲染

* 依赖库：`_deps/FastBVH.h`，求交的算法。已作调整。
* 算法核心：`nonEuclideanEngine/GlobalRenderer/RayTracer.h, RayTracer.cpp`
  * `RayTracer::RunShading()`
    * 渲染一张色图
  * `RayTracer::RunTracing()`
    * 渲染一张仅呈现几何遮挡关系的图
  * `RayTracer::shade()`
    * 采样追踪一条光线：（在追踪到面光源/衰减到阈值/时停止）
  * `RayTracer::trace()`
    * 追踪一条光线，仅体现几何遮挡关系。（在第一次面相交/衰减到阈值/距离达到极限时停止）
    * 返回的颜色就是纹理颜色
* 相关接口
  * `<core/primitive.h>   struct Triangle`
    * （已完成）
  * `<nonEuclideanEngine/world> World.GetTraingles()`
    * 需要获得世界中 Object 和 AreaLight 的所有的三角形，返回一个`std::vector<Triangle>`
    * 参考 Triangle 定义的各个成员注释。其中normal，是三角面的法线（参数坐标下，其计算类似于AreaLight下的genNormal ），不是顶点的法线。
    * 最重要的是obj_id 和 face_id。对材质等更多访问需要追溯到原模型。
  * `BRDF`
    * 急需。可以参考助教的HW9的stdBRDF。不同之处是，要转换到参数坐标下
    * 可以先迁移过来，再想怎么转换的参数坐标下。
    * 一个物体对应采用一个BRDF材质，如Object->mesh->BRDF，或者Object->BRDF，具体选哪种看怎么样才方便。