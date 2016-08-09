
#ifndef __FROZENCANVAS_H__
#define __FROZENCANVAS_H__

#include <vector>
#include <list>
#include <tinyxml2.h>
#include "Sprite.h"
#include "FontRenderer.h"
#include "ShaderProgram.h"



struct Particle
{
	float x;
	float y;
	float life;
};


class CanvasCell
{
public:
	inline bool _equalCoords( float x1, float x2, float radius ) {
		return fabs(x2 - x1) <= radius;
	}

	Particle* GetParticle(float x, float y, float radius)
	{
		radius = __max(1.0f, radius);
		for( std::size_t i = 0; i < particles.size(); ++i )
		{
			if( _equalCoords(x, particles[i].x, radius) && _equalCoords(y, particles[i].y, radius) )
			{
				return &particles[i];
			}
		}
		Particle p;
		p.x = x;
		p.y = y;

		particles.push_back( p );
		return &particles.back();
	}

	Particle* GetParticle(std::size_t idx)
	{
		return &particles[idx];
	}

	std::size_t GetCount() const
	{
		return particles.size();
	}

	void EraseParticle(std::size_t idx)
	{
		_ASSERT( particles.size() > 0 );
		particles[idx] = particles.back();
		particles.resize( particles.size() - 1 );
	}

private:
	std::vector<Particle> particles;
};


class FrozenCanvas
{
private:
	FrozenCanvas()
	{
		m_indraw = false;
		//m_maxStep = 30.0f;
		m_particleLife = 5.0f;
		m_maxParticles = 500;
		m_particleCount = 0;
		m_compareRadius = 20.0f;
	}

	CanvasCell* GetCell(int i, int j);
	CanvasCell* GetCell(float x, float y);
	void SpawnParticle(float life, float x, float y);

public:
	static std::shared_ptr<FrozenCanvas> Create(tinyxml2::XMLElement* e, std::shared_ptr<Texture> background, float screenWidth, float screenHeight);

	void Frame(int nextFrameDelta);
	void Render();
	void DrawDebugInfo(FontRenderer* font);
	void Drawing(bool indraw);
	void DrawAt(float x, float y);
	int GetParticleCount() const { return m_particleCount; }

private:
	int m_gridCols, m_gridRows;
	float m_width, m_height;
	float m_cellWidth, m_cellHeight;

	bool m_indraw;
	Vec3f m_pos, m_prevPos;
	//float m_maxStep;
	float m_currx, m_curry;
	float m_particleLife;
	float m_compareRadius;
	int m_maxParticles;
	int m_particleCount;

	std::shared_ptr<Sprite> m_sprite;
	std::vector<CanvasCell> m_cells;
};



#endif //__FROZENCANVAS_H__