
#ifndef __FROSTAPPLICATION_H__
#define __FROSTAPPLICATION_H__

#include <crtdbg.h>
#include <tinyxml2.h>
#include "Mathematics.h"
#include "Application.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "ShaderProgram.h"
#include "FontRenderer.h"
#include "Sprite.h"
#include "WallPaper.h"
#include "FrozenCanvas.h"
#include "HitekSocket.h"



class FrostApplication: public HitekSocketListener,public Application
{
public:
	void receiveAction();
	HitekSocket* hitekSocket;
	int inputMode;

	FrostApplication();

	bool ParseWallpapers(tinyxml2::XMLElement* e);
	bool ParseBrush(tinyxml2::XMLElement* e);

	bool OnCreate();
	void OnDestroy();
	void OnFrame();
	void OnRender();

	void OnKeyDown(SDL_Keycode keycode, SDL_Scancode scancode, Uint16 mod);
	void OnMouseMove(int mx, int my, int relx, int rely, bool left, bool right, bool middle);
	void OnLButtonDown(int mx, int my);
	void OnLButtonUp(int mx, int my);

private:
	bool m_drawDebugInfo;
	std::string m_vendorName;
	std::string m_rendererName;
	
	std::shared_ptr<Texture> m_texBack;
	std::shared_ptr<Texture> m_texFrost;
	std::shared_ptr<FrozenCanvas> m_canvas;
	std::shared_ptr<Sprite> m_debugPanel;
	std::shared_ptr<WallPaper> m_background;
	std::shared_ptr<WallPaper> m_frozenGlass;
	std::shared_ptr<FrameBuffer> m_bgFrameBuffer;
	std::shared_ptr<FontRenderer> m_systemFont;
	Uint32 m_lastMouseUpdate;
};

#endif //__FROSTAPPLICATION_H__