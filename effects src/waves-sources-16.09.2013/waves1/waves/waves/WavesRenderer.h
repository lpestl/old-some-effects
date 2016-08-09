
#ifndef __WAVESRENDERER_H__
#define __WAVESRENDERER_H__

#include "Texture.h"
#include "FrameBuffer.h"
#include "ShaderProgram.h"
#include "WaveEquationSolver.h"
#include "VertexBuffer.h"


struct Vertex3d
{
	float x;
	float y;
	float z;
};

struct TexCoord2d
{
	union { float s; float u; };
	union { float t; float v; };
};


class WavesRenderer
{
private:
	WavesRenderer()
	{
		m_solver = nullptr;
		m_indexCount = 0;
	}
public:
	static std::auto_ptr<WavesRenderer> Create(WaveEquationSolver* solver, int screenWidth, int screenHeight);
	void Render(ShaderProgram* shader);
	void Release();

private:
	WaveEquationSolver* m_solver;
	int m_indexCount;

	VertexBuffer m_vbo;
	VertexBuffer m_nbo;
	VertexBuffer m_ibo;
	VertexBuffer m_tcbo;
	VertexBuffer m_ubo;
};


#endif //__WAVESRENDERER_H__