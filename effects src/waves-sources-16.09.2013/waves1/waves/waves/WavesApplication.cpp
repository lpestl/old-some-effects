#include "WavesApplication.h"
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <gl/glew.h>
#include <gl/glu.h>
#include "FileSystem.h"




WavesApplication::WavesApplication()
{
	m_workingDir = Directory::GetCurrentDir();
}

std::string WavesApplication::MakeAbsPath(std::string relativePath)
{
	return m_workingDir + "/" + relativePath;
}


bool WavesApplication::OnCreate()
{
	m_vendorName = (const char*)glGetString(GL_VENDOR);
	m_rendererName = (const char*)glGetString(GL_RENDERER);

	SDL_Log("%s", m_vendorName.c_str());
	SDL_Log("%s\n", m_rendererName.c_str());

	if( !glewIsSupported("GL_VERSION_2_0") )
	{
		SDL_Log("Application requies OpenGL version 2.0");
		return false;
	}
	SDL_Log("GL_VERSION_2_0: supported");

	if( glewIsSupported("GL_ARB_framebuffer_object") )
	{
		FramebufferExt::InitializeExtension( FramebufferExt::ChooseARB );
		SDL_Log("GL_ARB_framebuffer_object: supported");
	}
	else if( glewIsSupported("GL_EXT_framebuffer_object") )
	{
		FramebufferExt::InitializeExtension( FramebufferExt::ChooseEXT );
		SDL_Log("GL_EXT_framebuffer_object: supported");
	}
	else {
		SDL_Log("Application requies one of:\n  GL_ARB_framebuffer_object\n  GL_EXT_framebuffer_object");
		return false;
	}

	int width = Application::WindowWidth();
	int height = Application::WindowHeight();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, width, height);

	std::string filename = MakeAbsPath("settings.ini");
	ProfileReader settings( filename.c_str() );

	filename = settings.GetString("TEXTURES", "background");
	filename = MakeAbsPath( filename );
	m_bgTexture = Texture::LoadPicture( filename.c_str(), GL_LINEAR );

	filename = MakeAbsPath("fonts/system.fnt");
	m_systemFont = FontRenderer::LoadFont( filename.c_str() );
	
	filename = settings.GetString("SHADERS","WavesFragmentShader");
	filename = MakeAbsPath( filename );
	std::string fsh = Directory::LoadText( filename );

	filename = settings.GetString("SHADERS","WavesVertexShader");
	filename = MakeAbsPath( filename );
	std::string vsh = Directory::LoadText( filename );

	m_shader = ShaderProgram::Create(vsh.c_str(), fsh.c_str());
	if( ShaderProgram::GetLastError() != ShaderProgram::ERR_NO_ERROR )
	{
		std::string errorString;
		switch( ShaderProgram::GetLastError() )
		{
		case ShaderProgram::ERR_FS_COMPILE_ERROR:
			errorString += "Fragment shader compile error:\n";
			errorString += m_shader->GetFSLog();
			break;
		case ShaderProgram::ERR_VS_COMPILE_ERROR:
			errorString += "Vertex shader compile error:\n";
			errorString += m_shader->GetVSLog();
			break;
		case ShaderProgram::ERR_PROGRAM_LINK_ERROR:
			errorString += "Shader build error:\n";
			errorString += m_shader->GetProgramLog();
			break;
		}
		SDL_Log("\n%s", errorString.c_str());
	}
	float damping = settings.GetFloat("SETTINGS", "Damping", 0.5f);
	int timeStep = 5;
	float speed = width/6.0f;
	float gridStep = width/120.0f; // для разрешения 1920х1080 получим шаг 16

	m_solver.CreateGrid((float)width, (float)height, gridStep, timeStep, speed);
	m_solver.SetDamping(damping);
	m_wavesRenderer = WavesRenderer::Create(&m_solver, width, height);


	int bgWidth = m_bgTexture->Width();
	int bgHeight = m_bgTexture->Height();

	if( bgWidth > 0 && bgHeight > 0 )
	{
		float texAspect = (float)bgWidth/bgHeight;
		float winAspect = (float)width/height;

		float newWidth;
		float newHeight;

		if( texAspect < winAspect ) {
			newWidth = (float)width;
			newHeight = bgHeight * (float)width/bgWidth;
		}
		else {
			newWidth = bgWidth * (float)height/bgHeight;
			newHeight = (float)height;
		}
		m_bgRect.left = -(newWidth - width)/2.0f;
		m_bgRect.right = width + (newWidth - width)/2.0f;
		
		m_bgRect.bottom = -(newHeight - height)/2.0f;
		m_bgRect.top = height + (newHeight - height)/2.0f;
	}	
		
	m_bgFrameBuffer = FrameBuffer::Generate(width, height, 0);
	m_lastMouseUpdate = SDL_GetTicks();

	hitekSocket=new HitekSocket();
	if(hitekSocket->inputMode==0){
		hitekSocket->hitekSocketListener=this;
	}
	return true;
}

