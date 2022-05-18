#include "RenderContextFactory.h"

RenderContext* getRenderContext(u_int disp_w, u_int disp_h, RENDERERS  renderType)
{
    switch (renderType)
    {
        case OpenGL45:
        default:
            return new RenderContext_OpenGL(disp_w, disp_h);
    }
}