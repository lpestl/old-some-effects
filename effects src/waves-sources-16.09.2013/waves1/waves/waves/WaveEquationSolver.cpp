
#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>
#include "WaveEquationSolver.h"
#include "Mathematics.h"


inline float _round(float f)
{
	return f>0 ? floor(f + 0.5f) : floor(f - 0.5f);
}


WaveEquationSolver::WaveEquationSolver()
{
	m_u0 = nullptr;
	m_u1 = nullptr;
	m_u2 = nullptr;
	m_dtAccumulated = 0;
	m_damping = 0.99f;
}

WaveEquationSolver::~WaveEquationSolver()
{
	Release();
}

void WaveEquationSolver::CreateGrid(float width, float height, float gridStep, int timeStep, float speed)
{
	m_xCount = (int)_round( width/gridStep ) + 1;
	m_yCount = (int)_round( height/gridStep ) + 1;
		
	m_gridStep = gridStep;
	m_timeStep = timeStep/1000.0f;
	m_speed = speed;
	m_perturbMultipler = 2.0f * width/1920.0f; //для разрешения экрана 1920х1080 получим 2

	m_u0 = new float[m_xCount * m_yCount];
	m_u1 = new float[m_xCount * m_yCount];
	m_u2 = new float[m_xCount * m_yCount];

	memset(m_u0, 0, sizeof(float) * m_xCount * m_yCount);
	memset(m_u1, 0, sizeof(float) * m_xCount * m_yCount);
	memset(m_u2, 0, sizeof(float) * m_xCount * m_yCount);
}

void WaveEquationSolver::PerturbationIdx(int i, int j)
{
	if( m_xCount < 10 || m_yCount < 10 )
		return;

	if( i<3 ) i = 3;
	if( j<3 ) j = 3;
	if( i > m_xCount-4 ) i = m_xCount-4;
	if( j > m_yCount-4 ) j = m_yCount-4;

	float dist;
	float height;
	//
	// Возмущение по косинусу
	//
	for( int ri = -2; ri <= 2; ++ri )
	for( int rj = -2; rj <= 2; ++rj )
	{
		float& u1 = m_u1[ Idx(i+ri,j+rj) ];
		float& u2 = m_u1[ Idx(i+ri,j+rj) ];

		dist = sqrt( (float)ri*ri + (float)rj*rj );
		height = cos( dist/3.0f * (float)M_PI ) + 1;
		height *= m_perturbMultipler;

		u1 += height;
		u2 = u1;
	}
}

void WaveEquationSolver::PerturbationCoord(float x, float y)
{
	int i = (int)_round( x/m_gridStep );
	int j = (int)_round( y/m_gridStep );
	PerturbationIdx( i,j );
}

void WaveEquationSolver::SolveOneStep()
{
	float dudx;
	float dudy;

	float a2 = m_speed * m_speed;
	float t2 = m_timeStep * m_timeStep;
	float h2 = m_gridStep * m_gridStep;
	//
	// Проход по всем узлам, исключая граничные
	//
	for( int i = 1; i < m_xCount-1; ++i )
	for( int j = 1; j < m_yCount-1; ++j )
	{
		dudx = m_u1[ Idx(i+1,j) ] - 2*m_u1[ Idx(i,j) ] + m_u1[ Idx(i-1,j) ];
		dudy = m_u1[ Idx(i,j+1) ] - 2*m_u1[ Idx(i,j) ] + m_u1[ Idx(i,j-1) ];

		m_u2[ Idx(i,j) ] = a2/h2 * (dudx + dudy) + (2*m_u1[ Idx(i,j) ] - m_u0[ Idx(i,j) ])/t2;
		m_u2[ Idx(i,j) ] *= t2 * m_damping;
	}
	std::swap( m_u0, m_u1 );
	std::swap( m_u1, m_u2 );
}

void WaveEquationSolver::SolveNextFrame(int nextFrameDelta)
{
	float dt = nextFrameDelta/1000.0f + m_dtAccumulated;
	int stepCount = (int) floor( dt/m_timeStep );
	m_dtAccumulated = dt - stepCount * m_timeStep;

	for( int i = 0; i < stepCount; ++i )
		SolveOneStep();
}

void WaveEquationSolver::Release()
{
	delete [] m_u0;
	delete [] m_u1;
	delete [] m_u2;
	m_u0 = nullptr;
	m_u1 = nullptr;
	m_u2 = nullptr;
}

void WaveEquationSolver::CalculateNormal(int i, int j, float* n)
{
	if( i>0 && j>0 && i<m_xCount-1 && j<m_yCount-1 )
	{
		Vec3f X1 = MakeVec<float>( (i-1)*m_gridStep, j*m_gridStep, m_u1[ Idx(i-1,j) ] );
		Vec3f X2 = MakeVec<float>( (i+1)*m_gridStep, j*m_gridStep, m_u1[ Idx(i+1,j) ] );

		Vec3f Y1 = MakeVec<float>( i*m_gridStep, (j-1)*m_gridStep, m_u1[ Idx(i,j-1) ] );
		Vec3f Y2 = MakeVec<float>( i*m_gridStep, (j+1)*m_gridStep, m_u1[ Idx(i,j+1) ] );

		Vec3f N = CrossProduct(X2 - X1, Y2 - Y1);
		n[0] = N[0];
		n[1] = N[1];
		n[2] = N[2];
		return;
	}
	n[0] = 0;
	n[1] = 0;
	n[2] = 1;
}