
#include "FrozenCanvas.h"



CanvasCell* FrozenCanvas::GetCell(int i, int j)
{
	return &m_cells[ j*m_gridCols + i ];
}

CanvasCell* FrozenCanvas::GetCell(float x, float y)
{
	int i = (int) floor( x/m_cellWidth );
	int j = (int) floor( y/m_cellHeight );
	i = _clampi( i, 0, m_gridCols-1 );
	j = _clampi( j, 0, m_gridRows-1 );

	return GetCell(i, j);
}


std::shared_ptr<Texture> GenerateBrush(float smallRadius, float bigRadius, float degree)
{
	smallRadius = __max(0, smallRadius);
	bigRadius = __max(1, bigRadius);

	if( smallRadius > bigRadius )
		std::swap( smallRadius, bigRadius );

	int texSize = 2 * (int) ceilf( bigRadius );
	std::shared_ptr<unsigned char> data( new unsigned char[texSize * texSize * 4] );
	unsigned char* ptr = data.get();

	degree = pow(2.0f, degree);
	float dist;
	float alpha;
	Vec2f center( texSize/2.0f );
	Vec2f pixel;

	for( int i = 0; i < texSize; ++i )
	for( int j = 0; j < texSize; ++j )
	{
		pixel[0] = i + 0.5f;
		pixel[1] = j + 0.5f;

		dist = Length( pixel - center );

		if( dist < smallRadius )
		{
			/* полностью непрозрачный */
			alpha = 1.0f;
		}
		else if( dist > bigRadius )
		{
			/* полностью прозрачный */
			alpha = 0.0f;
		}
		else
		{
			// нормализуем значение так, что теперь
			// dist изменяется в пределах [0; 1]
			dist = (dist - smallRadius)/(bigRadius - smallRadius);
			alpha = -pow(dist, degree) + 1.0f;
		}
		int index = j * texSize + i;
		ptr[index * 4 + 0] = 255;
		ptr[index * 4 + 1] = 255;
		ptr[index * 4 + 2] = 255;
		ptr[index * 4 + 3] = (unsigned char) floor( _clampf(255 * alpha, 0, 255) );
	}
	SDL_Surface* s = SDL_CreateRGBSurfaceFrom(ptr, texSize, texSize, 32, 0, 0xff000000, 0xff0000, 0xff00, 0xff);
	std::shared_ptr<SDL_Surface> surf( s, SDL_FreeSurface );
	SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_BLEND);

	return Texture::LoadPicture(s, GL_LINEAR);
}


std::shared_ptr<FrozenCanvas> FrozenCanvas::Create(tinyxml2::XMLElement* e, std::shared_ptr<Texture> background, float screenWidth, float screenHeight)
{
	std::shared_ptr<FrozenCanvas> canvas( new FrozenCanvas );
	std::shared_ptr<Texture> brush;

	auto element = e->FirstChildElement("filename");
	if( element != nullptr )
	{
		brush = Texture::LoadPicture( element->GetText(), GL_LINEAR );
	}
	else
	{
		float smallRadius = 32.0f;
		float bigRadius = 64.0f;
		float degree = 0.0f;

		element = e->FirstChildElement("smallradius");
		if( element != nullptr )
			smallRadius = (float) atof( element->GetText() ? element->GetText() : "32.0" );

		element = e->FirstChildElement("bigradius");
		if( element != nullptr )
			bigRadius = (float) atof( element->GetText() ? element->GetText() : "64.0" );

		element = e->FirstChildElement("gradient");
		if( element != nullptr )
			degree = (float) atof( element->GetText() ? element->GetText() : "0.0" );

		brush = GenerateBrush(smallRadius, bigRadius, degree);
	}
	float compareRadius = 20.0f;
	float fadeTime = 7.0f;
	int maxParticles = 500;

	element = e->FirstChildElement("fadetime");
	if( element != nullptr )
		fadeTime = (float) atof( element->GetText() ? element->GetText() : "7.0" );

	element = e->FirstChildElement("maxparticles");
	if( element != nullptr )
		maxParticles = atoi( element->GetText() ? element->GetText() : "500" );

	element = e->FirstChildElement("compareradius");
	if( element != nullptr )
	{
		const char* text = element->GetText();

		int percent;
		char percentSymbol;
		int ret = sscanf_s(text, "%d%c", &percent, &percentSymbol);

		if( ret == 2 && percentSymbol == '%' ) {
			percent = _clampi( percent, 0, 100 );

			// берём среднее меджду шириной и высотой и берём от этого percent процентов
			compareRadius = (brush->Width() + brush->Height())/2.0f * percent/100.0f;
		}
		else {
			compareRadius = (float) atof( text ? text : "20.0" );
		}
	}

	canvas->m_compareRadius = compareRadius;
	canvas->m_particleLife = fadeTime;
	canvas->m_maxParticles = maxParticles;

	canvas->m_sprite = Sprite::Create( (float)brush->Width(), (float)brush->Height() );
	canvas->m_sprite->SetTexture(brush, 0);
	canvas->m_sprite->GenerateFixedTexCoords(0, 0, 0, (float)brush->Width(), (float)brush->Height());
	canvas->m_sprite->SetAnchorNormalized(0.5f, 0.5f);
	canvas->m_sprite->SetTexture(background, 1, Sprite::TEXGEN_SCREENSPACE);
	canvas->m_sprite->EnableScreenSpaceTexCoordsGeneration(screenWidth, screenHeight);

	int cols = 16 * 2;
	int rows =  9 * 2;
	canvas->m_cellWidth = screenWidth/cols;
	canvas->m_cellHeight = screenHeight/rows;

	canvas->m_width = screenWidth;
	canvas->m_height = screenHeight;
	canvas->m_gridRows = rows;
	canvas->m_gridCols = cols;
	canvas->m_cells.resize( cols*rows );

	return canvas;
}

