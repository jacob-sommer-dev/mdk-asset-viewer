#ifndef __MDK_CONTEXT_H__
#define __MDK_CONTEXT_H__

#include <vector>

#include "renderer/RenderContext.hpp"
#include "renderer/RenderContextFactory.h"

#include "AssetManager.hpp"

typedef struct _mouse_state {
    bool init = false;
    int x = 0;
    int y = 0;
} MouseState;

class MDK_Context
{

private:

    MouseState mouse;

    u_int disp_w;
    u_int disp_h;
    u_int disp_half_w;
    u_int disp_half_h;

    float m_sensitivity = 0.5f;

    RenderContext* renderer;

    MDK_Context();
    ~MDK_Context();

public:

    float fov;
    Camera* camera;

    bool m_handle = false;

    static constexpr float physT = (1.0f/60.0f); // physics timestep in s

    static MDK_Context* get()
    {
        static MDK_Context* singleton = nullptr;
        if(singleton == nullptr)
        {
            singleton = new MDK_Context();
        }
        assert(singleton);
        return singleton;
    }

    AssetManager *assets;

    int Init(float fov, u_int disp_w, u_int disp_h);
    int Deinit();

    int setDisplayDimens(u_int disp_w, u_int disp_h);
    void getDisplayDimens(u_int *disp_w, u_int *disp_h);
    int setvFoV(float fov);

    void handleSDLKeyEvent(SDL_Event *event);
    void handleSDLMouseEvent(SDL_Event *event);
    void handleSDLEvent(SDL_Event *event);
    void handleKeyStates(const Uint8 *keystates, const float* frameT);

    void doPhysics(float *);

    bool addToRenderList(Renderable *);
    void removeFromRenderList(Renderable *);
    void clearRenderList();

    void render();

    glm::vec3* getCameraPos();

};



#endif // __MDK_CONTEXT_H__