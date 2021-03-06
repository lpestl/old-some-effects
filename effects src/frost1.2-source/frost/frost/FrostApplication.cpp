#include "FrostApplication.h"
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <gl/glew.h>
#include <gl/glu.h>
#include "Mathematics.h"
#include "FileSystem.h"
#include "Unicode.h"



FrostApplication::FrostApplication()
{
#ifndef NDEBUG
	m_drawDebugInfo = true;
#else
	m_drawDebugInfo = false;
#endif
}
void FrostApplication::receiveAction(){
	m_canvas->DrawAt( (float)hitekSocket->onePoint.x, (float)(Application::WindowHeight() - hitekSocket->onePoint.y) );
}

bool FrostApplication::ParseWallpapers(tinyxml2::XMLElement* e)
{
	int width = Application::WindowWidth();
	int height = Application::WindowHeight();

	tinyxml2::XMLElement* e1 = e->FirstChildElement("inputmode");
	if(strcmp(e1->GetText(),"0")==1)
		inputMode=1;
	else
		inputMode=0;
	e1=e->FirstChildElement("port");
	int port=atoi(e1->GetText());
	e1=e->FirstChildElement("host");
	e = e->FirstChildElement("wallpaper");

	while( e != nullptr )
	{
		const char* name = e->Attribute("name");

		if( !name ) {
			e = e->NextSiblingElement("wallpaper");
			continue;
		}
		else if( strcmp(name, "background") == 0 )
		{
			m_background = WallPaper::Create(e, (float)width, (float)height);
		}
		else if( strcmp(name, "frozenGlass") == 0 )
		{
			m_frozenGlass = WallPaper::Create(e, (float)width, (float)height);
		}
		e = e->NextSiblingElement("wallpaper");
	}
	if( !m_background )
	{
		SDL_Log("Cannot load settings for background wallpaper. Please check for valid \"settings.xml\"");
		return false;
	}
	if( !m_frozenGlass )
	{
		SDL_Log("Cannot load settings for frozenGlass wallpaper. Please check for valid \"settings.xml\"");
		return false;
	}
	
	hitekSocket=new HitekSocket();
	if(inputMode==0){
		hitekSocket->portNumber=port;
		strcpy(hitekSocket->host,e1->GetText());
		hitekSocket->initSocket();
		hitekSocket->hitekSocketListener=this;
	}
	return true;
}


bool FrostApplication::ParseBrush(tinyxml2::XMLElement* e)
{
	int width = Application::WindowWidth();
	int height = Application::WindowHeight();

	if( e != nullptr )
	{
		m_canvas = FrozenCanvas::Create(e, m_bgFrameBuffer->GetTexture(), (float)width, (float)height);
		return true;
	}
	SDL_Log("Cannot load brush settings. Please check for valid \"settings.xml\"");
	return false;
}


bool FrostApplication::OnCreate()
{
	
	SDL_ShowCursor(0);

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
	
	m_bgFrameBuffer = FrameBuffer::Generate(width, height, 0);
	m_systemFont = FontRenderer::LoadFont("fonts/system.fnt");

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glViewport(0, 0, width, height);

	tinyxml2::XMLDocument doc;
	doc.LoadFile("settings.xml");

	auto docElement = doc.FirstChildElement("document");
	if( !docElement )
	{
		SDL_Log("File \"settings.xml\" not found or has incorrect format");
		return false;
	}
	if( !ParseWallpapers( docElement ) )
		return false;

	auto brushElement = docElement->FirstChildElement("brush");
	if( !ParseBrush( brushElement ) )
		return false;
			
	m_debugPanel = Sprite::Create((float)width, 80);
	m_debugPanel->SetColor(0, 0, 0, 0.4f);		

	m_texBack = Texture::LoadPicture("images/novogodnyaya_noch_3.jpg", GL_LINEAR);
	m_texFrost = Texture::LoadPicture("images/frozenGlass.jpg", GL_LINEAR);
	
	m_lastMouseUpdate = SDL_GetTicks();

	m_canvas->Drawing(true);

	return true;
}

void FrostApplication::OnDestroy()
{
	m_bgFrameBuffer->Release();
	//m_background->Release();
}

void FrostApplication::OnFrame()
{
	Uint32 dt = Application::TimerGetDelta();
	m_canvas->Frame(dt);
}
float g_shift = 0;

void FrostApplication::OnRender()
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
	glTranslated(0, height, 0);
	glScalef(1, -1, 1);

	m_background->Render();
	m_bgFrameBuffer->Unbind();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, 0, height, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_frozenGlass->Render();
	m_canvas->Render();

	//
	// прорисовка отладочной информации
	//
	if( m_drawDebugInfo )
	{
		float bottom = (float)height - m_debugPanel->Height();
		m_debugPanel->Render(0, bottom);

		char fps[128];
		sprintf_s(fps, 64, "FPS: %d;\nparticles: %d;", Application::TimerGetFps(), m_canvas->GetParticleCount());

		m_systemFont->SetColor(1, 1, 1, 1);
		m_systemFont->RenderText(20, bottom + 10, fps);
		m_systemFont->RenderText(120, bottom + 10, m_rendererName.c_str());

		//m_systemFont->SetColor(0,0,0,1);
		//m_canvas->DrawDebugInfo( m_systemFont.get() );
	}

	Application::GfxEndScene();
}

void FrostApplication::OnKeyDown(SDL_Keycode keycode, SDL_Scancode scancode, Uint16 mod)
{
	if( keycode == SDLK_ESCAPE )
		Application::SystemExit();

	if( keycode == SDLK_F9 )
		m_drawDebugInfo = !m_drawDebugInfo;

	if( keycode == SDLK_UP )
		g_shift += 5.0f;

	if( keycode == SDLK_DOWN )
		g_shift -= 5.0f;
}

void FrostApplication::OnMouseMove(int mx, int my, int relx, int rely, bool left, bool right, bool middle)
{
	if(inputMode==1)
		m_canvas->DrawAt( (float)mx, (float)(Application::WindowHeight() - my) );
}

void FrostApplication::OnLButtonDown(int mx, int my)
{
	//m_canvas->Drawing(true);
	//m_canvas->DrawAt( (float)mx, (float)(Application::WindowHeight() - my) );
}

void FrostApplication::OnLButtonUp(int mx, int my)
{
//	m_canvas->DrawAt( (float)mx, (float)(Application::WindowHeight() - my) );
//	m_canvas->Drawing(false);
}