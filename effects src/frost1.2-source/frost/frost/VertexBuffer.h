
#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include <gl/glew.h>



class VertexBuffer
{
public:
	VertexBuffer()
	{
		m_handle = 0;
		m_target = 0;
	}
	void Release()
	{
		if( m_handle != 0 )
			glDeleteBuffers(1, &m_handle);
		m_handle = 0;
	}
	void Bind( GLenum target )
	{
		m_target = target;
		if( m_handle == 0 )
			glGenBuffers(1, &m_handle);
		glBindBuffer( m_target, m_handle );
	}
	void Unbind() {
		glBindBuffer( m_target,0 );
	}
	void SetData(GLsizeiptr size, GLvoid* ptr, GLenum usage) {
		glBufferData(m_target, size, ptr, usage);
	}
	void SetSubData(GLintptr offset, GLsizeiptr size, const GLvoid* ptr) {
		glBufferSubData(m_target, offset, size, ptr);
	}
	void GetSubData(GLintptr offset, GLsizeiptr size, GLvoid* ptr) {
		glGetBufferSubData(m_target, offset, size, ptr);
	}
	void* MapBuffer(GLenum access) {
		return glMapBuffer(m_target, access);
	}
	void UnmapBuffer() {
		glUnmapBuffer(m_target);
	}

private:
	GLuint m_handle;
	GLenum m_target;
};


#endif //__VERTEXBUFFER_H__