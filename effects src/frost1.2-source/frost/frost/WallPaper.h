
#ifndef __WALLPAPER_H__
#define __WALLPAPER_H__

#include <tinyxml2.h>
#include "Sprite.h"



class WallPaper
{
private:
	WallPaper()
	{
		m_rect.x = 0;
		m_rect.y = 0;
	}

public:
	enum VertAlign
	{
		VALIGN_TOP,
		VALIGN_BOTTOM,
		VALIGN_CENTER,
	};
	enum HorzAlign
	{
		HALIGN_LEFT,
		HALIGN_RIGHT,
		HALIGN_CENTER,
	};

	static std::shared_ptr<WallPaper> Create(tinyxml2::XMLElement* e, float winWidth, float winHeight)
	{
		std::shared_ptr<WallPaper> wp( new WallPaper );

		auto element = e->FirstChildElement("filename");
		if( !element )
			return std::shared_ptr<WallPaper>( nullptr );
		std::shared_ptr<Texture> texture = Texture::LoadPicture( element->GetText(), GL_LINEAR );
		
		float texWidth = (float)texture->Width();
		float texHeight = (float)texture->Height();

		float texAspect = texHeight/texWidth;
		float winAspect = winHeight/winWidth;

		if( texAspect > winAspect ) {
			wp->m_rect.width = winWidth;
			wp->m_rect.height = winWidth * texAspect;
		}
		else {
			wp->m_rect.width = winHeight / texAspect;
			wp->m_rect.height = winHeight;
		}
		wp->m_winWidth = winWidth;
		wp->m_winHeight = winHeight;

		wp->m_sprite = Sprite::Create( wp->m_rect.width, wp->m_rect.height );
		wp->m_sprite->SetTexture(texture, 0);
		wp->m_sprite->GenerateFixedTexCoords(0, 0, 0, texWidth, texHeight);

		element = e->FirstChildElement("align");
		if( element )
		{
			const char* halign = element->Attribute("horizontal");
			const char* valign = element->Attribute("vertical");

			wp->SetAlign( (float)atof(halign ? halign : "0.5"), (float)atof(valign ? valign : "0.5") );
		}
		return wp;
	}

	void Render() {
		m_sprite->Render(m_rect.x, m_rect.y);
	}

	void SetAlign(float halign, float valign)
	{
		halign = _clampf(halign, 0, 1);
		valign = _clampf(valign, 0, 1);

		m_rect.x = halign * m_winWidth;
		m_rect.y = valign * m_winHeight;

		m_sprite->SetAnchorNormalized(halign, valign);
	}

private:
	Rect m_rect;
	float m_winWidth;
	float m_winHeight;
	std::shared_ptr<Sprite> m_sprite;
};

#endif //__WALLPAPER_H__