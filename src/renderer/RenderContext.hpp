#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include <vector>
#include "../Camera.hpp"
#include "../ecs/Renderable.hpp"

#include "../dear_imgui/imgui.h"

class RenderContext
{

private:


protected:
    SDL_Window *window = NULL;

    ImVec4 imguiClearColor;
    bool drawImgui = true;

public:

    std::vector<Renderable*> renderList;

    bool addToRenderList(Renderable* renderable)
    {
        // don't add duplicate
        auto it = renderList.begin();
        while(it != renderList.end())
        {
            if (renderable == *it)
            {
                return false;
            }
            it++;
        }
        renderList.push_back(renderable);
        return true;
    }

    void removeFromRenderList(Renderable* renderable)
    {
        auto it = renderList.begin();
        while(it != renderList.end())
        {
            if (renderable == *it)
            {
                renderList.erase(it);
                break;
            }
            it++;
        }
    }

    void clearRenderList()
    {
        renderList.clear();
    }

    void toggleImgui()
    {
        drawImgui = !drawImgui;
    }

    bool imguiActive()
    {
        return drawImgui;
    }

    SDL_Window* getSdlWindow()
    {
        return window;
    }

    virtual void render() = 0;

};

#endif