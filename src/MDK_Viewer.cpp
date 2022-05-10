#include <GL/glew.h>
#include <SDL2/SDL.h>
// #include <GL/glu.h>
// #include <SDL2/SDL_opengl.h>

#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_impl_sdl.h"
#include "dear_imgui/imgui_impl_opengl3.h"

#include "MDK_Context.hpp"

const unsigned int DISP_WIDTH = 1280;
const unsigned int DISP_HEIGHT = 720;


int SDL_main(int argc, char *args[])
{

    SDL_Window *window = NULL;
    SDL_GLContext glcontext = NULL;

    MDK_Context* mdk_context = MDK_Context::get();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("SDL Couldn't Init. SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // create window
    // TODO: WxH should be an option
    window = SDL_CreateWindow("MDK Asset Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL couldn't init window.", NULL);
        printf("SDL couldn't init window. SDL Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // create gl context
	glcontext = SDL_GL_CreateContext(window);
    if(glcontext == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create OpenGL Context", NULL);
        printf("SDL coudln't init gl context. SDL Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // init glew
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't Init GLEW.", NULL);
        printf("Can't Init GLEW. Error: %s\n", glewGetErrorString(glewError));
        return EXIT_FAILURE;
    }

    // set window and context current
	if (SDL_GL_MakeCurrent(window, glcontext)) 
    {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't make window/context current", NULL);
        printf("Couldn't make window/context current. Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// try to determine "best" vsync. TODO: This should be an option
	if (SDL_GL_SetSwapInterval(-1) < 0) // try adaptive vsync
    { 
		if (SDL_GL_SetSwapInterval(1) < 0) // try vsync
        { 
			SDL_GL_SetSwapInterval(0); // no vsync, safe default
		}
	}

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// set the clear color to dark grey
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepthf(1.0f);

	// set up imgui
	if (!IMGUI_CHECKVERSION()) 
    {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "ImGui CHECKVERSION failed", NULL);
		return EXIT_FAILURE;
	}
	if (!ImGui::CreateContext()) 
    {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "ImGui couldn't create context", NULL);
		return EXIT_FAILURE;
	}
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags for additional input
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// set up platform/renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImVec4 imguiClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // mdk context
    mdk_context->Init(60.0f, DISP_WIDTH, DISP_HEIGHT);

    const std::string str3 = std::string("MISC/LOAD_3.LBB");
    mdk_context->assets->loadFromFile(&str3);

    const std::string str4 = std::string("MISC/LOAD_4.LBB");
    mdk_context->assets->loadFromFile(&str4);

    const std::string str5 = std::string("MISC/LOAD_5.LBB");
    mdk_context->assets->loadFromFile(&str5);

    const std::string str6 = std::string("MISC/LOAD_6.LBB");
    mdk_context->assets->loadFromFile(&str6);

    const std::string str7 = std::string("MISC/LOAD_7.LBB");
    mdk_context->assets->loadFromFile(&str7);

    const std::string str8 = std::string("MISC/LOAD_8.LBB");
    mdk_context->assets->loadFromFile(&str8);

    Uint32 startTicks = 0;
    Uint32 endTicks = 0;
    Uint32 elapsed = 0;

    // render loop
    bool quit = false;
    while (!quit)
    {

        endTicks = SDL_GetTicks();
        elapsed = endTicks - startTicks;
        startTicks = SDL_GetTicks();

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

            }

            if(!io.WantCaptureKeyboard)
            {
                switch (event.type)
                {
                    case SDL_KEYUP:
                        if (event.key.keysym.sym == SDLK_3)
                        {
                            mdk_context->clearRenderList();
                            std::string str2 = std::string("LOAD_3");
                            Widget* widget = mdk_context->assets->findWidget(&str2);
                            mdk_context->addToRenderList(widget);
                        }
                        else if (event.key.keysym.sym == SDLK_4)
                        {
                            mdk_context->clearRenderList();
                            std::string str2 = std::string("LOAD_4");
                            Widget* widget = mdk_context->assets->findWidget(&str2);
                            mdk_context->addToRenderList(widget);
                        }
                        else if (event.key.keysym.sym == SDLK_5)
                        {
                            mdk_context->clearRenderList();
                            std::string str2 = std::string("LOAD_5");
                            Widget* widget = mdk_context->assets->findWidget(&str2);
                            mdk_context->addToRenderList(widget);
                        }
                        else if (event.key.keysym.sym == SDLK_6)
                        {
                            mdk_context->clearRenderList();
                            std::string str2 = std::string("LOAD_6");
                            Widget* widget = mdk_context->assets->findWidget(&str2);
                            mdk_context->addToRenderList(widget);
                        }
                        else if (event.key.keysym.sym == SDLK_7)
                        {
                            mdk_context->clearRenderList();
                            std::string str2 = std::string("LOAD_7");
                            Widget* widget = mdk_context->assets->findWidget(&str2);
                            mdk_context->addToRenderList(widget);
                        }
                        else if (event.key.keysym.sym == SDLK_8)
                        {
                            mdk_context->clearRenderList();
                            std::string str2 = std::string("LOAD_8");
                            Widget* widget = mdk_context->assets->findWidget(&str2);
                            mdk_context->addToRenderList(widget);
                        }
                }
            }
        }

        // clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // give camera keystates
        mdk_context->handleKeyStates(keystates, &elapsed);

        // get camera matrices
        //glm::mat4 *viewMat = camera.update();

        // draw elements
        mdk_context->render();

        // build and render imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, World!");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Elapsed frametime ms: %d", elapsed);
        
        // glm::vec3 *campos = camera.getPosition();
        // ImGui::Text("Camera X: %.3f Y: %.3f Z: %.3f", campos->x, campos->y, campos->z);

        

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // present
        SDL_GL_SwapWindow(window);

    }

    mdk_context->Deinit();
    mdk_context->~MDK_Context();

    // clean up imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

    SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}


int main(int argc, char *args[])
{
    return SDL_main(argc, args);
}
