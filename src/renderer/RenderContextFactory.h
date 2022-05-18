#ifndef __RENDERCONTEXTFACTORY_H__
#define __RENDERCONTEXTFACTORY_H__

#include "RenderContext.hpp"
#include "RenderContext_OpenGL.hpp"

typedef enum {
    OpenGL33
} RENDERERS;

RenderContext* getRenderContext(u_int, u_int, RENDERERS);

#endif //__RENDERCONTEXTFACTORY_H__