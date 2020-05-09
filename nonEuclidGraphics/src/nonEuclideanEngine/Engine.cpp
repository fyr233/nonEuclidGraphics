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
        if (mouseIO)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (mouseIO && ImGui::GetIO().KeysDown['M'])
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseIO = false;
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

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        {
            // TODO: 创建一个控制台来调整一些参数
            ImGui::Begin("Engine");
            ImGui::Text("Information about the scene.(TODO)");
            //ImGui::SetWindowSize(ImVec2(200, 100));
            if (ImGui::CollapsingHeader("Objects"))
            {
                for (size_t i = 0; i < current_world->objectPtrs.size(); i++)
                    ImGui::Text("Object%d", i);
            }
            if (ImGui::CollapsingHeader("Camera"))
            {
                ImGui::SliderFloat("FarPlane", &far_plane, 10.f, 100.f);
                ImGui::SliderFloat("NearPlane", &near_plane, 0.001f, 1.0f);
            }
            if (ImGui::CollapsingHeader("Mesh"))
            {
                ImGui::Checkbox("Line", &status_line);
            }
            if (ImGui::CollapsingHeader("Controller"))
            {
                ImGui::Checkbox("Mouse", &mouseIO);
                ImGui::SameLine();
                ImGui::Text("(Press M to make the mouse unfocused)");
            }
            
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO:绘制场景
        UpdateCamera();
        matf4 perspective = Perspective(PI<float> / 4, (float)scrwidth / (float)scrheight, near_plane, far_plane);

        glUseProgram(programID);
        int Location = glGetUniformLocation(programID, "cameraPos");
        glUniform3f(Location, current_world->camera.paraPos[0], current_world->camera.paraPos[1], current_world->camera.paraPos[2]);
        Location = glGetUniformLocation(programID, "cameraX");
        glUniform3f(Location, current_world->camera.Right[0], current_world->camera.Right[1], current_world->camera.Right[2]);
        Location = glGetUniformLocation(programID, "cameraY");
        glUniform3f(Location, current_world->camera.Up[0], current_world->camera.Up[1], current_world->camera.Up[2]);
        Location = glGetUniformLocation(programID, "cameraZ");
        glUniform3f(Location, current_world->camera.Front[0], current_world->camera.Front[1], current_world->camera.Front[2]);
        Location = glGetUniformLocation(programID, "G");
        glUniformMatrix3fv(Location, 1, GL_TRUE, matf3::Identity().data);
        Location = glGetUniformLocation(programID, "P");
        glUniformMatrix4fv(Location, 1, GL_TRUE, perspective.data);
        for (size_t i = 0; i < current_world->objectPtrs.size(); i++)
            current_world->objectPtrs[i]->Draw(programID);

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
    if (mouseIO)
    {
        camera.yaw -= mouse_speed * io.MouseDelta.x;
        camera.pitch += mouse_speed * io.MouseDelta.y;
        if (camera.pitch > 89.0f) camera.pitch = 89.0f;
        if (camera.pitch < -89.0f) camera.pitch = -89.0f;
        matf3 S = cgcore::SchmidtOrthogonalize(current_world->metric(camera.paraPos));
        current_world->camera.UpdateDirection(camera.paraPos, S);
    }

    // Get keyboard input
    auto keyboardInput = io.KeysDown;
    vecf3 du = vecf3(0.0f);             // 相机的参数坐标变化量
    if (keyboardInput['A'])
        du = camera.Right * (-move_speed) * deltaTime;
    else if (keyboardInput['S'])
        du = camera.Front * (move_speed) * deltaTime;
    else if (keyboardInput['D'])
        du = camera.Right * (move_speed) * deltaTime;
    else if (keyboardInput['W'])
        du = camera.Front * (-move_speed) * deltaTime;
    else if (keyboardInput['Q'])
        du = camera.Up * (move_speed) * deltaTime;
    else if (keyboardInput['E'])
        du = camera.Up * (-move_speed) * deltaTime;

    camera.UpdatePosition(current_world->gamma(camera.paraPos), du, current_world->metric(camera.paraPos));
}
