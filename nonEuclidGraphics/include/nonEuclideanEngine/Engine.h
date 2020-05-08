#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>


#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include <GLFW/glfw3.h>

#include<nonEuclideanEngine/world.h>
#include <core/Mesh.h>
#include <core/Obj.h>
#include <nonEuclideanEngine/shader.hpp>

namespace nonEuc
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void SetWorld(World *world){ current_world = world; }
		bool Init();	// 初始化OpenGL和Imgui
		void Loop();	// 主要的循环
		void Clear();	// 在结束的时候调用

		void UpdateCamera();
		matf4 Perspective(float fovY, float aspect, float zNear, float zFar);	// 透视投影变换，需要的话可以写进transform.h

	private:
		// 目的就是把这个场景的东西显示出来
		// 里面应该包括：
		//	1.所有的Obj(Mesh)
		//	2.一个Camera
		//	3.合适的变换
		World *current_world = nullptr;


		GLFWwindow* window = nullptr;		// 窗口
		GLuint programID = 0;				// 着色器
		int scrwidth = 1280;
		int scrheight = 720;
		bool show_demo_window = false;
		bool mouseIO = false;
		float mouse_speed = 0.05f;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};
}


