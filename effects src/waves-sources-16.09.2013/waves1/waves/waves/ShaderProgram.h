
#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <gl/glew.h>
#include <gl/wglew.h>
#include <string>
#include <memory>


class ShaderProgram
{
private:
	ShaderProgram();

public:
	enum LastError
	{
		ERR_NO_ERROR,
		ERR_VS_COMPILE_ERROR,
		ERR_FS_COMPILE_ERROR,
		ERR_PROGRAM_LINK_ERROR,
	};
	static std::auto_ptr<ShaderProgram> Create(const GLchar* vertexSource, const GLchar* fragmentSource);
	static LastError GetLastError() { return s_lastError; }

	void Release();
	void UseProgram();
	void DisableProgram();
	GLint GetUniform(const GLchar* name);
	GLint GetAttrib(const GLchar* name);

	GLuint GetVS() const { return m_vertexShader; }
	GLuint GetFS() const { return m_fragmentShader; }
	GLuint GetProgram() const { return m_program; }

	std::string GetVSLog() const { return GetShaderInfoLog(m_vertexShader); }
	std::string GetFSLog() const { return GetShaderInfoLog(m_fragmentShader); }
	std::string GetProgramLog() const { return GetProgramInfoLog(m_program); }

private:
	std::string GetShaderInfoLog(GLuint object) const;
	std::string GetProgramInfoLog(GLuint object) const;

private:
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_program;
	static LastError s_lastError;
};

#endif //__SHADERPROGRAM_H__