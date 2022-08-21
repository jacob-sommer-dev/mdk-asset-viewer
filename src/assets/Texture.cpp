#include "Texture.hpp"

Texture::Texture(const void* palette, const void* data, int start, unsigned short w, unsigned short h, bool mips, bool vflip)
{
	type = TEXTURE_DIFFUSE;
	this->w = w;
	this->h = h;

	// turn palette and alpha data into RGB(A) here

	int idx = start;
	int imod = 0;
	int idiv = 0;

	int stride = 3;
	int len = w * h;
	int dlen = len * 4;
	int didx;
	unsigned char data2[dlen]{0};

	for(int i = 0; i < len; i++)
	{
		unsigned char pd = *((unsigned char*)data + sizeof(char) * i + start);

		if(vflip) // may not need this anymore, but keep it for now just in case
		{
			imod = i % w;
			idiv = i / w;

			idx = len - w - (w * idiv) + imod + start;

			pd = *((unsigned char*)data + sizeof(char) * idx);
		}

		unsigned char *pp = (unsigned char*)palette + sizeof(char) * pd*stride;

		didx = i * 4;

		data2[didx] = *pp; // + sizeof(char) * 0;
		data2[didx + 1] = *(pp + sizeof(char));// * 1;
		data2[didx + 2] = *(pp + sizeof(char) * 2);
		data2[didx + 3] = ((unsigned char)255);

	}

	// create texture
	glGenTextures(1, &gltex);
	glBindTexture(GL_TEXTURE_2D, gltex);

	// set filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(mips)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);

	if(mips)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

}

Texture::~Texture()
{
	if(gltex > 0)
	{
		glDeleteTextures(1, &gltex);
	}
	w = 0;
	h = 0;
}

// GLuint texLoad2(const void* palette, const void* data, int start, unsigned short w, unsigned short h)
// {

// 	// turn palette and alpha data into RGB(A) here

// 	int idx = start;
// 	int imod = 0;
// 	int idiv = 0;

// 	int stride = 3;
// 	int len = w * h;
// 	int dlen = len * 4;
// 	int didx;
// 	unsigned char data2[dlen]{0};

// 	for(int i = 0; i < len; i++)
// 	{
// 		// OpenGL expects top left to be 0,0, so flip it while doing the conversion
// 		imod = i % w;
// 		idiv = i / w;

// 		idx = len - w - (w * idiv) + imod + start;

// 		unsigned char pd = *((unsigned char*)data + sizeof(char) * idx);

// 		// if(pd > ((u_char)15))
// 		// {
// 		// 	stride = 4;
// 		// }
// 		// else
// 		// {
// 		// 	stride = 3;
// 		// }

// 		unsigned char *pp = (unsigned char*)palette + sizeof(char) * pd*stride;

// 		didx = i * 4;

// 		data2[didx] = *pp; // + sizeof(char) * 0;
// 		data2[didx + 1] = *(pp + sizeof(char));// * 1;
// 		data2[didx + 2] = *(pp + sizeof(char) * 2);
// 		if (stride == 3)
// 		{
// 			data2[didx + 3] = ((unsigned char)255);
// 		}
// 		else 
// 		{
// 			data2[didx + 3] = *(pp + sizeof(char) * 3);
// 		}
		

		
// 	}

// 	// create texture
// 	GLuint texture;
// 	glGenTextures(1, &texture);
// 	glBindTexture(GL_TEXTURE_2D, texture);
	
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);

// 	// set filtering
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

// 	// cleanup

// 	return texture;

// }

// void texDestroy(GLuint texName) 
// {
// 	glDeleteTextures(1, &texName);
// }