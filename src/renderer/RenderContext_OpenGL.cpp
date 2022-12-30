#include "RenderContext_OpenGL.hpp"

#include "../MDK_Context.hpp"
#include "../FileSet.h"

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

RenderContext_OpenGL::RenderContext_OpenGL(unsigned int disp_width, unsigned int disp_height)
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
	glClearColor(0.3f, 0.1f, 0.3f, 1.0f);//(0.2f, 0.2f, 0.2f, 1.0f);
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

    viewMat = *(mdk_context->camera->update());

    // draw elements
    auto it = RenderContext::renderList.begin();
    while(it != RenderContext::renderList.end())
    {
        (*it)->draw(&projMat, &viewMat);
        it++;
    }

    if(drawImgui)
    {
        // build and render imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        ImGui::Begin("Asset Selector");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // ImGui::Text("Elapsed frametime ms: %.3f", elapsedT);
        // ImGui::Text("Elapsed acc after phys ms: %.3f", acc);
            
        glm::vec3 *campos = mdk_context->getCameraPos();
        ImGui::Text("Camera X: %.3f Y: %.3f Z: %.3f", campos->x, campos->y, campos->z);

        static int file_prev = -1;
        static int file_current = -1;
        bool fileChanged = false;
        ImGui::Combo("Files", &file_current, FILESETS, IM_ARRAYSIZE(FILESETS));
        if(file_prev != file_current)
        {
            file_prev = file_current;
            fileChanged = true;

            // clear render list
            clearRenderList();
            // clear the loaded assets
            mdk_context->assets->clear();

            FileSet fileset;
            findFileSet(FILESETS[file_current], fileset);    

            // load the fileset
            for(int i = 0; i < fileset.size; i++)
            {
                const std::string str = std::string(fileset.fileset[i]);
                mdk_context->assets->loadFromFile(&str);
            }

            mdk_context->assets->finalizeAssets();
              
        }

        // radio buttons for asset type
        static int r_prev = -1;
        static int r_current = -1;
        ImGui::RadioButton("Textures", &r_current, 0); ImGui::SameLine();
        ImGui::RadioButton("Models", &r_current, 1);

        // asset list
        static std::vector<std::string> assets;
        static const char* items[100];

        if(r_prev != r_current || fileChanged)
        {
            r_prev = r_current;

            switch (r_current)
            {
                case 0:
                    mdk_context->assets->availWidgets(assets);
                    break;
                case 1:
                    mdk_context->assets->availModels(assets);
                    break;
            }
                
            for(int i = 0; i < assets.size(); i++)
            {
                items[i] = assets[i].c_str();
            }

        }

        static int selected_prev = -1;
        static int selected_current = -1;

        if(fileChanged)
        {
            selected_prev = -1;
            selected_current = -1;
        }

        ImGui::Combo("Assets", &selected_current, items, assets.size());

        if(selected_prev != selected_current)
        {
            selected_prev = selected_current;

            // clear render list
            clearRenderList();

            std::string sel = std::string(items[selected_current]);
                
            switch (r_current)
            {
                case 0:
                {
                    Widget* wid = mdk_context->assets->findWidget(&sel);
                    addToRenderList(wid);
                    break;
                }
                    
                case 1:
                {
                    Model* m = mdk_context->assets->findModel(&sel);
                    addToRenderList(m);
                    break;
                }
                    

            }

        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // present
    SDL_GL_SwapWindow(window);
}