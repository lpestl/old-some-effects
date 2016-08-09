
#include "Texture.h"
#include "FrameBuffer.h"


std::auto_ptr<Texture> Texture::LoadPicture(const char* file, GLenum filter, bool generateMipmap)
{
	std::auto_ptr<Texture> tex( new Texture );
	std::shared_ptr<SDL_Surface> surface( IMG_Load(file), SDL_FreeSurface );
	if( !surface ) {
		SDL_Log("Image file \"%s\" not found or have unsupported format!", file);
		return tex;
	}

	glGenTextures(1, &tex->m_handle);

	tex->Bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	if( generateMipmap && filter == GL_LINEAR )
		filter = GL_LINEAR_MIPMAP_LINEAR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_PixelFormat *format = surface->format;
	if (format->Amask) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}
	if( generateMipmap )
		FramebufferExt::GenerateMipmap(GL_TEXTURE_2D);

	tex->m_width = surface->w;
	tex->m_height = surface->h;
	tex->Unbind();

	return tex;
}

std::auto_ptr<Texture> Texture::CreateBlank(GLuint width, GLuint height, GLenum filter, GLenum format, GLenum internalFormat)
{
	std::auto_ptr<Texture> tex( new Texture );
	glGenTextures(1, &tex->m_handle);

	tex->Bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	if( filter == GL_LINEAR )
		filter = GL_LINEAR_MIPMAP_LINEAR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	tex->m_width = width;
	tex->m_height = height;
	tex->Unbind();

	return tex;
}

void Texture::Bind()
{
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*) &m_previous);
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, m_previous);
}
	
void Texture::GenerateMipmap()
{
	Bind();
	FramebufferExt::GenerateMipmap(GL_TEXTURE_2D);
	Unbind();
}

void Texture::Release()
{
	if( m_handle != 0 )
		glDeleteTextures(1, &m_handle);
	m_handle = 0;
}