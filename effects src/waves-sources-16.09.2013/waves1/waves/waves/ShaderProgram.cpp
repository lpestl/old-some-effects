
#include <memory>
#include "ShaderProgram.h"


ShaderProgram::LastError ShaderProgram::s_lastError = ShaderProgram::ERR_NO_ERROR;


ShaderProgram::ShaderProgram()
{
	m_vertexShader = 0;
	m_fragmentShader = 0;
	m_program = 0;
	s_lastError = ERR_NO_ERROR;
}


std::auto_ptr<ShaderProgram> ShaderProgram::Create(const GLchar* vertexSource, const GLchar* fragmentSource)
{
	std::auto_ptr<ShaderProgram> shader( new ShaderProgram );
	s_lastError = ERR_NO_ERROR;

	int vsCompiled;
	int fsCompiled;
	int progLinked;

	shader->m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shader->m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shader->m_vertexShader, 1, &vertexSource, 0);
	glShaderSource(shader->m_fragmentShader, 1, &fragmentSource, 0);

	glCompileShader(shader->m_vertexShader);
	glGetShaderiv(shader->m_vertexShader, GL_COMPILE_STATUS, &vsCompiled);

	if (vsCompiled != GL_TRUE)
	{
		s_lastError = ERR_VS_COMPILE_ERROR;
		return shader;
	}

	glCompileShader(shader->m_fragmentShader);
	glGetShaderiv(shader->m_fragmentShader, GL_COMPILE_STATUS, &fsCompiled);

	if (fsCompiled != GL_TRUE)
	{
		s_lastError = ERR_FS_COMPILE_ERROR;
		return shader;
	}

	shader->m_program = glCreateProgram();
	printf("%d\n",sizeof(shader->m_program));

	glAttachShader(shader->m_program, shader->m_vertexShader);
	glAttachShader(shader->m_program, shader->m_fragmentShader);

	glLinkProgram(shader->m_program);
	glGetProgramiv(shader->m_program, GL_LINK_STATUS, &progLinked);

	if (progLinked != GL_TRUE)
	{
		s_lastError = ERR_PROGRAM_LINK_ERROR;
		return shader;
	}
	return shader;
}


void ShaderProgram::Release()
{
	if( m_program > 0 )
		glDeleteShader(m_program);
	m_program = 0;

	if( m_vertexShader > 0 )
		glDeleteShader(m_vertexShader);
	m_vertexShader = 0;

	if( m_fragmentShader > 0 )
		glDeleteShader(m_fragmentShader);
	m_fragmentShader = 0;
}


void ShaderProgram::UseProgram()
{
	glUseProgram(m_program);
}

void ShaderProgram::DisableProgram()
{
	glUseProgram(0);
}

GLint ShaderProgram::GetUniform(const GLchar* name)
{
	return glGetUniformLocation(m_program, name);
}

GLint ShaderProgram::GetAttrib(const GLchar* name)
{
	return glGetAttribLocation(m_program, name);
}

std::string ShaderProgram::GetShaderInfoLog(GLuint object) const
{
	int logLength;
	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		std::shared_ptr<GLchar> log( new GLchar[logLength] );
		glGetShaderInfoLog(object, logLength, 0, log.get());
		return log.get();
	}
	return "";
}

std::string ShaderProgram::GetProgramInfoLog(GLuint object) const
{
	int logLength;
	glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		std::shared_ptr<GLchar> log( new GLchar[logLength] );
		glGetProgramInfoLog(object, logLength, 0, log.get());
		return log.get();
	}
	return "";
}
