#include "RenderContext_OpenGL.hpp"

#include "../MDK_Context.hpp"

MDK_Context* mdk_context = nullptr;

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

RenderContext_OpenGL::RenderContext_OpenGL(u_int disp_width, u_int disp_height)
{
    mdk_context = MDK_Context::get();

    disp_w = disp_width;
    disp_h = disp_height;

    viewMat = *(mdk_context->camera->update());
    projMat = glm::perspective(glm::radians(mdk_context->fov), (float)disp_w / (float)disp_h, 1.0f, 1000.0f);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    // create window
    // TODO: WxH should be an option
    window = SDL_CreateWindow("MDK Asset Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, disp_w, disp_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL couldn't init window.", NULL);
        printf("SDL couldn't init window. SDL Error: %s\n", SDL_GetError());
        //return EXIT_FAILURE;
    }

    // create gl context
	glcontext = SDL_GL_CreateContext(window);
    if(glcontext == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create OpenGL Context", NULL);
        printf("SDL coudln't init gl context. SDL Error: %s\n", SDL_GetError());
        //return EXIT_FAILURE;
    }

    // init glew
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't Init GLEW.", NULL);
        printf("Can't Init GLEW. Error: %s\n", glewGetErrorString(glewError));
        //return EXIT_FAILURE;
    }

    // set window and context current
	if (SDL_GL_MakeCurrent(window, glcontext)) 
    {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't make window/context current", NULL);
        printf("Couldn't make window/context current. Error: %s\n", SDL_GetError());
		//return EXIT_FAILURE;
	}

    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

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
	glClearColor(1.0f, 0.2f, 1.0f, 1.0f);//(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepthf(1.0f);

    // set up imgui
	if (!IMGUI_CHECKVERSION()) 
    {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "ImGui CHECKVERSION failed", NULL);
		//return EXIT_FAILURE;
	}
	if (!ImGui::CreateContext()) 
    {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "ImGui couldn't create context", NULL);
		//return EXIT_FAILURE;
	}

	// set up platform/renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
	ImGui_ImplOpenGL3_Init("#version 330");

	imguiClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

}

RenderContext_OpenGL::~RenderContext_OpenGL()
{
    // clean up imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

    SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
}

void RenderContext_OpenGL::render()
{
        // clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw elements
        auto it = RenderContext::renderList.begin();
        while(it != RenderContext::renderList.end())
        {
            (*it)->draw(&projMat, &viewMat);
            it++;
        }

        // build and render imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, World!");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // ImGui::Text("Elapsed frametime ms: %.3f", elapsedT);
        // ImGui::Text("Elapsed acc after phys ms: %.3f", acc);
        
        glm::vec3 *campos = mdk_context->getCameraPos();
        ImGui::Text("Camera X: %.3f Y: %.3f Z: %.3f", campos->x, campos->y, campos->z);

        static int file_prev = -1;
        static int file_current = -1;
        ImGui::Combo("Files", &file_current, MDK_Context::files, IM_ARRAYSIZE(MDK_Context::files));
        if(file_prev != file_current)
        {
            file_prev = file_current;

            // clear render list
            mdk_context->clearRenderList();
            // clear the loaded assets
            mdk_context->assets->clear();

            // load the file
            const std::string str = std::string(MDK_Context::files[file_current]);
            mdk_context->assets->loadFromFile(&str);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // present
        SDL_GL_SwapWindow(window);
}