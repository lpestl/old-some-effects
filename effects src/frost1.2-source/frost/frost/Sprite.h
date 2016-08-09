
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <memory>
#include "Rendering.h"
#include "Mathematics.h"
#include "Texture.h"



class Sprite
{
private:
	Sprite(const Sprite&);
	void operator = (const Sprite&);

	Sprite()
	{
		m_texgenIsEnabled = false;
	}

public:
	static std::shared_ptr<Sprite> Create(float sw, float sh);

	void Render(float x, float y);
	void RenderAligned(const Rect& box, float halign, float valign);
	void RenderEx(float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f);
	void RenderStretch(float x1, float y1, float x2, float y2);
	
	void SetColor(float r, float g, float b, float alpha = 1.0f);
	void SetAlpha(float alpha);
	void SetAnchor(float ax, float ay);
	void SetAnchorNormalized(float clampX, float clampY);

	enum TexCoordsGen
	{
		TEXGEN_FIXED,
		TEXGEN_SCREENSPACE,
	};
	void SetTexture(std::shared_ptr<Texture> texture, GLenum texUnit, TexCoordsGen texgen = TEXGEN_FIXED);
	void GenerateFixedTexCoords(GLenum texUnit, float texSpaceX, float texSpaceY, float texSpaceWidth, float texSpaceHeight);
	void EnableScreenSpaceTexCoordsGeneration(float screenWidth, float screenHeight);

	void Release();

	std::shared_ptr<Texture> GetTexture(GLenum texUnit) { return m_textures[texUnit]; }
	float Width() const { return m_width; }
	float Height() const { return m_height; }

private:
	float m_width;
	float m_height;
	ScreenQuad m_quad;
	Vec2f m_anchor;

	// screen width and height for screenspace texture coords generation
	float m_sw, m_sh;
	bool m_texgenIsEnabled;
	TexCoordsGen m_texgen[MAX_TEXTURE_UNITS];
	std::shared_ptr<Texture> m_textures[MAX_TEXTURE_UNITS];
};

#endif //__SPRITE_H__