void WavesApplication::receiveAction(){
	Uint32 interval = 50; //milliseconds
	Uint32 time = SDL_GetTicks();

	if( time - m_lastMouseUpdate > interval )
	{
		m_solver.PerturbationCoord(hitekSocket->onePoint.x, Application::WindowHeight() - hitekSocket->onePoint.y);
		m_lastMouseUpdate = time;
	}
}

void WavesApplication::OnDestroy()
{
	m_wavesRenderer->Release();
	m_shader->Release();
	m_bgFrameBuffer->Release();
	m_bgTexture->Release();
}

void WavesApplication::OnFrame()
{
	Uint32 dt = Application::TimerGetDelta();
	m_solver.SolveNextFrame(dt);
}


void WavesApplication::OnRender()
{
	Application::GfxBeginScene();

	int width = Application::WindowWidth();
	int height = Application::WindowHeight();
	
	
	m_bgFrameBuffer->Bind();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, 0, height, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	m_bgTexture->Bind();
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0,1);
		glVertex3f(m_bgRect.left, m_bgRect.bottom, 0);

		glTexCoord2f(1,1);
		glVertex3f(m_bgRect.right, m_bgRect.bottom, 0);

		glTexCoord2f(1,0);
		glVertex3f(m_bgRect.right, m_bgRect.top, 0);

		glTexCoord2f(0,0);
		glVertex3f(m_bgRect.left, m_bgRect.top, 0);
	}
	glEnd();
	m_bgTexture->Unbind();
	m_bgFrameBuffer->Unbind();


#ifndef TEST_SKEW_VIEW
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, 0, height, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#else
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, -height/2.0f, height/2.0f, 1000, -1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1,-1,1,0,0,0,0,0,1);
#endif

	
	m_shader->UseProgram();
	glUniform1i( m_shader->GetUniform("background"), 0 );
	glUniform1f( m_shader->GetUniform("screenWidth"), (float)width );
	glUniform1f( m_shader->GetUniform("screenHeight"), (float)height );

	glActiveTexture( GL_TEXTURE0 );
	m_bgFrameBuffer->GetTexture()->Bind();

	m_wavesRenderer->Render( m_shader.get() );

	m_bgFrameBuffer->GetTexture()->Unbind();
	m_shader->DisableProgram();

#ifndef NDEBUG
	//
	// прорисовка отладочной информации
	//
	Rect r = { 0, (float)width, (float)height - 50.0f, (float)height };
	
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0, 0, 0, 0.3f);
	glBegin(GL_QUADS);
		glVertex3f(r.left, r.bottom, 0);
		glVertex3f(r.right, r.bottom, 0);
		glVertex3f(r.right, r.top, 0);
		glVertex3f(r.left, r.top, 0);
	glEnd();
	glPopAttrib();

	char fps[64];
	sprintf_s(fps, 64, "FPS: %d", Application::TimerGetFps());

	m_systemFont->RenderText(20, r.bottom + 10, fps);
	m_systemFont->RenderText(120, r.bottom + 10, m_rendererName.c_str());
#endif

	Application::GfxEndScene();
}

void WavesApplication::OnKeyDown(SDL_Keycode keycode, SDL_Scancode scancode, Uint16 mod)
{
	if( keycode == SDLK_ESCAPE )
		Application::SystemExit();
}

void WavesApplication::OnMouseMove(int mx, int my, int relx, int rely, bool left, bool right, bool middle)
{	
	if(hitekSocket->inputMode==1){
		Uint32 interval = 50; //milliseconds
		Uint32 time = SDL_GetTicks();

		if( time - m_lastMouseUpdate > interval )
		{
			m_solver.PerturbationCoord(mx, Application::WindowHeight() - my);
			m_lastMouseUpdate = time;
		}
	}
}