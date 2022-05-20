#include "MDK_Context.hpp"

MDK_Context::MDK_Context() {}

MDK_Context::~MDK_Context() {}

int MDK_Context::Init(float fov, u_int disp_w, u_int disp_h)
{

    this->fov = fov;
    this->disp_w = disp_w;
    this->disp_h = disp_h;

    camera = new Camera(fov, (float)disp_w / (float)disp_h);

    renderer = getRenderContext(disp_w, disp_h, OpenGL45);

    assets = new AssetManager(disp_w, disp_h);

    return 0;
}

int MDK_Context::Deinit()
{
    delete renderer;
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

void MDK_Context::handleSDLKeyEvent(SDL_Event *eventp)
{
    // SDL_Event event = *eventp;
    // switch ((event).type)
    // {
    //     case SDL_KEYUP:
    //         if (event.key.keysym.sym == SDLK_3)
    //         {
    //             std::string str2 = std::string("LOAD_3");
    //             Widget* widget = assets->findWidget(&str2);
    //             if(widget != nullptr)
    //                 addToRenderList(widget);
    //         }
    //         else if (event.key.keysym.sym == SDLK_4)
    //         {
    //             std::string str2 = std::string("LOAD_4");
    //             Widget* widget = assets->findWidget(&str2);
    //             if(widget != nullptr)
    //                 addToRenderList(widget);
    //         }
    //         else if (event.key.keysym.sym == SDLK_5)
    //         {
    //             std::string str2 = std::string("LOAD_5");
    //             Widget* widget = assets->findWidget(&str2);
    //             if(widget != nullptr)
    //                 addToRenderList(widget);
    //         }
    //         else if (event.key.keysym.sym == SDLK_6)
    //         {
    //             std::string str2 = std::string("LOAD_6");
    //             Widget* widget = assets->findWidget(&str2);
    //             if(widget != nullptr)
    //                 addToRenderList(widget);
    //         }
    //         else if (event.key.keysym.sym == SDLK_7)
    //         {
    //             std::string str2 = std::string("LOAD_7");
    //             Widget* widget = assets->findWidget(&str2);
    //             if(widget != nullptr)
    //                 addToRenderList(widget);
    //         }
    //         else if (event.key.keysym.sym == SDLK_8)
    //         {
    //             std::string str2 = std::string("LOAD_8");
    //             Widget* widget = assets->findWidget(&str2);
    //             if(widget != nullptr)
    //                 addToRenderList(widget);
    //         }
    // }
}

void MDK_Context::handleSDLMouseEvent(SDL_Event *event)
{

}

void MDK_Context::handleSDLEvent(SDL_Event *event)
{

}

void MDK_Context::handleKeyStates(const Uint8 *keystates, const float* frameT)
{
    camera->handleKeys(keystates, frameT);
    /*projMat = */camera->update();
}

void MDK_Context::doPhysics(float* acc)
{
    // full physics steps
    while (*acc >= physT)
    {
        // TODO update simulation by a full step
        *acc -= physT;
    }
    // interpolated state to actually render
    const float alpha = *acc / physT;
    // TODO interpolate current state + alpha

}

bool MDK_Context::addToRenderList(Renderable* renderable)
{
    return renderer->addToRenderList(renderable);
}

void MDK_Context::removeFromRenderList(Renderable* renderable)
{
    renderer->removeFromRenderList(renderable);
}

void MDK_Context::clearRenderList()
{
    renderer->clearRenderList();
}

void MDK_Context::render()
{
    renderer->render();
}

glm::vec3* MDK_Context::getCameraPos()
{
    return camera->getPosition();
}