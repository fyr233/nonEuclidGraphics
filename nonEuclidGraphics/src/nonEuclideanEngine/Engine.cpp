#include <nonEuclideanEngine/Engine.h>
#include <fstream>
#include <core/geometry.h>
#include <core/transform.h>

using namespace nonEuc;

static void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Engine::Engine()
{
    if (!Init())
    {
        std::cout << "Error:Engine::Init()" << std::endl;
    }
}

Engine::~Engine()
{

}

bool Engine::Init()
{
	// 加载GLFW窗口
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return false;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(scrwidth, scrheight, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return false;
    }

    // Imgui部分
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Load Font

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 初始化着色器
    programID = LoadShaders("../nonEuclidGraphics/include/Shader/vertex.vert", "../nonEuclidGraphics/include/Shader/fragment.frag");
    
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    return true;
}

void Engine::Loop()
{
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        glfwGetFramebufferSize(window, &scrwidth, &scrheight);
        if (Roam_status)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (Roam_status && ImGui::GetIO().KeysDown['M'])
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Roam_status = false;
        }
        if (status_line)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // TODO:处理键盘事件，在允许鼠标移动时能够隐藏鼠标

        // 加载Imgui的窗口
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            // TODO: 创建一个控制台来调整一些参数
            ImGui::Begin("Engine");
            ImGui::Text("Information about the scene.(TODO)");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (ImGui::CollapsingHeader("Objects"))
            {
                for (size_t i = 0; i < current_world->objectPtrs.size(); i++)
                    ImGui::Text("Object%d", i);
            }
            if (ImGui::CollapsingHeader("Camera"))
            {
                ImGui::SliderFloat("FarPlane", &far_plane, 5.f, 20.f);
                ImGui::SliderFloat("NearPlane", &near_plane, 0.001f, 1.0f);
            }
            if (ImGui::CollapsingHeader("Mesh"))
            {
                ImGui::Checkbox("Line", &status_line);
            }
            if (ImGui::CollapsingHeader("Controller"))
            {
                ImGui::Checkbox("Roam", &Roam_status);
                ImGui::SameLine();
                ImGui::Text("(Press M to make the mouse unfocused)");
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO:绘制场景
        if (Roam_status)
            UpdateCamera();
        matf4 perspective = Perspective(PI<float> / 4, scrheight == 0 ? 1.0f : (float)scrwidth / (float)scrheight, near_plane, far_plane);
        
        glUseProgram(programID);

        for (int j = -2; j <= 2; j++)
        {
            matf4 view = current_world->camera.GetView(j);

            int Location = glGetUniformLocation(programID, "V");
            glUniformMatrix4fv(Location, 1, GL_TRUE, view.data);
            Location = glGetUniformLocation(programID, "P");
            glUniformMatrix4fv(Location, 1, GL_TRUE, perspective.data);

            auto LightColor = current_world->light_as_point->color;
            auto LightPos = current_world->light_as_point->getLightPos();
            auto ViewPos = current_world->regularize(current_world->camera.paraPos, j);

            Location = glGetUniformLocation(programID, "lightColor");
            glUniform3f(Location, LightColor.r, LightColor.g, LightColor.b);
            Location = glGetUniformLocation(programID, "lightPos");
            glUniform3f(Location, LightPos[0], LightPos[1], LightPos[2]);
            Location = glGetUniformLocation(programID, "viewPos");
            glUniform3f(Location, ViewPos[0], ViewPos[1], ViewPos[2]);

            for (size_t i = 0; i < current_world->objectPtrs.size(); i++)
                current_world->objectPtrs[i]->Draw(programID);
        }

        // 绘制Imgui的窗口,放在这里可以使其浮于最上方
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
}

void Engine::Clear()
{
    // Cleanup
    glDeleteProgram(programID);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Engine::UpdateCamera()
{
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;
    ImGuiIO & io = ImGui::GetIO();
    Camera & camera = current_world->camera;

    // Get mouse position
    float dyaw = -mouse_speed * io.MouseDelta.x;
    float dpitch = mouse_speed * io.MouseDelta.y;
    current_world->camera.UpdateDirection(dyaw, dpitch);

    // Get keyboard input
    auto keyboardInput = io.KeysDown;
    vecf3 du = vecf3(0.0f);             // 相机的参数坐标变化量
    vecf3 Front = vecf3{ camera.T(2, 0), camera.T(2, 1), camera.T(2, 2) };
    vecf3 Right = vecf3{ camera.T(0, 0), camera.T(0, 1), camera.T(0, 2) };
    vecf3 Up = vecf3{ camera.T(1, 0), camera.T(1, 1), camera.T(1, 2) };

    if (keyboardInput['A'])
        du = Right * (-move_speed) * deltaTime;
    else if (keyboardInput['S'])
        du = Front * (move_speed) * deltaTime;
    else if (keyboardInput['D'])
        du = Right * (move_speed) * deltaTime;
    else if (keyboardInput['W'])
        du = Front * (-move_speed) * deltaTime;
    else if (keyboardInput['Q'])
        du = Up * (move_speed) * deltaTime;
    else if (keyboardInput['E'])
        du = Up * (-move_speed) * deltaTime;

    camera.UpdatePosition(du);
}
