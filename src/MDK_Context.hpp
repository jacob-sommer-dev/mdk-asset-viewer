#ifndef __MDK_CONTEXT_H__
#define __MDK_CONTEXT_H__

#include <vector>

#include "AssetManager.hpp"
#include "Camera.hpp"

class MDK_Context
{

private:

    float fov;
    u_int disp_w;
    u_int disp_h;

    std::vector<Renderable*> renderList;
    
    Camera *camera;
    glm::mat4 projMat;
    glm::mat4 viewMat;


public:

    AssetManager *assets;

    MDK_Context();
    ~MDK_Context();

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

    int Init(float fov, u_int disp_w, u_int disp_h);
    int Deinit();

    int setDisplayDimens(u_int disp_w, u_int disp_h);
    void getDisplayDimens(u_int *disp_w, u_int *disp_h);
    int setvFoV(float fov);

    void handleSDLEvent(SDL_Event *event);
    void handleKeyStates(const Uint8 *keystates, const Uint32 *ticks);

    bool addToRenderList(Renderable *);
    void removeFromRenderList(Renderable *);
    void clearRenderList();

    void render();

};



#endif // __MDK_CONTEXT_H__