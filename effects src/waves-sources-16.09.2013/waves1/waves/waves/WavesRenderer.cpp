
#include "WavesRenderer.h"



std::auto_ptr<WavesRenderer> WavesRenderer::Create(WaveEquationSolver* solver, int screenWidth, int screenHeight)
{
	std::auto_ptr<WavesRenderer> renderer( new WavesRenderer );
	renderer->m_solver = solver;

	int xCount = solver->CountX();
	int yCount = solver->CountY();
	float gridStep = solver->GetGridStep();

	int coordCount = 3 * xCount * yCount;
	int texCoordCount = 2 * xCount * yCount;
	int indexCount = ( 2*yCount - 1 )*( xCount - 1 ) + 1;
	int attribCount = xCount * yCount;

	std::shared_ptr<float> vertices( new float[coordCount] );
	std::shared_ptr<float> normals( new float[coordCount] );
	std::shared_ptr<float> texCoords( new float[texCoordCount] );
	std::shared_ptr<float> attributes( new float[attribCount] );
	std::shared_ptr<unsigned int> indices( new unsigned int[indexCount] );

	int currv = 0, curri = 0;
	float* vptr = vertices.get();
	float* nptr = normals.get();
	float* tptr = texCoords.get();
	unsigned int* iptr = indices.get();

	for( int i = 0; i < xCount; ++i )
	for( int j = 0; j < yCount; ++j )
	{
		vptr[ 0 + 3 * solver->Idx(i,j) ] = i * gridStep;
		vptr[ 1 + 3 * solver->Idx(i,j) ] = j * gridStep;
		vptr[ 2 + 3 * solver->Idx(i,j) ] = 0;

		nptr[ 0 + 3 * solver->Idx(i,j) ] = 0;
		nptr[ 1 + 3 * solver->Idx(i,j) ] = 0;
		nptr[ 2 + 3 * solver->Idx(i,j) ] = 1;

		tptr[ 0 + 2 * solver->Idx(i,j) ] = i*gridStep / screenWidth;
		tptr[ 1 + 2 * solver->Idx(i,j) ] = j*gridStep / screenHeight;
	}

	for( int i = 0; i < solver->CountX()-1; ++i )
	{
		if( (i&1) == 0 ) {
			for( int j = 0; j < solver->CountY(); ++j )
			{
				iptr[curri++] = solver->Idx(i,j);
				iptr[curri++] = solver->Idx(i+1,j);
			}
		}
		else { 
			for( int j = solver->CountY()-1; j > 0; --j )
			{
				iptr[curri++] = solver->Idx(i+1,j);
				iptr[curri++] = solver->Idx(i,j-1);
			}
		}
	}
	if( (xCount & 1) == 1 )
		iptr[curri++] = solver->Idx(xCount-1, 0);

	renderer->m_indexCount = indexCount;

	renderer->m_vbo.Bind( GL_ARRAY_BUFFER );
	renderer->m_vbo.SetData( sizeof(float) * coordCount, vptr, GL_STATIC_DRAW );
	renderer->m_vbo.Unbind();

	renderer->m_nbo.Bind( GL_ARRAY_BUFFER );
	renderer->m_nbo.SetData( sizeof(float) * coordCount, nptr, GL_DYNAMIC_COPY );
	renderer->m_nbo.Unbind();

	renderer->m_tcbo.Bind( GL_ARRAY_BUFFER );
	renderer->m_tcbo.SetData( sizeof(float) * texCoordCount, tptr, GL_STATIC_DRAW );
	renderer->m_tcbo.Unbind();

	renderer->m_ibo.Bind( GL_ELEMENT_ARRAY_BUFFER );
	renderer->m_ibo.SetData( sizeof(unsigned) * indexCount, iptr, GL_STATIC_DRAW );
	renderer->m_ibo.Unbind();

	renderer->m_ubo.Bind( GL_ARRAY_BUFFER );
	memset(attributes.get(), 0, sizeof(float) * attribCount);
	renderer->m_ubo.SetData( sizeof(float) * attribCount, attributes.get(), GL_DYNAMIC_COPY );
	renderer->m_ubo.Unbind();

	return renderer;
}


void WavesRenderer::Render(ShaderProgram* shader)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_NORMALIZE);

	m_ubo.Bind( GL_ARRAY_BUFFER );
	float* attribData = (float*)m_ubo.MapBuffer( GL_WRITE_ONLY );

	for( int i = 0; i < m_solver->CountX(); ++i )
	for( int j = 0; j < m_solver->CountY(); ++j )
	{
		attribData[ m_solver->Idx(i,j) ] = m_solver->Get(i,j);
	}
	m_ubo.UnmapBuffer();
	GLint attrib = shader->GetAttrib("zCoord");

	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	m_ubo.Unbind();

	m_vbo.Bind( GL_ARRAY_BUFFER );
#ifdef TEST_SKEW_VIEW
	float* vertexBuffer = (float*)m_vbo.MapBuffer( GL_WRITE_ONLY );
	for( int i = 0; i < m_solver->CountX(); ++i )
	for( int j = 0; j < m_solver->CountY(); ++j )
	{
		vertexBuffer[ 2 + 3 * m_solver->Idx(i,j) ] = m_solver->Get(i,j);
	}
	m_vbo.UnmapBuffer();
#endif
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	m_vbo.Unbind();

	m_nbo.Bind( GL_ARRAY_BUFFER );
	float n[3];
	float* normalData = (float*)m_nbo.MapBuffer( GL_WRITE_ONLY );

	for( int i = 0; i < m_solver->CountX(); ++i )
	for( int j = 0; j < m_solver->CountY(); ++j )
	{
		m_solver->CalculateNormal(i,j,n);
		normalData[ 0 + 3 * m_solver->Idx(i,j) ] = n[0];
		normalData[ 1 + 3 * m_solver->Idx(i,j) ] = n[1];
		normalData[ 2 + 3 * m_solver->Idx(i,j) ] = n[2];
	}
	m_nbo.UnmapBuffer();
	glNormalPointer(GL_FLOAT, 0, nullptr);
	m_nbo.Unbind();

	m_tcbo.Bind( GL_ARRAY_BUFFER );
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
	m_tcbo.Unbind();	

	m_ibo.Bind( GL_ELEMENT_ARRAY_BUFFER );
	glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);
	m_ibo.Unbind();

	glDisableVertexAttribArray(attrib);
	glDisable(GL_NORMALIZE);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void WavesRenderer::Release()
{
	m_ibo.Release();
	m_tcbo.Release();
	m_vbo.Release();
}