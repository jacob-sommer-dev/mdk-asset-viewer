#include "MDK_Context.hpp"

MDK_Context::MDK_Context() {}

MDK_Context::~MDK_Context() {}

int MDK_Context::Init(float fov, u_int disp_w, u_int disp_h)
{
    //this->fov = fov;
    this->disp_w = disp_w;
    this->disp_h = disp_h;

    assets = new AssetManager(disp_w, disp_h);

    camera = new Camera(fov, (float)disp_w / (float)disp_h);
    viewMat = *(camera->update());
    projMat = glm::perspective(glm::radians(fov), (float)disp_w / (float)disp_h, 1.0f, 1000.0f);

    return 0;
}

int MDK_Context::Deinit()
{
    camera->~Camera();
    assets->clear();
    assets->~AssetManager();

    return 0;
}

int MDK_Context::setDisplayDimens(u_int disp_w, u_int disp_h)
{
    this->disp_w = disp_w;
    this->disp_h = disp_h;

    // camera->setDimensions(disp_w, disp_h);

    return 0;
}

void MDK_Context::getDisplayDimens(u_int *disp_w, u_int *disp_h)
{
    *disp_w = this->disp_w;
    *disp_h = this->disp_h;
}

int MDK_Context::setvFoV(float fov)
{
    //camera->setvFoV();
    return 0;
}

void MDK_Context::handleSDLEvent(SDL_Event *event)
{

}

void MDK_Context::handleKeyStates(const Uint8 *keystates, const Uint32 *ticks)
{
    camera->handleKeys(keystates, ticks);
    /*projMat = */camera->update();
}

bool MDK_Context::addToRenderList(Renderable* renderable)
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

void MDK_Context::removeFromRenderList(Renderable* renderable)
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

void MDK_Context::clearRenderList()
{
    renderList.clear();
}

void MDK_Context::render()
{
    auto it = renderList.begin();
    while(it != renderList.end())
    {
        (*it)->draw(&projMat, &viewMat);
        it++;
    }
}