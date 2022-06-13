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
        Texture* texture = loadlbb(path, &w, &h);

        if(texture != nullptr)
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

            Widget *wid = new Widget(texture->gltex, w, h, &disp_w, &disp_h, shader);

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
            std::string t = std::string(bni->records[28].title);
            u_int l = bni->records[28].len;
            void* d = bni->records[28].data;

            Palette* p = new Palette();
            p->setData(bni->records[28].data, bni->records[28].len);

            palettes.emplace(std::string(bni->records[28].title), p);
            // palettes.emplace(std::string(bni->records[29].title), new Palette(bni->records[29].data));
            // palettes.emplace(std::string(bni->records[30].title), new Palette(bni->records[30].data));
            // palettes.emplace(std::string(bni->records[31].title), new Palette(bni->records[31].data));
            // palettes.emplace(std::string(bni->records[32].title), new Palette(bni->records[32].data));

            // would be set by specific level load. TODO: Have way to set palette
            auto it = palettes.find(std::string(bni->records[28].title));
            currentPalette = it->second;

            // load kurt model
            t = std::string(bni->records[0].title);
            l = bni->records[0].len;
            d = bni->records[0].data;

            std::string sname = std::string("Mesh");
            GLuint shader = findShader(&sname);

            if(shader == 0)
            {
                shader = loadShaderProgram("../src/shaders/Mesh.vert", "../src/shaders/Mesh.frag");
                shaders.emplace(sname, shader);
            }

            Model* m = new Model(t, d);
            models.emplace(t, m);

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
                        u_short w = *((u_short*)record.data);
                        u_short h = *((u_short*)(record.data) + 1);
                        const void* p;
                        currentPalette->palette(p);
                        Texture* texture = new Texture(p, record.data, sizeof(u_short)*2, w, h, false, false);

                        if(texture != nullptr)
                        {
                            std::string sname = std::string("Widget");
                            GLuint shader = findShader(&sname);

                            if(shader == 0)
                            {
                                shader = loadShaderProgram("../src/shaders/Widget.vert", "../src/shaders/Widget.frag");
                                shaders.emplace(sname, shader);
                            }

                            Widget *wid = new Widget(texture->gltex, w, h, &disp_w, &disp_h, shader);

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
    currentPalette = nullptr;

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
        delete titer->second;
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

Texture* AssetManager::findTexture(std::string* key)
{
    auto it = textures.find(*key);
    return (it != textures.end()) ? it->second : nullptr;
}

Brush* AssetManager::findBrush(std::string* key)
{
    auto it = brushes.find(*key);
    return (it != brushes.end()) ? it->second : nullptr;
}

Material* AssetManager::findMaterial(std::string* key)
{
    auto itt = textures.find(*key);
    if (itt != textures.end())
    {
        return itt->second;
    }
    else 
    {
        auto itb = brushes.find(*key);
        if(itb != brushes.end())
        {
            return itb->second;
        }
        else
        {
            // any checking for specific brushes, maybe add them in constructor so we don't have to do this?
            return nullptr;
        }
    }
}

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

Model* AssetManager::findModel(std::string* key)
{
    auto it = models.find(*key);
    return (it != models.end()) ? it->second : nullptr;
}

void AssetManager::availWidgets(std::vector<std::string>& list)
{
    list.clear();
    for(std::pair<const std::string, Widget*> pair : widgets)
    {
        list.push_back(pair.first);
    }
}

void AssetManager::availModels(std::vector<std::string>& list)
{
    list.clear();
    for(std::pair<const std::string, Model*> pair : models)
    {
        list.push_back(pair.first);
    }
}

void AssetManager::finalizeAssets()
{
    for(std::pair<const std::string, Model*> pair : models)
    {
        pair.second->load();
    }
}