void FrozenCanvas::SpawnParticle(float life, float x, float y)
{
	if( m_particleCount >= m_maxParticles )
		return;

	CanvasCell* cell = GetCell(x, y);

	// запоминаем старое значение
	std::size_t cellParticleCount = cell->GetCount();

	Particle* p = cell->GetParticle(x, y, m_compareRadius);
	p->life = m_particleLife;
	//p->x = x;
	//p->y = y;

	// количество частиц изменилось, делаем подсчёт
	if( cell->GetCount() > cellParticleCount )
		++m_particleCount;
}

void FrozenCanvas::Frame(int nextFrameDelta)
{
	float dt = nextFrameDelta/1000.0f;

	for( int i = 0; i < m_gridCols; ++i )
	for( int j = 0; j < m_gridRows; ++j )
	{
		CanvasCell* cell = GetCell(i, j);
		Particle* p;

		for( std::size_t k = 0; k < cell->GetCount(); ++k )
		{
			p = cell->GetParticle(k);
			p->life -= dt;
			if( p->life < 0 )
			{
				cell->EraseParticle(k);

				// количество частиц изменилось, делаем подсчёт
				--m_particleCount;
			}
		}
	}
}

void FrozenCanvas::Render()
{	
	for( int i = 0; i < m_gridCols; ++i )
	for( int j = 0; j < m_gridRows; ++j )
	{
		CanvasCell* cell = GetCell(i, j);
		Particle* p;

		for( std::size_t k = 0; k < cell->GetCount(); ++k )
		{
			p = cell->GetParticle(k);
			if( p->life <= 0 )
				continue;

			m_sprite->SetAlpha( p->life/m_particleLife );
			m_sprite->Render( p->x, p->y );
		}
	}

	//if( m_indraw ) {
	//	// прорисовка дополнительной частицы
	//	m_sprite->SetAlpha(1.0f);
	//	m_sprite->Render(m_pos[0], m_pos[1]);
	//}

}

void FrozenCanvas::DrawDebugInfo(FontRenderer* font)
{
	char buf[64];

	for( int i = 0; i < m_gridCols; ++i )
	for( int j = 0; j < m_gridRows; ++j )
	{
		CanvasCell* cell = GetCell(i, j);
		sprintf_s(buf, 64, "%d", cell->GetCount());
		font->RenderText( (i + 0.5f)*m_cellWidth, (j + 0.5f)*m_cellHeight, buf );
	}
}

void FrozenCanvas::Drawing(bool indraw)
{
	if( indraw && !m_indraw ) {
		m_prevPos = m_pos;
	}
	m_indraw = indraw;
}

void FrozenCanvas::DrawAt(float x, float y)
{
	// запоминаем для рисования в текущем положении дополнительной частицы 
	m_pos[0] = x;
	m_pos[1] = y;

	if( !m_indraw )
		return;

	SpawnParticle(m_particleLife, x, y);
}