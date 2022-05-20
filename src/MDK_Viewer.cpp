// #include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
// // #include <GL/glu.h>
// // #include <SDL2/SDL_opengl.h>

// #include "dear_imgui/imgui.h"
// #include "dear_imgui/imgui_impl_sdl.h"
// #include "dear_imgui/imgui_impl_opengl3.h"

#include "MDK_Context.hpp"


int SDL_main(int argc, char *args[])
{

    MDK_Context* mdk_context = MDK_Context::get();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("SDL Couldn't Init. SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    atexit(SDL_Quit);
    
    // mdk context
    mdk_context->Init(60.0f, 1920, 1080);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags for additional input
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    Uint32 startTicks = 0;
    Uint32 endTicks = 0;
    float elapsedT = 0.0f;
    float acc = 0.0f;

    // render loop
    bool quit = false;
    while (!quit)
    {
        endTicks = SDL_GetTicks();
        elapsedT = (float)(endTicks - startTicks) * 0.001f;

        // sanity check for physics-breaking values (would produce large error)
        if (elapsedT > 1.0f) // 1 sec
        {
            elapsedT = 1.0f;
        }

        startTicks = endTicks;

        acc += elapsedT;

        const Uint8 *keystates = SDL_GetKeyboardState(NULL);

        SDL_Event event;
        while(SDL_PollEvent(&event) == 1)
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if(event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }

            if(!io.WantCaptureMouse)
            {
                mdk_context->handleSDLKeyEvent(&event);
            }

            if(!io.WantCaptureKeyboard)
            {
                mdk_context->handleSDLKeyEvent(&event);
            }

            mdk_context->handleSDLEvent(&event);
        }

        // give context keystates
        mdk_context->handleKeyStates(keystates, &elapsedT);

        // physics update
        mdk_context->doPhysics(&acc);

        // draw elements
        mdk_context->render();

    }

    mdk_context->Deinit();
    // mdk_context->~MDK_Context();


	SDL_Quit();

	return EXIT_SUCCESS;
}


int main(int argc, char **args)
{
    return SDL_main(argc, args);
}
