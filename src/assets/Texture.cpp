#include "Texture.hpp"

GLuint texLoad(const void* palette, const void* data, unsigned short w, unsigned short h, bool alpha) 
{

	// turn palette and alpha data into RGB(A) here

	int idx;
	int imod = 0;
	int idiv = 0;

	int mult = (alpha) ? 4 : 3;
	int len = w * h;
	int dlen = len * mult;
	int didx;
	unsigned char data2[dlen]{0};
	for(int i = 0; i < len; i++)
	{
		// image data seems to be upside-down, so flip it while doing the conversion
		imod = i % w;
		idiv = i / w;

		idx = len - w - (w * idiv) + imod;

		unsigned char pd = *((unsigned char*)data + sizeof(char) * idx);
		unsigned char *pp = (unsigned char*)palette + sizeof(char) * pd*mult;

		didx = i * mult;

		data2[didx] = *pp; // + sizeof(char) * 0;
		data2[didx + 1] = *(pp + sizeof(char));// * 1;
		data2[didx + 2] = *(pp + sizeof(char) * 2);
		if(alpha)
		{
			data2[didx + 3] = *(pp + sizeof(char) * 3);
		}
		
	}

	// create texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if(alpha)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) 
	{
		SDL_Log("Creating texture failed, code %u\n", err);
		glDeleteTextures(1, &texture);
		texture = 0;
		return 0;
	}

	// set filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// cleanup

	return texture;

}

void texDestroy(GLuint texName) 
{
	glDeleteTextures(1, &texName);
}