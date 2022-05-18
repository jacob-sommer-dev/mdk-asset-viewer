#ifndef __RENDERCONTEXT_OPENGL__
#define __RENDERCONTEXT_OPENGL__

#include "RenderContext.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

#include "../dear_imgui/imgui.h"
#include "../dear_imgui/imgui_impl_sdl.h"
#include "../dear_imgui/imgui_impl_opengl3.h"

class RenderContext_OpenGL : public RenderContext
{

private:

    glm::mat4 projMat;
    glm::mat4 viewMat;

    u_int disp_w;
    u_int disp_h;

    SDL_Window *window = NULL;
    SDL_GLContext glcontext = NULL;

    ImVec4 imguiClearColor;

    ~RenderContext_OpenGL();

public:
    RenderContext_OpenGL(u_int, u_int);
    
    void render() override;

};

#endif