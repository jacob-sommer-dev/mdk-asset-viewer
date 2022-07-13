#ifndef __RENDERCONTEXT_OPENGL__
#define __RENDERCONTEXT_OPENGL__

#include "RenderContext.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../dear_imgui/imgui.h"
#include "../dear_imgui/imgui_impl_sdl.h"
#include "../dear_imgui/imgui_impl_opengl3.h"

class RenderContext_OpenGL : public RenderContext
{

private:

    glm::mat4 projMat = glm::mat4(1.0f);
    glm::mat4 viewMat = glm::mat4(1.0f);

    u_int disp_w;
    u_int disp_h;

    SDL_GLContext glcontext = NULL;

    ~RenderContext_OpenGL();

public:
    RenderContext_OpenGL(u_int, u_int);
    
    void render() override;

};

#endif