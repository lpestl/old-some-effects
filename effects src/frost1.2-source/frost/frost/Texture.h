
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL.h>
#include <SDL_image.h>
#include <gl/glew.h>
#include <gl/glu.h>
#include <memory>



class Texture
{
private:
	Texture()
	{
		m_width = 0;
		m_height = 0;
		m_previous = 0;
		m_handle = 0;
	}
	Texture(const Texture&);
	void operator = (const Texture&);

public:
	static std::shared_ptr<Texture> LoadPicture(const SDL_Surface* surface, GLenum filter, bool generateMipmap = true);
	static std::shared_ptr<Texture> LoadPicture(const char* file, GLenum filter, bool generateMipmap = true);
	static std::shared_ptr<Texture> CreateBlank(GLuint width, GLuint height, GLenum filter, GLenum format, GLenum internalFormat);

	void Bind();
	void Unbind();	
	void GenerateMipmap();
	void Release();

	GLuint Handle() const { return m_handle; }
	bool IsHandleValid() const { return m_handle > 0; }
	int Width() const { return m_width; }
	int Height() const { return m_height; }

private:
	GLuint m_handle;
	GLuint m_previous;
	int m_width;
	int m_height;
};

#endif //__TEXTURE_H__