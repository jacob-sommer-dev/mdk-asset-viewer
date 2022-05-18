#include "AssetManager.hpp"

AssetManager::AssetManager(u_int disp_w, u_int disp_h) 
{
    this->disp_w = disp_w;
    this->disp_h = disp_h;

    currentPalette = nullptr;
}

AssetManager::~AssetManager()
{   

}

void AssetManager::nameFromFilename(const std::string *path, std::string& name)
{
    char* dup = strdup((*path).c_str());
    char* fn;
    char *cname;

    (fn = strchr(dup, '/')) ? ++fn : fn = dup;
    cname = strchr(fn, '.');
    cname = strndup(fn, cname - fn);

    name = std::string(cname);
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
            std::string sname;
            nameFromFilename(&fname, sname);

            GLuint shader = findShader(&sname);

            if(shader == 0)
            {
                shader = loadShaderProgram("../src/shaders/Widget.vert", "../src/shaders/Widget.frag");
                shaders.emplace(sname, shader);
            }

            Widget *wid = new Widget(texture, w, h, &disp_w, &disp_h, shader);

            std::string namestr = std::string(name);

            textures.emplace(namestr, texture);
            widgets.emplace(namestr, wid);
        }
    }
    else if (strcmp(ext, "BNI") == 0)
    {
        // For BNIs, memory won't be held by the BNI after this
        // Assets that need access to the memory will need to
        // do a memcpy (most won't)

        char* fn;
        char* name;

        (fn = strchr(dup, '/')) ? ++fn : fn = dup;
        name = strchr(fn, '.');
        name = strndup(fn, name - fn);

        const char *cpath = path->c_str();
        BNI *bni = (BNI*)malloc(sizeof(BNI));
        int parsed = parsebni(cpath, bni);

        if (strcmp(name, "FALL3D") == 0)
        {
            // 28-32 are palettes, load first
            palettes.emplace(std::string(bni->records[28].title), new Palette(bni->records[28].data));
            palettes.emplace(std::string(bni->records[29].title), new Palette(bni->records[29].data));
            palettes.emplace(std::string(bni->records[30].title), new Palette(bni->records[30].data));
            palettes.emplace(std::string(bni->records[31].title), new Palette(bni->records[31].data));
            palettes.emplace(std::string(bni->records[32].title), new Palette(bni->records[32].data));

            // would be set by specific level load. TODO: Have way to set palette
            auto it = palettes.find(std::string(bni->records[28].title));
            currentPalette = it->second;

        }
    }
    else if (strcmp(ext, "MTI") == 0)
    {
        if(currentPalette == nullptr)
        {
            // we shouldn't be loading MTI before the palette in the BNI
            return 1;
        }

        const char *cpath = path->c_str();
        MTI *mti = (MTI*)malloc(sizeof(MTI));
        int parsed = parsemti(cpath, mti);

        if(mti != NULL && parsed == 0)
        {
            for (int i = 0; i < mti->header->num_entries; i++)
            {
                MTI_RECORD record = mti->records[i];
                if(record.metaf == (u_short)16480) // 60 40, TODO: but remember to check for weird entries like in LEVEL_3S.MTI
                {
                    u_int offset = 0;

                    if(record.metab == (u_short)1)
                    {
                        // sprite sheet

                    }
                    else 
                    {
                        // regular texture
                        u_short w = *(((u_short*)record.data));
                        u_short h = *(((u_short*)record.data) + sizeof(u_short));
                        const void* p;
                        currentPalette->palette(p);
                        GLuint texture = texLoad(p, record.data, sizeof(u_short)*2, w, h);

                        if(texture != 0)
                        {
                            std::string fname = std::string("Widget.vert");
                            std::string sname;
                            nameFromFilename(&fname, sname);

                            GLuint shader = findShader(&sname);

                            if(shader == 0)
                            {
                                shader = loadShaderProgram("../src/shaders/Widget.vert", "../src/shaders/Widget.frag");
                                shaders.emplace(sname, shader);
                            }

                            Widget *wid = new Widget(texture, w, h, &disp_w, &disp_h, shader);

                            std::string namestr = std::string(record.title);
                            
                            textures.emplace(namestr, texture);
                            widgets.emplace(namestr, wid);
                            
                        }

                    }
                } 
                
                // skip brushes for now
            }

            freemti(mti);
            free(mti);
        }
    }

    return 0;

}

int AssetManager::clear()
{
    if (currentPalette != nullptr)
    {
        delete currentPalette;
    }

    auto piter = palettes.begin();
    while(piter != palettes.end())
    {
        //delete &(piter->first);
        delete piter->second;
        piter++;
    }
    palettes.clear();

    auto witer = widgets.begin();
    while(witer != widgets.end())
    {
        //delete &(witer->first);
        delete witer->second;
        witer++;
    }
    widgets.clear();

    auto titer = textures.begin();
    while(titer != textures.end())
    {
        //delete &(titer->first);
        glDeleteTextures(1, &(titer->second));
        titer++;
    }
    textures.clear();

    // auto miter = meshes.begin();
    // while(miter != meshes.end())
    // {
    //     delete &(miter->first);
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
    //     //delete &(siter->first);
    //     glDeleteProgram(siter->second);
    //     siter++;
    // }
    // shaders.clear();

    return 0;
}

void* AssetManager::getCurrPalette()
{
    return currentPalette;
}

void* AssetManager::findPalette(std::string* key)
{
    auto it = palettes.find(*key);
    return (it != palettes.end()) ? it->second : nullptr;
}

GLuint AssetManager::findTexture(std::string* key)
{
    auto it = textures.find(*key);
    return (it != textures.end()) ? it->second : 0;
}

// Mesh* AssetManager::findMesh(std::string* key)
// {
//     auto it = meshes.find(*key);
//     return (it != meshes.end()) ? it->second : nullptr;
// }

// Animation* AssetManager::findAnimation(std::string* key)
// {
//     auto it = animations.find(*key);
//     return (it != animations.end()) ? it->second : nullptr;
// }

// int* AssetManager::findWav(std::string* key)
// {
//     auto it = wavs.find(*key);
//     return (it != wavs.end()) ? it->second : nullptr;
// }

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