
#include <core/gl.h>

namespace cgcore::gl
{

	void SetInt(GLuint programID, const char* name_str, int value)
	{
		GLint loc = glGetUniformLocation(programID, name_str);
		glUniform1i(loc, value);
	}
	void SetFloat(GLuint programID, const char* name_str, float value)
	{
		GLint loc = glGetUniformLocation(programID, name_str);
		glUniform1f(loc, value);
	}
	void SetVec3f(GLuint programID, const char* name_str, const vecf3& value)
	{
		GLint loc = glGetUniformLocation(programID, name_str);
		glUniform3fv(loc, 1, value.data);
	}

	void SetVec4f(GLuint programID, const char* name_str, const vecf4& value)
	{
		GLint loc = glGetUniformLocation(programID, name_str);
		glUniform4fv(loc, 1, value.data);
	}

	void SetMat3f(GLuint programID, const char* name_str, const matf3& value)
	{
		GLint loc = glGetUniformLocation(programID, name_str);
		glUniformMatrix3fv(loc, 1, GL_TRUE, value.data);
	}
	void SetMat4f(GLuint programID, const char* name_str, const matf4& value)
	{
		GLint loc = glGetUniformLocation(programID, name_str);
		glUniformMatrix4fv(loc, 1, GL_TRUE, value.data);
	}
}
