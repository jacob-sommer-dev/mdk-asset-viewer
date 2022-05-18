#ifndef __MDK_CONTEXT_H__
#define __MDK_CONTEXT_H__

#include <vector>

#include "renderer/RenderContext.hpp"
#include "renderer/RenderContextFactory.h"

#include "AssetManager.hpp"

class MDK_Context
{

private:

    u_int disp_w;
    u_int disp_h;

    RenderContext* renderer;

    MDK_Context();
    ~MDK_Context();

public:

    float fov;
    Camera* camera;

    static constexpr float physT = (1.0f/60.0f); // physics timestep in s

    static constexpr char* files[] = { "MISC/LOAD_3.LBB",
                            "MISC/LOAD_4.LBB",
                            "MISC/LOAD_5.LBB",
                            "MISC/LOAD_6.LBB",
                            "MISC/LOAD_7.LBB",
                            "MISC/LOAD_8.LBB"
                          };

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