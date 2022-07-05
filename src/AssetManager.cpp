#include "AssetManager.hpp"

AssetManager::AssetManager(u_int disp_w, u_int disp_h) 
{
    this->disp_w = disp_w;
    this->disp_h = disp_h;

    currentPalette = nullptr;

    pen = std::regex("PEN_\\d+");

    // keep a standard set of brushes - some, like BONES in FALL, don't bring them over in the MTI
    stdBrushes.emplace(std::string("WHITE"), new Brush((u_char)0, (u_char)0, (u_char)0, (u_char)255));

}

AssetManager::~AssetManager()
{   
    clear();

    auto biter = stdBrushes.begin();
    while(biter != stdBrushes.end())
    {
        //delete &(titer->first);
        delete biter->second;
        biter++;
    }
    stdBrushes.clear();
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
    char* ext = 0;

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

            // ensure mesh shaders are loaded
            std::string sname = std::string("Mesh");
            GLuint shader = findShader(&sname);

            if(shader == 0)
            {
                shader = loadShaderProgram("../src/shaders/Mesh.vert", "../src/shaders/Mesh.frag");
                shaders.emplace(sname, shader);
            }

            // load kurt model
            t = std::string(bni->records[0].title);
            l = bni->records[0].len;
            d = bni->records[0].data;

            Model* kurt = new Model(t, d, false);
            models.emplace(t, kurt);

            // load max "bones" model
            t = std::string(bni->records[4].title);
            l = bni->records[4].len;
            d = bni->records[4].data;

            Model* max = new Model(t, d, false);
            models.emplace(t, max);

            // load missile model
            t = std::string(bni->records[6].title);
            l = bni->records[6].len;
            d = bni->records[6].data;

            Model* missile = new Model(t, d, false);
            models.emplace(t, missile);

            // load chute model
            t = std::string(bni->records[7].title);
            l = bni->records[7].len;
            d = bni->records[7].data;

            Model* chute = new Model(t, d, false);
            models.emplace(t, chute);

            // load item models
            for(int i = 9; i < 22; i++)
            {
                t = std::string(bni->records[i].title);
                l = bni->records[i].len;
                d = bni->records[i].data;

                Model* m = new Model(t, d, true);
                models.emplace(t, m);
            }

            // load dummy model
            t = std::string(bni->records[22].title);
            l = bni->records[22].len;
            d = bni->records[22].data;

            Model* dummy = new Model(t, d, false);
            models.emplace(t, dummy);

            // load H150 model
            t = std::string(bni->records[23].title);
            l = bni->records[23].len;
            d = bni->records[23].data;

            Model* h150 = new Model(t, d, false);
            models.emplace(t, h150);

            // load thump model
            t = std::string(bni->records[24].title);
            l = bni->records[24].len;
            d = bni->records[24].data;

            Model* thump = new Model(t, d, false);
            models.emplace(t, thump);

            // load twist model
            t = std::string(bni->records[25].title);
            l = bni->records[25].len;
            d = bni->records[25].data;

            Model* twist = new Model(t, d, false);
            models.emplace(t, twist);

            // load inter model
            t = std::string(bni->records[26].title);
            l = bni->records[26].len;
            d = bni->records[26].data;

            Model* inter = new Model(t, d, false);
            models.emplace(t, inter);


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
                else if (record.metaf == 0)// && (record.metaa == record.metab == (u_short)65535)) // brush
                {
                    std::string namestr = std::string(record.title);
                    u_char r = 0;
                    u_char g = 0;
                    u_char b = 0;
                    u_short off = 16;

                    if(std::regex_match(namestr, pen))
                    {
                        off = 0;
                    }

                    currentPalette->colorAt(record.metac + off, &r, &g, &b);

                    Brush *brush = new Brush(r, g, b, (u_char)255);
                    brushes.emplace(namestr, brush);
                }

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

    auto biter = brushes.begin();
    while(biter != brushes.end())
    {
        //delete &(titer->first);
        delete biter->second;
        biter++;
    }
    brushes.clear();

    auto miter = models.begin();
    while(miter != models.end())
    {
        //delete &(miter->first);
        delete miter->second;
        miter++;
    }
    models.clear();

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

    auto siter = shaders.begin();
    while(siter != shaders.end())
    {
        //delete &(siter->first);
        glDeleteProgram(siter->second);
        siter++;
    }
    shaders.clear();

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
            // check the standard brushes
            auto itsb = stdBrushes.find(*key);
            if(itsb != stdBrushes.end())
            {
                return itsb->second;
            }
            else
            {
                // if the name matches PEN_# then we know the offset
                // otherwise, just make it transparent

                u_char r = 0;
                u_char g = 0;
                u_char b = 0;
                u_char a = 0;


                if(std::regex_match(*key, pen))
                {
                    // split the offset from PEN_
                    char* dup = strdup((*key).c_str());
                    char* idx = 0;
                    char* pal_idx = 0;

                    (idx = strchr(dup, '_')) ? ++idx : idx = dup;
                    pal_idx = strchr(idx, '_');
                    pal_idx = strndup(idx, pal_idx - idx);

                    std::string name = std::string(pal_idx);
                    u_char pindx = (u_char)std::stoi(name);

                    currentPalette->colorAt(pindx, &r, &g, &b);

                    a = 255;
                    
                }
                
                // lazy create and return brush
                Brush *brush = new Brush(r, g, b, a);
                brushes.emplace(*key, brush);

                return brush;
            }
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