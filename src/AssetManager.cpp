#include "AssetManager.hpp"

AssetManager::AssetManager(u_int disp_w, u_int disp_h) 
{
    this->disp_w = disp_w;
    this->disp_h = disp_h;
}

AssetManager::~AssetManager()
{   

}

std::string* AssetManager::nameFromFilename(const std::string *path)
{
    char* dup = strdup((*path).c_str());
    char* fn;
    char *cname;

    (fn = strchr(dup, '/')) ? ++fn : fn = dup;
    cname = strchr(fn, '.');
    cname = strndup(fn, cname - fn);

    return new std::string(cname);
}

int AssetManager::loadFromFile(const std::string *path)
{
    char* dup = strdup((*path).c_str());
    char* ext;

    (ext = strchr(dup, '.')) ? ++ext : ext = dup;

    if (strcmp(ext, "LBB") == 0)
    {
        char* fn;
        char* name;

        (fn = strchr(dup, '/')) ? ++fn : fn = dup;
        name = strchr(fn, '.');
        name = strndup(fn, name - fn);

        u_short w,h;
        GLuint texture = loadlbb(path, &w, &h);

        if(texture != 0)
        {
            std::string fname = std::string("Widget.vert");
            std::string* sname = nameFromFilename(&fname);

            GLuint shader = findShader(sname);

            if(shader == 0)
            {
                shader = loadShaderProgram("../src/shaders/Widget.vert", "../src/shaders/Widget.frag");
                shaders.emplace(*sname, shader);
            }

            Widget *wid = new Widget(texture, w, h, &disp_w, &disp_h, shader);

            std::string *namestr = new std::string(name);

            textures.emplace(*namestr, texture);
            widgets.emplace(*namestr, wid);
        }
    }

    return 0;

}

int AssetManager::clear()
{
    // if (currentPalette != nullptr)
    // {
    //     free(currentPalette);
    // }

    // auto piter = palettes.begin();
    // while(piter != palettes.end())
    // {
    //     delete piter->first;
    //     free(piter->second);
    //     piter++;
    // }

    // auto witer = widgets.begin();
    // while(witer != widgets.end())
    // {
    //     delete witer->first;
    //     delete witer->second;
    //     witer++;
    // }

    // auto titer = textures.begin();
    // while(titer != textures.end())
    // {
    //     delete titer->first;
    //     glDeleteTextures(1, &(titer->second));
    //     titer++;
    // }

    // auto miter = meshes.begin();
    // while(miter != meshes.end())
    // {
    //     delete miter->first;
    //     delete miter->second;
    //     miter++;
    // }

    // auto aiter = animations.begin();
    // while(aiter != animations.end())
    // {
    //     delete aiter->first;
    //     delete aiter->second;
    //     aiter++;
    // }

    // auto waiter = wavs.begin();
    // while(waiter != wavs.end())
    // {
    //     delete waiter->first;
    //     free(waiter->second);
    //     waiter++;
    // }

    // auto siter = shaders.begin();
    // while(siter != shaders.end())
    // {
    //     delete siter->first;
    //     glDeleteProgram(siter->second);
    //     siter++;
    // }

    return 0;
}

char* AssetManager::getCurrPalette()
{
    return currentPalette;
}

char* AssetManager::findPalette(std::string* key)
{
    auto it = palettes.find(*key);
    return (it != palettes.end()) ? it->second : nullptr;
}

GLuint AssetManager::findTexture(std::string* key)
{
    auto it = textures.find(*key);
    return (it != textures.end()) ? it->second : 0;
}

Mesh* AssetManager::findMesh(std::string* key)
{
    auto it = meshes.find(*key);
    return (it != meshes.end()) ? it->second : nullptr;
}

Animation* AssetManager::findAnimation(std::string* key)
{
    auto it = animations.find(*key);
    return (it != animations.end()) ? it->second : nullptr;
}

int* AssetManager::findWav(std::string* key)
{
    auto it = wavs.find(*key);
    return (it != wavs.end()) ? it->second : nullptr;
}

GLuint AssetManager::findShader(std::string* key)
{
    auto it = shaders.find(*key);
    return (it != shaders.end()) ? it->second : 0;
}

Widget* AssetManager::findWidget(std::string* key)
{
    auto it = widgets.find(*key);
    return (it != widgets.end()) ? it->second : nullptr;
}