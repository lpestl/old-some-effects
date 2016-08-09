
#ifndef __WAVESAPPLICATION_H__
#define __WAVESAPPLICATION_H__

#include "Application.h"
#include "WavesRenderer.h"
#include "FontRenderer.h"
#include "HitekSocket.h"

struct Rect
{
	float left;
	float right;
	float bottom;
	float top;
};


class WavesApplication: public HitekSocketListener, public Application
{
public:
	void receiveAction();
	HitekSocket* hitekSocket;
	int inputMode;

	WavesApplication();

	bool OnCreate();
	void OnDestroy();
	void OnFrame();
	void OnRender();

	void OnKeyDown(SDL_Keycode keycode, SDL_Scancode scancode, Uint16 mod);
	void OnMouseMove(int mx, int my, int relx, int rely, bool left, bool right, bool middle);
	std::string MakeAbsPath(std::string relativePath);

private:
	std::string m_workingDir;
	std::string m_vendorName;
	std::string m_rendererName;
	WaveEquationSolver m_solver;

	std::auto_ptr<WavesRenderer> m_wavesRenderer;	
	std::auto_ptr<Texture> m_bgTexture;
	std::auto_ptr<FrameBuffer> m_bgFrameBuffer;
	std::auto_ptr<ShaderProgram> m_shader;
	std::auto_ptr<FontRenderer> m_systemFont;
	Rect m_bgRect;
	Uint32 m_lastMouseUpdate;
};

#endif //__WAVESAPPLICATION_H__