
#ifndef __WAVEEQUATIONSOLVER_H__
#define __WAVEEQUATIONSOLVER_H__



class WaveEquationSolver
{
public:
	WaveEquationSolver();
	~WaveEquationSolver();

	void CreateGrid(float width, float height, float gridStep, int timeStep, float speed);
	void PerturbationIdx(int i, int j);
	void PerturbationCoord(float x, float y);
	void SolveOneStep();
	void SolveNextFrame(int nextFrameDelta);
	void Release();
	
	int CountX() const { return m_xCount; }
	int CountY() const { return m_yCount; }

	int Idx(int i, int j) const { return j * m_xCount + i; }
	float Get(int i, int j) const { return m_u1[ Idx(i,j) ]; }

	float GetGridStep() const { return m_gridStep; }
	void SetSpeed(float speed) { m_speed = speed; }
	void SetDamping(float damping)
	{
		if( damping < 0 ) damping = 0;
		if( damping > 1 ) damping = 1;
		m_damping = 0.99f + damping * 0.01f;
	}
	void CalculateNormal(int i, int j, float* n);

private:
	float* m_u0;
	float* m_u1;
	float* m_u2;

	float m_dtAccumulated;
	float m_gridStep;
	float m_timeStep;
	float m_speed;
	float m_damping;
	float m_perturbMultipler;
	int m_xCount;
	int m_yCount;
};


#endif //__WAVEEQUATIONSOLVER_H__