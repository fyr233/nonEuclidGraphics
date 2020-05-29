# nonEuclidGraphics

非欧几何世界中的渲染引擎，有局部渲染和全局渲染两种模式。局部渲染使用OpenGL实现，全局渲染通过求解测地线方程实现。

我们会后续的维护和新的算法的探索。

## 编译

很抱歉我们还没有掌握 CMake，而是直接上传了 Visual Studio 的工程文件。大部分依赖文件都已经包括在了项目中。如果顺利的话，你需要下面几步：

* 推荐 Visual Studio 2019. C++17. 一些语法可能无法在低版本下编译通过。我们上传了 `.sln` 项目解决方案文件，你可以直接打开它，
* 配置 OpenCV，并将OpenCV的`build`目录（例如`D:\Library\opencv\build`）添加到系统变量中，名称为`OpenCV_Dir`，你可以在 Windows 的系统属性->系统环境变量->系统变量添加这个变量。
* 本项目使用了GLFW库，请构建GLFW并将GLFW/bin添加到环境变量，**或**直接将glfw3.dll放在可执行文件所在目录。
* 我们使用OpenMP实现简易的并行加速。要在编译时开启它，需要在Visual Studio 2019的项目->属性->C/C++->语言中，将**OpenMP支持**设置为**是**，将**符合模式**设置为**是**。

## 运行

**请选择 Release 模式编译运行以保证性能。**

（这个项目的GUI基于Dear-Imgui）

在界面中通过 Engine 框内的控件交互完成操作，主界面显示的是**局部渲染**的效果

* Camera 栏，显示摄像机位置；设定 FarPlane 和 NearPlane。它决定了可见的范围。这对于非欧空间中的局部渲染很重要，因为空间的弯曲，局部渲染中远处的物体的呈现是不准确的。我们通过朦胧的效果使远处的物体逐渐消失，它由FarPlane决定。
* Mesh 栏，没有用。我们早期拿它来调试。
* GlobalRender 栏，全局渲染器的控制，使用全局渲染来查看准确的效果。Width 是图像的宽度（决定分辨率），step size是迭代的步长（改变它通常不会有明显的效果变化，但会影响计算的耗时）。Distance和Decay决了视野的大小，如果你希望视野范围更远，可以调大一旦这两个数值。点击 RayTracing 开始即可计算。命令行窗口中会显示进度——它通常需要数秒钟来渲染一张 1080p 的图片。
* Roam ，选中以进入漫游。使用"w前 a做 s后 d右 q上 e下"来移动摄像机，移动鼠标来改变方向。进入漫游模式后，鼠标会被占用，按下m可以退出漫游。
* EditMenu， 选中以打开物体编辑，查看所有的物体。可以进行移动、旋转、缩放、改变材质参数等操作。
* WorldExample，项目中提供了若干预设的非欧空间，妮可
* LoadMesh，加载网格文件到场景中（请小心！我们还没有在这里做异常处理，如果文件读取失败，程序会崩溃）。



## 项目源文件的构成

* `Shader`
  * `vertex.vert`
  * `fragment.frag`

* `app`
  * `AutoCameraController`  
* `core`
  * `gl`
  * `vec`
  * `mat`
  * `tensor`
  * `func`
  * `transform`
  * `rgb`
  * `Mesh`
  * `Obj`
  * `Object`
  * `AreaLight`
  * `Texture2D`
  * `Material`
* `nonEucideanEngine`
  * `RayTracer`
  * `Camera`
  * `Engine`
  * `Shader`
  * `World`
  * `WorldExample`
