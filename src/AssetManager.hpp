#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include <string>
#include <map>
#include <vector>
#include <regex>

#include <GL/glew.h>

#include "assets/Palette.hpp"
#include "assets/Shader.hpp"
#include "assets/Widget.hpp"
#include "assets/Material.hpp"
#include "assets/Texture.hpp"
#include "assets/Brush.hpp"
#include "assets/Mesh.hpp"
#include "assets/Animation.hpp"
#include "assets/Model.hpp"

#include "assets/mdk_lbb.hpp"
#include "assets/mdk_mti.h"
#include "assets/mdk_bni.h"

class AssetManager
{

private:
    std::regex pen;

    u_int disp_w;
    u_int disp_h;

    Palette* currentPalette = nullptr;
    std::map<std::string, Palette *> palettes;
    std::map<std::string, Widget *> widgets;
    std::map<std::string, Texture *> textures;
    std::map<std::string, Brush *> brushes;
    std::map<std::string, Brush *> stdBrushes;
    // std::map<std::string, Mesh *> meshes;
    // std::map<std::string, Animation *> animations;
    // std::map<std::string, int *> wavs;
    std::map<std::string, GLuint> shaders;
    std::map<std::string, Model *> models;

    

public:

    AssetManager(u_int disp_w, u_int disp_h);
    ~AssetManager();

    void nameFromFilename(const std::string *, std::string &);

    int loadFromFile(const std::string *);
    int clear();
    void* getCurrPalette();
    void* findPalette(std::string *);
    Texture* findTexture(std::string *);
    Brush* findBrush(std::string *);
    Material* findMaterial(std::string *);
    int* findWav(std::string *);
    GLuint findShader(std::string *);

    Widget* findWidget(std::string *);
    Model* findModel(std::string *);

    void availWidgets(std::vector<std::string>&);
    void availModels(std::vector<std::string>&);

    void finalizeAssets();

};


#endif //__ASSETMANAGER_H__