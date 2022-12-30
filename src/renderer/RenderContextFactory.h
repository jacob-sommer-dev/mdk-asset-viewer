#ifndef __RENDERCONTEXTFACTORY_H__
#define __RENDERCONTEXTFACTORY_H__

#include "RenderContext.hpp"
#include "RenderContext_OpenGL.hpp"

typedef enum {
    OpenGL45
} RENDERERS;

RenderContext* getRenderContext(unsigned int, unsigned int, RENDERERS);

#endif //__RENDERCONTEXTFACTORY_H__