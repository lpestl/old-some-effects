
#ifndef __FONTRENDERER_H__
#define __FONTRENDERER_H__

#include <gl/glew.h>
#include <memory>
#include "Texture.h"
#include "FileSystem.h"


struct Letter
{
	int x, y, w, h;
	int pre, post;
	bool empty;

	Letter() { x = y = w = h = pre = post = 0; empty = true; }
};


class FontRenderer
{
private:
	FontRenderer()
	{
	}

public:
	static const int max_letters = 256;

	static std::auto_ptr<FontRenderer> LoadFont(const char* file)
	{
		std::auto_ptr<FontRenderer> renderer( new FontRenderer );
		ProfileReader font(file);
		memset(&renderer->m_letters, 0, sizeof(Letter) * max_letters);

		std::string record;
		char key[16];
		int lettersCount = 0;
		renderer->m_letterHeight = 0;

		for( int i = 0; i < max_letters; ++i )
		{
			if( i == 0x0ff )
				record = "";
			sprintf_s(key,16,"x%03X",i);
			record = font.GetString("HGEFONT",key);
			if( record.empty() )
				continue;

			Letter a;
			int scanned = sscanf_s(record.c_str(), "%d,%d,%d,%d,%d,%d", &a.x, &a.y, &a.w, &a.h, &a.pre, &a.post);
			if( scanned != 6 )
				continue;
			a.empty = false;
			renderer->m_letters[i] = a;
			renderer->m_letterHeight = __max( a.h, renderer->m_letterHeight );
			lettersCount++;
		}
		if( lettersCount < 1 ) {
			renderer->m_texture = Texture::CreateBlank(16, 16, GL_NEAREST, GL_RGB, GL_RGB8);
			return renderer;
		}

		std::string bitmap = font.GetString("HGEFONT","Bitmap");
		if( bitmap.empty() ) {
			renderer->m_texture = Texture::CreateBlank(16, 16, GL_NEAREST, GL_RGB, GL_RGB8);
			return renderer;
		}

		std::string path = file;
		std::string::size_type backSlash = path.find_last_of("\\/", path.npos);
		if( backSlash < path.size() )
			path = path.substr(0, backSlash);
		else path = "";

		path += "/" + bitmap;
		renderer->m_texture = Texture::LoadPicture(path.c_str(), GL_NEAREST, false);

		return renderer;
	}

	void RenderText(float xstart, float ystart, const char* text)
	{
		float xpos = xstart;
		float ypos = ystart;

		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_texture->Bind();
		glColor3f(1,1,1);

		while( *text )
		{
			if( *text == '\n' )
			{
				xpos = xstart;
				ypos += m_letterHeight * 1.5f;
			}
			else
			{
				unsigned i = (unsigned)(*text);
				if( i > max_letters-1 )
					i = '?';
				if( m_letters[i].empty )
					i = '?';

				if( m_letters[i].empty )
					continue;

				xpos += (float)m_letters[i].pre;
				RenderLetter(xpos, ypos, m_letters[i]);
				xpos += (float)m_letters[i].w + (float)m_letters[i].post;
			}
			++text;
		}
		m_texture->Unbind();

		glPopAttrib(); //GL_COLOR_BUFFER_BIT
	}

private:
	void RenderLetter(int x, int y, Letter& a)
	{
		float tw = (float)m_texture->Width();
		float th = (float)m_texture->Height();
		
		glBegin(GL_QUADS);
		glTexCoord2f( a.x/tw, (a.y + a.h)/th );
		glVertex2i(x, y);
			
		glTexCoord2f( (a.x + a.w)/tw, (a.y + a.h)/th );
		glVertex2i(x + a.w, y);
			
		glTexCoord2f( (a.x + a.w)/tw, a.y/th );
		glVertex2i(x + a.w, y + a.h);
			
		glTexCoord2f(a.x/tw, a.y/th);
		glVertex2i(x, y + a.h);
		glEnd();
	}

private:
	std::auto_ptr<Texture> m_texture;
	Letter m_letters[max_letters];
	float m_letterHeight;
};


#endif //__FONTRENDERER_H__