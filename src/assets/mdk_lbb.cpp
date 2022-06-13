#include "mdk_lbb.hpp"

Texture* loadlbb(const std::string *filepath, u_short *w, u_short *h)
{

    SDL_RWops *srw = SDL_RWFromFile((*filepath).c_str(), "rb");
    if (srw != NULL)
    {
        char palette[768];
        if(srw->read(srw, palette, sizeof(char), 768) != 768)
        {
            // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL couldn't read palette.", NULL);
            printf("SDL couldn't load file. SDL Error: %s\n", SDL_GetError());
            // return EXIT_FAILURE;
        }

        if(srw->read(srw, w, sizeof(short), 1) != 1)
        {
            // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL couldn't read w.", NULL);
            printf("SDL couldn't load file. SDL Error: %s\n", SDL_GetError());
            // return EXIT_FAILURE;
        }
        
        if(srw->read(srw, h, sizeof(short), 1) != 1)
        {
            // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL couldn't read h.", NULL);
            printf("SDL couldn't load file. SDL Error: %s\n", SDL_GetError());
            // return EXIT_FAILURE;
        }

        int len = *w * *h;

        char tex[len];
        if(srw->read(srw, tex, sizeof(char), len) != len)
        {
            // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL couldn't read img.", NULL);
            printf("SDL couldn't load file. SDL Error: %s\n", SDL_GetError());
            // return EXIT_FAILURE;
        }

        srw->close(srw);

        return new Texture(palette, tex, 0, *w, *h, false, false);
    }

    return 0;
}