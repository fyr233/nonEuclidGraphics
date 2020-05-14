#pragma once

#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include <GLFW/glfw3.h>

#include <core/mat.h>

namespace cgcore
{
	namespace gl
	{
		void SetInt(GLuint programID, const char* name_str, int value);
		
		void SetFloat(GLuint programID, const char* name_str, float value);
		
		void SetVec3f(GLuint programID, const char* name_str, const vecf3& value);
		
		template<typename vec3f_type>
		void TSetVec3f(GLuint programID, const char* name_str, const vec3f_type &value)
		{
			GLint loc = glGetUniformLocation(programID, name_str);
			glUniform3fv(loc, 1, (float*)&value);
		}
		void SetVec4f(GLuint programID, const char* name_str, const vecf4& value);

		template<typename vec4f_type>
		void TSetVec4f(GLuint programID, const char* name_str, const vec4f_type& value)
		{
			GLint loc = glGetUniformLocation(programID, name_str);
			glUniform4fv(loc, 1, (float*)&value);
		}
		void SetMat3f(GLuint programID, const char* name_str, const matf3& value);

		void SetMat4f(GLuint programID, const char* name_str, const matf4& value);
	}
}