#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>
#include <SDL2/SDL.h>

/**
* Load a 2D texture from a palette and data buffer
* 
* @param palette palette of the image to load
* @param data data of the image to load
* @param w width
* @param h height
* @return GLuint the texture's name or 0 if failed
*/
GLuint texLoad(const void* palette, const void* data, int start, unsigned short w, unsigned short h);

/**
* Deallocate a texture
* @param texName the name of the texture to deallocate
*/
void texDestroy(GLuint texName);

#endif