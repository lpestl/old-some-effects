
#ifndef __RENDERING_H__
#define __RENDERING_H__

#include <gl/glew.h>
#include "Mathematics.h"

enum Max
{
	MAX_TEXTURE_UNITS = 8,
};


struct Vertex2d
{
	Vec2f pos;
	Vec4f color;
	Vec2f texCoords[MAX_TEXTURE_UNITS];
};

struct ScreenQuad
{
	Vertex2d v[4];
	GLuint tex[MAX_TEXTURE_UNITS];
};

struct Rect
{
	float x;
	float y;
	float width;
	float height;
};


void RenderQuad(const ScreenQuad* q);


#endif //__RENDERING_H__