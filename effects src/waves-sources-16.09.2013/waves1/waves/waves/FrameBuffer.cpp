
#include "FrameBuffer.h"


FramebufferExt::BindFramebuffer_f		 FramebufferExt::BindFramebuffer;
FramebufferExt::GenFramebuffers_f		 FramebufferExt::GenFramebuffers;
FramebufferExt::FramebufferTexture2D_f	 FramebufferExt::FramebufferTexture2D;
FramebufferExt::CheckFramebufferStatus_f FramebufferExt::CheckFramebufferStatus;
FramebufferExt::DeleteFramebuffers_f	 FramebufferExt::DeleteFramebuffers;
FramebufferExt::GenerateMipmap_f		 FramebufferExt::GenerateMipmap;



void FramebufferExt::InitializeExtension(Type chooseType)
{
	switch( chooseType )
	{
	case ChooseEXT:
		BindFramebuffer		   = glBindFramebufferEXT;
		GenFramebuffers		   = glGenFramebuffersEXT;
		FramebufferTexture2D   = glFramebufferTexture2DEXT;
		CheckFramebufferStatus = glCheckFramebufferStatusEXT;
		DeleteFramebuffers	   = glDeleteFramebuffersEXT;
		GenerateMipmap		   = glGenerateMipmapEXT;
		break;

	case ChooseARB:
		BindFramebuffer		   = glBindFramebuffer;
		GenFramebuffers		   = glGenFramebuffers;
		FramebufferTexture2D   = glFramebufferTexture2D;
		CheckFramebufferStatus = glCheckFramebufferStatus;
		DeleteFramebuffers	   = glDeleteFramebuffers;
		GenerateMipmap		   = glGenerateMipmap;
		break;
	};
}

std::auto_ptr<FrameBuffer> FrameBuffer::Generate(int width, int height, int attachment)
{
	std::auto_ptr<FrameBuffer> buffer( new FrameBuffer );
	if( width <= 0 || height <= 0 )
		return buffer;

	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);

	if( width > maxSize || height > maxSize )
		return buffer;
		
	buffer->m_texture = Texture::CreateBlank(width, height, GL_NEAREST, GL_RGB, GL_RGB8);
	FramebufferExt::GenFramebuffers(1, &buffer->m_handle);

	buffer->Bind();
	buffer->m_texture->Bind();

	FramebufferExt::BindFramebuffer(GL_FRAMEBUFFER, buffer->m_handle);
	FramebufferExt::FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, buffer->m_texture->Handle(), 0);

	buffer->m_texture->Unbind();
	buffer->Unbind();

	return buffer;
}

bool FrameBuffer::IsValid()
{
	if( m_handle < 1 )
		return false;

	Bind();
	GLenum status = FramebufferExt::CheckFramebufferStatus(GL_FRAMEBUFFER);
	Unbind();

	return( status == GL_FRAMEBUFFER_COMPLETE );
}

void FrameBuffer::Bind()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*) &m_previous);
	FramebufferExt::BindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

void FrameBuffer::Unbind()
{
	FramebufferExt::BindFramebuffer(GL_FRAMEBUFFER, m_previous);
}

void FrameBuffer::Release()
{
	m_texture->Release();
	if( m_handle != 0 )
		FramebufferExt::DeleteFramebuffers(1, &m_handle);
	m_handle = 0;
}