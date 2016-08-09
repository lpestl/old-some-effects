
#include <crtdbg.h>
#include "Rendering.h"



void RenderQuad(const ScreenQuad* q)
{
	_ASSERT(q != nullptr);

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for( int t = 0; t < MAX_TEXTURE_UNITS; ++t )
	{
		glActiveTexture(GL_TEXTURE0 + t);
		if( q->tex[t] > 0 )
		{
			glBindTexture(GL_TEXTURE_2D, q->tex[t]);
			glEnable(GL_TEXTURE_2D);
		}
		else glDisable(GL_TEXTURE_2D);
	}

	glBegin(GL_QUADS);
	for( int i = 0; i < 4; ++i )
	{
		for( int t = 0; t < MAX_TEXTURE_UNITS; ++t )
		{
			if( q->tex[t] > 0 )
				glMultiTexCoord2fv( GL_TEXTURE0 + t, q->v[i].texCoords[t].m_v );
		}
		glColor4fv( q->v[i].color.m_v );
		glVertex2fv( q->v[i].pos.m_v );
	}
	glEnd();
	glPopAttrib();
}