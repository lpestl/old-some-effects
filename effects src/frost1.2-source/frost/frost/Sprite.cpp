
#include "Sprite.h"


std::shared_ptr<Sprite> Sprite::Create(float sw, float sh)
{
	std::shared_ptr<Sprite> spr( new Sprite );

	spr->m_width = sw;
	spr->m_height = sh;
	
	for( int t = 0; t < MAX_TEXTURE_UNITS; ++t )
		spr->m_quad.tex[t] = 0;

	spr->m_quad.v[0].color =
	spr->m_quad.v[1].color =
	spr->m_quad.v[2].color =
	spr->m_quad.v[3].color = Vec4f( 1.0f );

	return spr;
}

void Sprite::SetTexture(std::shared_ptr<Texture> texture, GLenum tu, Sprite::TexCoordsGen texgen)
{
	_ASSERT(tu >= 0 && tu < MAX_TEXTURE_UNITS);

	m_textures[tu] = texture;
	m_quad.tex[tu] = texture->Handle();
	m_texgen[tu] = texgen;
}

void Sprite::GenerateFixedTexCoords(GLenum tu, float sx, float sy, float sw, float sh)
{
	_ASSERT(tu >= 0 && tu < MAX_TEXTURE_UNITS);

	float tw = (float)m_textures[tu]->Width();
	float th = (float)m_textures[tu]->Height();

	m_quad.v[0].texCoords[tu][0] = sx/tw;
	m_quad.v[0].texCoords[tu][1] = ( sy + sh )/th;

	m_quad.v[1].texCoords[tu][0] = ( sx + sw )/tw;
	m_quad.v[1].texCoords[tu][1] = ( sy + sh )/th;

	m_quad.v[2].texCoords[tu][0] = ( sx + sw )/tw;
	m_quad.v[2].texCoords[tu][1] = sy/th;

	m_quad.v[3].texCoords[tu][0] = sy/tw;
	m_quad.v[3].texCoords[tu][1] = sy/th;
}

void Sprite::EnableScreenSpaceTexCoordsGeneration(float screenWidth, float screenHeight)
{
	if( screenWidth > 0 && screenHeight > 0 )
	{
		m_sw = screenWidth;
		m_sh = screenHeight;
		m_texgenIsEnabled = true;
	}
}

void Sprite::Render(float x, float y)
{
	float x1 = x - m_anchor[0];
	float y1 = y - m_anchor[1];
	float x2 = x1 + m_width;
	float y2 = y1 + m_height;

	// расчитываем координаты
	m_quad.v[0].pos[0] = x1; m_quad.v[0].pos[1] = y1;
	m_quad.v[1].pos[0] = x2; m_quad.v[1].pos[1] = y1;
	m_quad.v[2].pos[0] = x2; m_quad.v[2].pos[1] = y2;
	m_quad.v[3].pos[0] = x1; m_quad.v[3].pos[1] = y2;

	if( m_texgenIsEnabled )
	{
		for( int t = 0; t < MAX_TEXTURE_UNITS; ++t )
		{
			if( m_texgen[t] == TEXGEN_SCREENSPACE )
			{
				m_quad.v[0].texCoords[t][0] = x1/m_sw;
				m_quad.v[0].texCoords[t][1] = 1 - y1/m_sh;

				m_quad.v[1].texCoords[t][0] = x2/m_sw;
				m_quad.v[1].texCoords[t][1] = 1 - y1/m_sh;

				m_quad.v[2].texCoords[t][0] = x2/m_sw;
				m_quad.v[2].texCoords[t][1] = 1 - y2/m_sh;

				m_quad.v[3].texCoords[t][0] = x1/m_sw;
				m_quad.v[3].texCoords[t][1] = 1 - y2/m_sh;
			}
		}
	}
	RenderQuad( &m_quad );
}

void Sprite::RenderAligned(const Rect& box, float halign, float valign)
{
	halign = _clampf(halign, 0, 1);
	valign = _clampf(valign, 0, 1);

	Vec2f oldAnchor = m_anchor;
	SetAnchorNormalized( halign, valign );

	Render( box.x + halign * box.width, box.y + valign * box.height );
	m_anchor = oldAnchor;
}

void Sprite::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
}

void Sprite::RenderStretch(float x1, float y1, float x2, float y2)
{
}

void Sprite::SetColor(float r, float g, float b, float alpha)
{
	for( int i = 0; i < 4; ++i )
	{
		m_quad.v[i].color[0] = r;
		m_quad.v[i].color[1] = g;
		m_quad.v[i].color[2] = b;
		m_quad.v[i].color[3] = alpha;
	}
}

void Sprite::SetAlpha(float alpha)
{
	for( int i = 0; i < 4; ++i )
		m_quad.v[i].color[3] = alpha;
}

void Sprite::SetAnchor(float ax, float ay)
{
	m_anchor[0] = ax;
	m_anchor[1] = ay;
}

void Sprite::SetAnchorNormalized(float clampX, float clampY)
{
	clampX = _clampf(clampX, 0, 1);
	clampY = _clampf(clampY, 0, 1);

	m_anchor[0] = m_width * clampX;
	m_anchor[1] = m_height * clampY;
}

void Sprite::Release()
{
	for( int t = 0; t < MAX_TEXTURE_UNITS; ++t )
		if( m_textures[t].get() != nullptr )
			m_textures[t]->Release();
}