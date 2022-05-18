#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include <vector>
#include "../Camera.hpp"
#include "../ecs/Renderable.hpp"

class RenderContext
{

private:
    

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

    virtual void render() = 0;
};

#endif