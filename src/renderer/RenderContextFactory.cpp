#include "RenderContextFactory.h"

RenderContext* getRenderContext(unsigned int disp_w, unsigned int disp_h, RENDERERS  renderType)
{
    switch (renderType)
    {
        case OpenGL45:
        default:
            return new RenderContext_OpenGL(disp_w, disp_h);
    }
}