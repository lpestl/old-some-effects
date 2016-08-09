
#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Texture.h"

/*
GL_ARB_framebuffer_object:                                     OK 
--------------------------
  glBindFramebuffer:                                           OK
  glBindRenderbuffer:                                          OK
  glBlitFramebuffer:                                           OK
  glCheckFramebufferStatus:                                    OK
  glDeleteFramebuffers:                                        OK
  glDeleteRenderbuffers:                                       OK
  glFramebufferRenderbuffer:                                   OK
  glFramebufferTexture1D:                                      OK
  glFramebufferTexture2D:                                      OK
  glFramebufferTexture3D:                                      OK
  glFramebufferTextureLayer:                                   OK
  glGenFramebuffers:                                           OK
  glGenRenderbuffers:                                          OK
  glGenerateMipmap:                                            OK
  glGetFramebufferAttachmentParameteriv:                       OK
  glGetRenderbufferParameteriv:                                OK
  glIsFramebuffer:                                             OK
  glIsRenderbuffer:                                            OK
  glRenderbufferStorage:                                       OK
  glRenderbufferStorageMultisample:                            OK
*/


struct FramebufferExt
{
	enum Type
	{
		ChooseEXT,
		ChooseARB,
	};
	typedef decltype(glBindFramebuffer)			BindFramebuffer_f;
	typedef decltype(glGenFramebuffers)			GenFramebuffers_f;
	typedef decltype(glFramebufferTexture2D)	FramebufferTexture2D_f;
	typedef decltype(glCheckFramebufferStatus)	CheckFramebufferStatus_f;
	typedef decltype(glDeleteFramebuffers)		DeleteFramebuffers_f;
	typedef decltype(glGenerateMipmap)			GenerateMipmap_f;

	static BindFramebuffer_f		BindFramebuffer;
	static GenFramebuffers_f		GenFramebuffers;
	static FramebufferTexture2D_f	FramebufferTexture2D;
	static CheckFramebufferStatus_f	CheckFramebufferStatus;
	static DeleteFramebuffers_f		DeleteFramebuffers;
	static GenerateMipmap_f			GenerateMipmap;

	static void InitializeExtension(Type chooseType);
};



class FrameBuffer
{
private:
	FrameBuffer()
	{
		m_previous = 0;
		m_handle = 0;
	}

public:
	static std::shared_ptr<FrameBuffer> Generate(int width, int height, int attachment);

	bool IsValid();
	void Bind();
	void Unbind();
	void Release();

	GLuint Handle() const { return m_handle; }
	bool IsHandleValid() const { return m_handle > 0; }
	int Width() const { return m_texture->Width(); }
	int Height() const { return m_texture->Height(); }
	std::shared_ptr<Texture> GetTexture() { return m_texture; }

private:
	GLuint m_handle;
	GLuint m_previous;
	std::shared_ptr<Texture> m_texture;
};


#endif //__FRAMEBUFFER_H__